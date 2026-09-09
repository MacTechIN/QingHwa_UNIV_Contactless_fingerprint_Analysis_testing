// =============================================================================
//  capture_mf.cpp — Media Foundation 카메라 캡처 (Windows 전용, dev_plan V1)
//
//  [왜 Media Foundation인가]
//  research.md §2.1이 재현성의 필수 조건으로 못박은 것이 "노출·초점·화이트밸런스
//  수동 고정"이다. 이걸 안 하면 조명을 바꿔도 센서의 자동 노출이 보상해 버려서
//  "조도를 올렸더니 인식률이 올랐다"는 실험 자체가 성립하지 않는다.
//  Windows에서 이 제어를 얻는 표준 경로가 Media Foundation의
//  IAMCameraControl / IAMVideoProcAmp (DirectShow 호환 인터페이스)다.
//
//  [COM 수명 관리]
//  MF는 COM 기반이라 참조 카운트 실수가 곧 누수/크래시다.
//  원시 포인터를 직접 다루지 않고 ComPtr(RAII 래퍼)로만 잡는다.
// =============================================================================
#ifdef _WIN32

#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <shlwapi.h>
// IAMCameraControl / IAMVideoProcAmp — DirectShow 시절 인터페이스지만
// MF 미디어 소스에서 QueryInterface로 그대로 얻을 수 있다. strmif.h에 선언되어 있다.
// (vidcap.h가 아니다 — 그쪽은 IKsTopologyInfo 등 KS 계층용이다)
#include <strmif.h>
#include <control.h>

#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>

#include "cfp/capture.hpp"

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "shlwapi.lib")

namespace cfp {

namespace {

// -----------------------------------------------------------------------------
//  최소 COM 스마트 포인터.
//  <wrl/client.h>의 Microsoft::WRL::ComPtr을 쓸 수도 있으나, 의존을 늘리지 않기 위해
//  필요한 기능(생성/복사금지/해제/주소전달)만 20줄로 구현한다.
// -----------------------------------------------------------------------------
template <class T>
class ComPtr {
public:
    ComPtr() = default;
    ~ComPtr() { reset(); }
    ComPtr(const ComPtr&) = delete;
    ComPtr& operator=(const ComPtr&) = delete;
    ComPtr(ComPtr&& o) noexcept : p_(o.p_) { o.p_ = nullptr; }
    ComPtr& operator=(ComPtr&& o) noexcept {
        if (this != &o) { reset(); p_ = o.p_; o.p_ = nullptr; }
        return *this;
    }
    T*  get()  const noexcept { return p_; }
    T*  operator->() const noexcept { return p_; }
    T** put() noexcept { reset(); return &p_; }          // 출력 인자용
    explicit operator bool() const noexcept { return p_ != nullptr; }
    void reset() noexcept { if (p_) { p_->Release(); p_ = nullptr; } }
private:
    T* p_ = nullptr;
};

std::string to_utf8(const wchar_t* w) {
    if (!w) return {};
    const int n = ::WideCharToMultiByte(CP_UTF8, 0, w, -1, nullptr, 0, nullptr, nullptr);
    if (n <= 1) return {};
    std::string s(static_cast<std::size_t>(n - 1), '\0');
    ::WideCharToMultiByte(CP_UTF8, 0, w, -1, s.data(), n, nullptr, nullptr);
    return s;
}

}  // namespace

// =============================================================================
//  MF 전역 초기화 — 프로세스당 1회
// =============================================================================
struct MfRuntime {
    bool ok = false;
    MfRuntime() {
        // COINIT_APARTMENTTHREADED: 카메라 장치는 STA에서 다루는 것이 안전하다.
        const HRESULT hr1 = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        // 이미 초기화된 경우(RPC_E_CHANGED_MODE 등)도 실패로 보지 않는다.
        const HRESULT hr2 = ::MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
        ok = SUCCEEDED(hr2);
        (void)hr1;
    }
    ~MfRuntime() { if (ok) ::MFShutdown(); ::CoUninitialize(); }
};

// =============================================================================
//  장치 열거
// =============================================================================
std::vector<CameraInfo> enumerate_cameras() {
    std::vector<CameraInfo> out;
    ComPtr<IMFAttributes> attr;
    if (FAILED(::MFCreateAttributes(attr.put(), 1))) return out;
    attr->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                  MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

    IMFActivate** devices = nullptr;
    UINT32 count = 0;
    if (FAILED(::MFEnumDeviceSources(attr.get(), &devices, &count))) return out;

    for (UINT32 i = 0; i < count; ++i) {
        CameraInfo info;
        info.index = static_cast<int>(i);
        WCHAR* name = nullptr; UINT32 len = 0;
        if (SUCCEEDED(devices[i]->GetAllocatedString(
                MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &name, &len))) {
            info.name = to_utf8(name);
            ::CoTaskMemFree(name);
        }
        out.push_back(std::move(info));
        devices[i]->Release();
    }
    ::CoTaskMemFree(devices);
    return out;
}

// =============================================================================
//  MediaFoundationCamera 구현
// =============================================================================
class MediaFoundationCamera::Impl {
public:
    MfRuntime           rt;
    ComPtr<IMFSourceReader> reader;
    ComPtr<IMFMediaSource>  source;
    int  width = 0, height = 0;
    bool ok = false;
    std::string error;

    bool open(int device_index, int want_w, int want_h);
    bool grab(cv::Mat& bgr_out);
    bool lock_auto_controls();
    bool set_manual(long exposure_log2, long focus, bool lock_wb);
};

// -----------------------------------------------------------------------------
//  카메라 열기 + 포맷 협상
//
//  [핵심 로직 해설] MF는 원하는 포맷을 "요청"하면 드라이버가 근사치를 준다.
//  우리는 RGB32를 요청한다 — 드라이버가 MJPG/NV12를 내더라도 MF의 내장 디코더가
//  RGB32로 변환해 주기 때문이다. 직접 YUV 변환 코드를 쓰지 않아도 되고,
//  포맷별 분기가 사라진다.
//  단, 변환 비용이 있으므로 고해상도 고프레임에서는 네이티브 NV12를 받아
//  cv::cvtColor(COLOR_YUV2BGR_NV12)로 직접 변환하는 것이 빠르다(백로그).
// -----------------------------------------------------------------------------
bool MediaFoundationCamera::Impl::open(int device_index, int want_w, int want_h) {
    if (!rt.ok) { error = "MFStartup 실패"; return false; }

    ComPtr<IMFAttributes> attr;
    if (FAILED(::MFCreateAttributes(attr.put(), 1))) { error = "MFCreateAttributes 실패"; return false; }
    attr->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                  MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

    IMFActivate** devices = nullptr;
    UINT32 count = 0;
    if (FAILED(::MFEnumDeviceSources(attr.get(), &devices, &count)) || count == 0) {
        error = "카메라를 찾을 수 없습니다";
        if (devices) ::CoTaskMemFree(devices);
        return false;
    }
    if (device_index < 0 || device_index >= static_cast<int>(count)) device_index = 0;

    // IID_PPV_ARGS는 (REFIID, void**) 두 인자로 전개된다.
    // IID_PPV_ARGS_Helper를 직접 쓰면 인자 순서가 뒤바뀌어 컴파일되지 않는다.
    HRESULT hr = devices[device_index]->ActivateObject(IID_PPV_ARGS(source.put()));
    for (UINT32 i = 0; i < count; ++i) devices[i]->Release();
    ::CoTaskMemFree(devices);
    if (FAILED(hr)) { error = "장치 활성화 실패"; return false; }

    ComPtr<IMFAttributes> ratt;
    if (SUCCEEDED(::MFCreateAttributes(ratt.put(), 1))) {
        // 비디오 처리(포맷 변환)를 리더가 대신 해 주도록 켠다.
        ratt->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    }
    if (FAILED(::MFCreateSourceReaderFromMediaSource(source.get(), ratt.get(), reader.put()))) {
        error = "SourceReader 생성 실패";
        return false;
    }

    ComPtr<IMFMediaType> mt;
    if (FAILED(::MFCreateMediaType(mt.put()))) { error = "MediaType 생성 실패"; return false; }
    mt->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    mt->SetGUID(MF_MT_SUBTYPE,    MFVideoFormat_RGB32);
    if (want_w > 0 && want_h > 0)
        ::MFSetAttributeSize(mt.get(), MF_MT_FRAME_SIZE,
                             static_cast<UINT32>(want_w), static_cast<UINT32>(want_h));

    if (FAILED(reader->SetCurrentMediaType(
            static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), nullptr, mt.get()))) {
        error = "RGB32 포맷 협상 실패";
        return false;
    }

    // 실제로 협상된 크기를 되읽는다. 요청과 다를 수 있다.
    ComPtr<IMFMediaType> cur;
    if (SUCCEEDED(reader->GetCurrentMediaType(
            static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), cur.put()))) {
        UINT32 w = 0, h = 0;
        if (SUCCEEDED(::MFGetAttributeSize(cur.get(), MF_MT_FRAME_SIZE, &w, &h))) {
            width  = static_cast<int>(w);
            height = static_cast<int>(h);
        }
    }
    ok = (width > 0 && height > 0);
    if (!ok) error = "프레임 크기를 확인할 수 없습니다";
    return ok;
}

// -----------------------------------------------------------------------------
//  프레임 획득
//
//  [핵심 로직 해설] MF 버퍼는 "상하 반전(bottom-up)"인 경우가 있다.
//  MF_MT_DEFAULT_STRIDE가 음수면 첫 행이 영상의 마지막 행이라는 뜻이다.
//  이걸 무시하면 화면이 뒤집혀 나오고, 더 나쁘게는 세그멘테이션의 상하 판별
//  (손끝이 위인가)이 통째로 반대가 된다.
//  cv::flip으로 바로잡는다.
// -----------------------------------------------------------------------------
bool MediaFoundationCamera::Impl::grab(cv::Mat& bgr_out) {
    if (!ok) return false;

    ComPtr<IMFSample> sample;
    DWORD stream_flags = 0;
    LONGLONG ts = 0;
    const HRESULT hr = reader->ReadSample(
        static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM),
        0, nullptr, &stream_flags, &ts, sample.put());
    if (FAILED(hr)) { error = "ReadSample 실패"; return false; }
    if (!sample) return false;                       // 아직 프레임 없음 — 정상 상태

    ComPtr<IMFMediaBuffer> buf;
    if (FAILED(sample->ConvertToContiguousBuffer(buf.put()))) return false;

    BYTE* data = nullptr;
    DWORD max_len = 0, cur_len = 0;
    if (FAILED(buf->Lock(&data, &max_len, &cur_len))) return false;

    bool flipped = false;
    LONG stride = static_cast<LONG>(width) * 4;
    {
        ComPtr<IMFMediaType> cur;
        UINT32 s = 0;
        if (SUCCEEDED(reader->GetCurrentMediaType(
                static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), cur.put())) &&
            SUCCEEDED(cur->GetUINT32(MF_MT_DEFAULT_STRIDE, &s))) {
            const LONG ls = static_cast<LONG>(static_cast<INT32>(s));
            if (ls < 0) { flipped = true; stride = -ls; } else if (ls > 0) { stride = ls; }
        }
    }

    // BGRA(RGB32) → BGR. 버퍼를 그대로 감싸고 clone으로 소유권을 우리 쪽으로 가져온다.
    // (Unlock 이후 원본 메모리는 무효가 되므로 clone은 선택이 아니라 필수다)
    if (cur_len >= static_cast<DWORD>(stride) * static_cast<DWORD>(height)) {
        cv::Mat bgra(height, width, CV_8UC4, data, static_cast<std::size_t>(stride));
        cv::cvtColor(bgra, bgr_out, cv::COLOR_BGRA2BGR);
        if (flipped) cv::flip(bgr_out, bgr_out, 0);
    }
    buf->Unlock();
    return !bgr_out.empty();
}

// -----------------------------------------------------------------------------
//  자동 제어 잠금 — research.md §2.1의 핵심 요구사항
//
//  [핵심 로직 해설] IAMCameraControl / IAMVideoProcAmp는 DirectShow 시절
//  인터페이스지만, MF 미디어 소스에서 QueryInterface로 그대로 얻을 수 있다.
//  각 컨트롤의 Flags를 CameraControl_Flags_Manual로 바꾸면 자동 조절이 꺼진다.
//
//  주의: 노트북 내장 카메라는 이 인터페이스를 지원하지 않는 경우가 매우 흔하다.
//  (dev_plan 리스크 R2) 실패해도 예외를 던지지 않고 false를 반환해, UI가
//  "이 카메라는 수동 노출 미지원"이라는 경고 배지를 띄울 수 있게 한다.
// -----------------------------------------------------------------------------
bool MediaFoundationCamera::Impl::lock_auto_controls() {
    if (!source) return false;
    bool any = false;

    ComPtr<IAMCameraControl> cam;
    if (SUCCEEDED(source->QueryInterface(IID_PPV_ARGS(cam.put())))) {
        long val = 0, flags = 0;
        if (SUCCEEDED(cam->Get(CameraControl_Exposure, &val, &flags))) {
            if (SUCCEEDED(cam->Set(CameraControl_Exposure, val,
                                   CameraControl_Flags_Manual))) any = true;
        }
        if (SUCCEEDED(cam->Get(CameraControl_Focus, &val, &flags))) {
            if (SUCCEEDED(cam->Set(CameraControl_Focus, val,
                                   CameraControl_Flags_Manual))) any = true;
        }
    }

    ComPtr<IAMVideoProcAmp> amp;
    if (SUCCEEDED(source->QueryInterface(IID_PPV_ARGS(amp.put())))) {
        long val = 0, flags = 0;
        if (SUCCEEDED(amp->Get(VideoProcAmp_WhiteBalance, &val, &flags))) {
            if (SUCCEEDED(amp->Set(VideoProcAmp_WhiteBalance, val,
                                   VideoProcAmp_Flags_Manual))) any = true;
        }
        if (SUCCEEDED(amp->Get(VideoProcAmp_Gain, &val, &flags))) {
            if (SUCCEEDED(amp->Set(VideoProcAmp_Gain, val,
                                   VideoProcAmp_Flags_Manual))) any = true;
        }
    }
    if (!any) error = "이 카메라는 수동 노출/초점 제어를 지원하지 않습니다";
    return any;
}

bool MediaFoundationCamera::Impl::set_manual(long exposure_log2, long focus, bool lock_wb) {
    if (!source) return false;
    bool any = false;
    ComPtr<IAMCameraControl> cam;
    if (SUCCEEDED(source->QueryInterface(IID_PPV_ARGS(cam.put())))) {
        // DirectShow 규약: 노출은 log2(초) 단위 정수다. -6이면 1/64초.
        if (SUCCEEDED(cam->Set(CameraControl_Exposure, exposure_log2,
                               CameraControl_Flags_Manual))) any = true;
        if (focus >= 0 &&
            SUCCEEDED(cam->Set(CameraControl_Focus, focus,
                               CameraControl_Flags_Manual))) any = true;
    }
    if (lock_wb) {
        ComPtr<IAMVideoProcAmp> amp;
        if (SUCCEEDED(source->QueryInterface(IID_PPV_ARGS(amp.put())))) {
            long v = 0, f = 0;
            if (SUCCEEDED(amp->Get(VideoProcAmp_WhiteBalance, &v, &f)))
                amp->Set(VideoProcAmp_WhiteBalance, v, VideoProcAmp_Flags_Manual);
        }
    }
    return any;
}

// ---- 공개 래퍼 ----
MediaFoundationCamera::MediaFoundationCamera() : impl_(new Impl) {}
MediaFoundationCamera::~MediaFoundationCamera() { delete impl_; }

bool MediaFoundationCamera::open(int device_index, int w, int h) {
    return impl_->open(device_index, w, h);
}
bool MediaFoundationCamera::next(cv::Mat& bgr_out) { return impl_->grab(bgr_out); }
bool MediaFoundationCamera::lock_auto_controls()   { return impl_->lock_auto_controls(); }
bool MediaFoundationCamera::set_manual(long e, long f, bool wb) {
    return impl_->set_manual(e, f, wb);
}
bool MediaFoundationCamera::is_open() const { return impl_->ok; }
int  MediaFoundationCamera::width()   const { return impl_->width; }
int  MediaFoundationCamera::height()  const { return impl_->height; }
std::string MediaFoundationCamera::last_error() const { return impl_->error; }

}  // namespace cfp

#endif  // _WIN32
