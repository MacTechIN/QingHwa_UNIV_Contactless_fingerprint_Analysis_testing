// =============================================================================
//  main_win32.cpp — 비접촉 지문인식 MVP · Windows 네이티브 앱 (dev_plan V0~V8)
//
//  [왜 순수 Win32 + GDI인가]
//  dev_plan은 WinUI 3를 목표로 잡았으나, WinUI 3는 Windows App SDK와 vcxproj가
//  필요해 CMake 단독 빌드가 어렵다. 이 앱의 목적은 "논문 구현을 실제로 돌려보고
//  단계별 결과를 눈으로 확인하는 것"이므로, 추가 SDK 없이 즉시 빌드되는
//  순수 Win32가 실질적으로 더 낫다. UI 미려함은 이 단계의 목표가 아니다.
//  (WinUI 3 셸은 V10+ 백로그로 남긴다)
//
//  화면 구성
//    ┌ 툴바 ────────────────────────────────────────────────┐
//    │ [카메라] [정지] [AE잠금] [사진열기] [실행] [보고서]    │
//    ├──────────────┬──────────────┬────────────────────────┤
//    │ 입력/프리뷰  │ 단계별 썸네일 │ 논문 대조 해설(스크롤) │
//    ├──────────────┴──────────────┴────────────────────────┤
//    │ 상태바: 품질지표 · 미뉴셔 수 · 소요시간               │
//    └───────────────────────────────────────────────────────┘
// =============================================================================
#ifdef _WIN32

#ifndef UNICODE
#  define UNICODE
#endif
#ifndef _UNICODE
#  define _UNICODE
#endif

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "cfp/capture.hpp"
#include "cfp/enhancer.hpp"
#include "cfp/matcher.hpp"
#include "cfp/minutiae.hpp"
#include "cfp/narrate_stages.hpp"
#include "cfp/pipeline.hpp"
#include "cfp/quality.hpp"
#include "cfp/report.hpp"
#include "cfp/segmenter.hpp"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shell32.lib")

using namespace cfp;

namespace {

// ---- 컨트롤 ID ----
enum : int {
    ID_BTN_CAM = 1001, ID_BTN_STOP, ID_BTN_LOCK, ID_BTN_OPEN,
    ID_BTN_RUN, ID_BTN_REPORT, ID_EDIT_LOG, ID_TIMER_PREVIEW = 1
};

constexpr int kToolbarH = 44;
constexpr int kStatusH  = 26;

// -----------------------------------------------------------------------------
//  UTF-8 → UTF-16
//  [핵심] 해설 문자열은 전부 UTF-8이지만 Win32 컨트롤은 UTF-16을 받는다.
//  변환을 빠뜨리면 한글이 깨진다. 앱 전체를 UNICODE 빌드로 고정하고
//  경계에서 한 번만 변환한다.
// -----------------------------------------------------------------------------
std::wstring widen(const std::string& s) {
    if (s.empty()) return {};
    const int n = ::MultiByteToWideChar(CP_UTF8, 0, s.c_str(),
                                        static_cast<int>(s.size()), nullptr, 0);
    std::wstring w(static_cast<std::size_t>(n), L'\0');
    ::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()),
                          w.data(), n);
    return w;
}

std::string narrow(const std::wstring& w) {
    if (w.empty()) return {};
    const int n = ::WideCharToMultiByte(CP_UTF8, 0, w.c_str(),
                                        static_cast<int>(w.size()),
                                        nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<std::size_t>(n), '\0');
    ::WideCharToMultiByte(CP_UTF8, 0, w.c_str(), static_cast<int>(w.size()),
                          s.data(), n, nullptr, nullptr);
    return s;
}

// -----------------------------------------------------------------------------
//  cv::Mat → GDI 출력
//
//  [핵심 로직 해설] BITMAPINFOHEADER의 biHeight를 **음수**로 준다.
//  DIB의 기본 규약은 bottom-up(첫 행이 영상 맨 아래)인데, cv::Mat은 top-down이다.
//  음수 높이는 "이 비트맵은 top-down"이라는 뜻이므로, 이렇게 하면 뒤집기 없이
//  그대로 그릴 수 있다. 이걸 모르면 화면이 상하 반전되어 나온다.
//
//  또 하나: DIB는 각 행이 4바이트 경계에 정렬되어야 한다. cv::Mat의 step이
//  그 조건을 만족하지 않을 수 있으므로 연속 메모리로 복사해 안전하게 만든다.
// -----------------------------------------------------------------------------
void draw_mat(HDC hdc, const cv::Mat& src, RECT rc) {
    const int rw = rc.right - rc.left, rh = rc.bottom - rc.top;
    if (rw <= 2 || rh <= 2) return;

    ::FillRect(hdc, &rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));
    if (src.empty()) return;

    cv::Mat bgr;
    if (src.channels() == 1)      cv::cvtColor(src, bgr, cv::COLOR_GRAY2BGR);
    else if (src.channels() == 4) cv::cvtColor(src, bgr, cv::COLOR_BGRA2BGR);
    else                          bgr = src;
    if (bgr.type() != CV_8UC3) bgr.convertTo(bgr, CV_8UC3);

    // 종횡비 유지 축소 (레터박스)
    const double s = std::min(static_cast<double>(rw) / bgr.cols,
                             static_cast<double>(rh) / bgr.rows);
    const int dw = std::max(1, static_cast<int>(bgr.cols * s));
    const int dh = std::max(1, static_cast<int>(bgr.rows * s));
    const int dx = rc.left + (rw - dw) / 2;
    const int dy = rc.top  + (rh - dh) / 2;

    cv::Mat cont;
    if (bgr.isContinuous() && (bgr.cols * 3) % 4 == 0) cont = bgr;
    else {
        // 4바이트 정렬을 만족하도록 폭을 패딩해 복사
        const int pad = (4 - (bgr.cols * 3) % 4) % 4;
        cont.create(bgr.rows, bgr.cols + (pad ? 1 : 0), CV_8UC3);
        cont.setTo(cv::Scalar::all(0));
        bgr.copyTo(cont(cv::Rect(0, 0, bgr.cols, bgr.rows)));
    }

    BITMAPINFO bi{};
    bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth       = cont.cols;
    bi.bmiHeader.biHeight      = -cont.rows;      // ← 음수 = top-down
    bi.bmiHeader.biPlanes      = 1;
    bi.bmiHeader.biBitCount    = 24;
    bi.bmiHeader.biCompression = BI_RGB;

    ::SetStretchBltMode(hdc, HALFTONE);
    ::SetBrushOrgEx(hdc, 0, 0, nullptr);
    ::StretchDIBits(hdc, dx, dy, dw, dh, 0, 0, bgr.cols, bgr.rows,
                    cont.data, &bi, DIB_RGB_COLORS, SRCCOPY);
}

void draw_label(HDC hdc, RECT rc, const wchar_t* text) {
    ::SetBkMode(hdc, TRANSPARENT);
    ::SetTextColor(hdc, RGB(255, 210, 90));
    RECT t = rc; t.left += 6; t.top += 4;
    ::DrawTextW(hdc, text, -1, &t, DT_LEFT | DT_TOP | DT_SINGLELINE);
}

// =============================================================================
//  앱 상태
// =============================================================================
struct App {
    HWND hwnd = nullptr, edit = nullptr, status = nullptr;
    HFONT font = nullptr, font_mono = nullptr;

    // 파이프라인 (DI 조립 — 절편 교체 지점)
    std::shared_ptr<IQualityGate>      quality   = std::make_shared<SimpleQualityGate>();
    std::shared_ptr<IFingerSegmenter>  segmenter = std::make_shared<YCbCrSegmenter>();
    std::shared_ptr<IRidgeEnhancer>    enhancer  = std::make_shared<GaborRidgeEnhancer>();
    std::shared_ptr<IMinutiaExtractor> extractor = std::make_shared<CrossingNumberExtractor>();
    std::unique_ptr<Pipeline>          pipe;

    std::unique_ptr<MediaFoundationCamera> cam;
    bool     preview_on = false;
    bool     ae_locked  = false;

    cv::Mat  input;                       // 현재 입력 (프리뷰 또는 로드 이미지)
    PipelineOutput out;                   // 마지막 실행 결과
    std::vector<StageNarration> narr;
    std::vector<std::pair<std::wstring, cv::Mat>> panels;
    std::wstring source_name = L"(입력 없음)";
    bool has_result = false;

    App() {
        pipe = std::make_unique<Pipeline>(quality, segmenter, enhancer, extractor);
    }

    void set_status(const std::wstring& s) {
        if (status) ::SetWindowTextW(status, s.c_str());
    }
    void log(const std::wstring& s) {
        if (!edit) return;
        const int len = ::GetWindowTextLengthW(edit);
        ::SendMessageW(edit, EM_SETSEL, len, len);
        ::SendMessageW(edit, EM_REPLACESEL, FALSE,
                       reinterpret_cast<LPARAM>(s.c_str()));
    }
    void clear_log() { if (edit) ::SetWindowTextW(edit, L""); }
};

App g;

// -----------------------------------------------------------------------------
//  파이프라인 1회 실행 + 논문 대조 해설 생성
// -----------------------------------------------------------------------------
void run_pipeline() {
    if (g.input.empty()) {
        ::MessageBoxW(g.hwnd, L"입력이 없습니다. [사진 열기] 또는 [카메라 시작] 후 실행하세요.",
                      L"안내", MB_OK | MB_ICONINFORMATION);
        return;
    }
    const auto t0 = std::chrono::steady_clock::now();

    // 품질 게이트를 강제하지 않는다 — 게이트에서 멈추면 이후 스택의
    // 논문 대조를 보여줄 수 없기 때문(검증 도구로서의 목적).
    g.out = g.pipe->run(g.input, /*enforce_quality_gate=*/false);

    const double cap_ms = std::chrono::duration<double, std::milli>(
        std::chrono::steady_clock::now() - t0).count();

    auto ms_of = [&](const char* s) {
        for (const auto& t : g.out.timings) if (t.stage == s) return t.ms;
        return 0.0;
    };

    int idx = 1;
    g.narr.clear();
    g.narr.push_back(narrate::capture(g.input, cap_ms, idx++));
    g.narr.push_back(narrate::quality(g.out.quality, ms_of("quality"), idx++));
    g.narr.push_back(narrate::segment(g.out.segment, ms_of("segment"), idx++));
    if (g.out.segment.ok) {
        const double e = ms_of("enhance") / 4.0;
        g.narr.push_back(narrate::normalize  (g.out.enhance, e, idx++));
        g.narr.push_back(narrate::orientation(g.out.enhance, e, idx++));
        g.narr.push_back(narrate::frequency  (g.out.enhance, e, idx++));
        g.narr.push_back(narrate::gabor      (g.out.enhance, e, idx++));
        g.narr.push_back(narrate::minutiae(g.out.minutiae, g.out.enhance,
                                           ms_of("minutiae"), idx++));
    }

    // 썸네일 패널
    g.panels.clear();
    auto add = [&](const wchar_t* c, const cv::Mat& m) {
        if (!m.empty()) g.panels.emplace_back(c, m.clone());
    };
    add(L"① ROI",        g.out.segment.roi_gray);
    add(L"② 정규화",     g.out.enhance.normalized);
    add(L"③ 유효영역",   g.out.enhance.mask);
    add(L"④ Gabor",      g.out.enhance.enhanced);
    add(L"⑤ 이진화",     g.out.enhance.binary);
    add(L"⑥ 세선화",     g.out.enhance.skeleton);
    if (!g.out.minutiae.empty() && !g.out.enhance.enhanced.empty()) {
        cv::Mat vis;
        cv::cvtColor(g.out.enhance.enhanced, vis, cv::COLOR_GRAY2BGR);
        for (const auto& m : g.out.minutiae) {
            const cv::Scalar c = (m.type == MinutiaType::RidgeEnding)
                                 ? cv::Scalar(80, 220, 90) : cv::Scalar(60, 165, 245);
            const cv::Point p(static_cast<int>(m.x), static_cast<int>(m.y));
            cv::circle(vis, p, 3, c, 1, cv::LINE_AA);
            cv::line(vis, p,
                     {static_cast<int>(m.x + 11 * std::cos(m.theta)),
                      static_cast<int>(m.y + 11 * std::sin(m.theta))}, c, 1, cv::LINE_AA);
        }
        add(L"⑦ 미뉴셔", vis);
    }

    // 해설 출력
    g.clear_log();
    for (const auto& n : g.narr) g.log(widen(render_text(n)));

    double total = 0.0;
    for (const auto& t : g.out.timings) total += t.ms;
    wchar_t buf[512];
    ::swprintf_s(buf, L"  선명도 %.1f · 밝기 %.0f · 대비 %.1f  |  미뉴셔 %zu개  |  총 %.0f ms  |  %s",
                 g.out.quality.sharpness, g.out.quality.brightness, g.out.quality.contrast,
                 g.out.minutiae.size(), total,
                 g.out.ok ? L"완료" : widen(g.out.failure_stage).c_str());
    g.set_status(buf);
    g.has_result = true;
    ::InvalidateRect(g.hwnd, nullptr, FALSE);
}

// -----------------------------------------------------------------------------
//  보고서 생성 (HTML + PDF) 후 기본 프로그램으로 열기
// -----------------------------------------------------------------------------
void make_report() {
    if (!g.has_result) {
        ::MessageBoxW(g.hwnd, L"먼저 [실행]으로 파이프라인을 돌리세요.",
                      L"안내", MB_OK | MB_ICONINFORMATION);
        return;
    }
    report::ReportInput ri;
    ri.subtitle    = "각 스택이 구현한 논문 내용과 이번 실행의 실측 결과 대조";
    ri.source_name = narrow(g.source_name);
    ri.narrations  = g.narr;
    ri.timings     = g.out.timings;
    for (const auto& p : g.panels)
        ri.panels.push_back({narrow(p.first), "", p.second});
    ri.summary = g.out.ok
        ? "Chowdhury & Imtiaz(2022) 리뷰가 정리한 고전 경로를 실행한 결과입니다."
        : ("파이프라인 중단: " + g.out.failure_stage);

    g.set_status(L"  보고서 생성 중... (PDF 변환에 수 초 걸릴 수 있습니다)");
    std::string html, pdf, why;
    const bool ok = report::save(ri, "out", html, pdf, why);

    ::ShellExecuteW(g.hwnd, L"open", widen(ok ? pdf : html).c_str(),
                    nullptr, nullptr, SW_SHOWNORMAL);
    g.set_status(ok ? L"  보고서 생성 완료 (out/report.html, out/report.pdf)"
                    : (L"  HTML만 생성됨 — " + widen(why)).c_str());
}

void open_image() {
    wchar_t path[MAX_PATH] = L"";
    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = g.hwnd;
    ofn.lpstrFilter = L"이미지\0*.png;*.jpg;*.jpeg;*.bmp;*.tif\0모든 파일\0*.*\0";
    ofn.lpstrFile   = path;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (!::GetOpenFileNameW(&ofn)) return;

    g.input = cv::imread(narrow(path), cv::IMREAD_COLOR);
    if (g.input.empty()) {
        ::MessageBoxW(g.hwnd, L"이미지를 열 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
        return;
    }
    g.source_name = path;
    g.has_result = false;
    g.panels.clear();
    g.clear_log();
    g.set_status(L"  이미지 로드됨 — [실행]을 누르세요");
    ::InvalidateRect(g.hwnd, nullptr, FALSE);
}

void start_camera() {
    g.cam = std::make_unique<MediaFoundationCamera>();
    if (!g.cam->open(0, 1280, 720)) {
        ::MessageBoxW(g.hwnd, widen("카메라를 열 수 없습니다: " + g.cam->last_error()).c_str(),
                      L"오류", MB_OK | MB_ICONERROR);
        g.cam.reset();
        return;
    }
    g.preview_on = true;
    g.source_name = L"카메라";
    ::SetTimer(g.hwnd, ID_TIMER_PREVIEW, 33, nullptr);   // 약 30fps
    wchar_t b[128];
    ::swprintf_s(b, L"  카메라 %dx%d 프리뷰 중", g.cam->width(), g.cam->height());
    g.set_status(b);
}

void stop_camera() {
    ::KillTimer(g.hwnd, ID_TIMER_PREVIEW);
    g.preview_on = false;
    g.cam.reset();
    g.set_status(L"  카메라 정지");
}

void lock_ae() {
    if (!g.cam) {
        ::MessageBoxW(g.hwnd, L"먼저 카메라를 시작하세요.", L"안내", MB_OK | MB_ICONINFORMATION);
        return;
    }
    // research.md §2.1: 자동 노출이 켜져 있으면 조도 실험이 성립하지 않는다.
    g.ae_locked = g.cam->lock_auto_controls();
    ::MessageBoxW(g.hwnd,
        g.ae_locked ? L"노출/초점/화이트밸런스를 수동 고정했습니다."
                    : L"이 카메라는 수동 제어를 지원하지 않습니다.\n"
                      L"조도·거리 실험 결과의 재현성이 떨어질 수 있습니다.\n"
                      L"(dev_plan 리스크 R2 — 수동제어 지원 UVC 웹캠 권장)",
        g.ae_locked ? L"완료" : L"경고",
        MB_OK | (g.ae_locked ? MB_ICONINFORMATION : MB_ICONWARNING));
}

// =============================================================================
//  윈도우 프로시저
// =============================================================================
LRESULT CALLBACK WndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        g.hwnd = h;
        g.font = ::CreateFontW(-15, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
                               0, 0, CLEARTYPE_QUALITY, 0, L"Malgun Gothic");
        g.font_mono = ::CreateFontW(-13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
                                    0, 0, CLEARTYPE_QUALITY, FIXED_PITCH, L"D2Coding");

        struct { int id; const wchar_t* t; int x, w; } btns[] = {
            {ID_BTN_CAM,    L"카메라 시작",   8, 100},
            {ID_BTN_STOP,   L"정지",        112,  60},
            {ID_BTN_LOCK,   L"AE/AF 잠금",  176, 100},
            {ID_BTN_OPEN,   L"사진 열기",   284,  90},
            {ID_BTN_RUN,    L"▶ 실행",      378,  80},
            {ID_BTN_REPORT, L"보고서(HTML/PDF)", 462, 150},
        };
        for (const auto& b : btns) {
            HWND c = ::CreateWindowW(L"BUTTON", b.t,
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                b.x, 8, b.w, 28, h, reinterpret_cast<HMENU>(
                    static_cast<INT_PTR>(b.id)), nullptr, nullptr);
            ::SendMessageW(c, WM_SETFONT, reinterpret_cast<WPARAM>(g.font), TRUE);
        }

        g.edit = ::CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY |
            ES_AUTOVSCROLL, 0, 0, 10, 10, h,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_EDIT_LOG)), nullptr, nullptr);
        ::SendMessageW(g.edit, WM_SETFONT, reinterpret_cast<WPARAM>(g.font_mono), TRUE);

        g.status = ::CreateWindowW(L"STATIC",
            L"  준비됨 — [사진 열기] 또는 [카메라 시작]",
            WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP | SS_CENTERIMAGE,
            0, 0, 10, 10, h, nullptr, nullptr, nullptr);
        ::SendMessageW(g.status, WM_SETFONT, reinterpret_cast<WPARAM>(g.font), TRUE);
        return 0;
    }

    case WM_SIZE: {
        RECT rc; ::GetClientRect(h, &rc);
        const int W = rc.right, H = rc.bottom;
        const int logw = std::max(340, W * 38 / 100);
        ::MoveWindow(g.edit, W - logw, kToolbarH, logw,
                     H - kToolbarH - kStatusH, TRUE);
        ::MoveWindow(g.status, 0, H - kStatusH, W, kStatusH, TRUE);
        ::InvalidateRect(h, nullptr, TRUE);
        return 0;
    }

    case WM_TIMER:
        if (wp == ID_TIMER_PREVIEW && g.cam) {
            cv::Mat f;
            if (g.cam->next(f) && !f.empty()) {
                g.input = f;
                ::InvalidateRect(h, nullptr, FALSE);
            }
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BTN_CAM:    start_camera(); break;
        case ID_BTN_STOP:   stop_camera();  break;
        case ID_BTN_LOCK:   lock_ae();      break;
        case ID_BTN_OPEN:   stop_camera(); open_image(); break;
        case ID_BTN_RUN:    run_pipeline(); break;
        case ID_BTN_REPORT: make_report();  break;
        }
        return 0;

    case WM_ERASEBKGND:
        return 1;                            // WM_PAINT에서 전부 칠하므로 깜빡임 방지

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(h, &ps);
        RECT rc; ::GetClientRect(h, &rc);
        const int W = rc.right, H = rc.bottom;
        const int logw = std::max(340, W * 38 / 100);
        const int area_w = W - logw;
        const int top = kToolbarH, bot = H - kStatusH;

        // 더블 버퍼링 — 프리뷰 30fps에서 깜빡임을 없앤다.
        HDC mem = ::CreateCompatibleDC(hdc);
        HBITMAP bmp = ::CreateCompatibleBitmap(hdc, W, H);
        HGDIOBJ old = ::SelectObject(mem, bmp);

        RECT full{0, 0, W, H};
        ::FillRect(mem, &full, (HBRUSH)(COLOR_BTNFACE + 1));

        // 좌: 입력/프리뷰
        const int prev_w = area_w * 46 / 100;
        RECT rprev{6, top, 6 + prev_w, bot - 6};
        draw_mat(mem, g.input, rprev);
        draw_label(mem, rprev, g.preview_on ? L"프리뷰 (카메라)" : L"입력");

        // 우: 단계별 썸네일 격자
        const int gx = rprev.right + 8;
        const int gw = area_w - prev_w - 20;
        if (!g.panels.empty() && gw > 60) {
            const int cols = 2;
            const int rows = static_cast<int>((g.panels.size() + cols - 1) / cols);
            const int cw = gw / cols;
            const int ch = (bot - top - 6) / std::max(1, rows);
            for (std::size_t i = 0; i < g.panels.size(); ++i) {
                const int c = static_cast<int>(i) % cols;
                const int r = static_cast<int>(i) / cols;
                RECT cell{gx + c * cw + 2, top + r * ch + 2,
                          gx + (c + 1) * cw - 2, top + (r + 1) * ch - 2};
                draw_mat(mem, g.panels[i].second, cell);
                draw_label(mem, cell, g.panels[i].first.c_str());
            }
        } else {
            RECT r{gx, top, gx + gw, bot - 6};
            ::FillRect(mem, &r, (HBRUSH)::GetStockObject(BLACK_BRUSH));
            draw_label(mem, r, L"[실행]을 누르면 단계별 결과가 여기 표시됩니다");
        }

        ::BitBlt(hdc, 0, 0, W, H, mem, 0, 0, SRCCOPY);
        ::SelectObject(mem, old);
        ::DeleteObject(bmp);
        ::DeleteDC(mem);
        ::EndPaint(h, &ps);
        return 0;
    }

    case WM_DESTROY:
        stop_camera();
        if (g.font)      ::DeleteObject(g.font);
        if (g.font_mono) ::DeleteObject(g.font_mono);
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(h, msg, wp, lp);
}


// -----------------------------------------------------------------------------
//  DPI 인식 설정
//
//  [핵심 로직 해설] SetProcessDpiAwarenessContext는 Windows 10 1703부터 존재한다.
//  링크 시점에 걸어 두면 그보다 낮은 SDK로 빌드할 때 컴파일이 깨지고,
//  더 낮은 OS에서 실행하면 로더가 DLL 임포트를 못 찾아 프로세스가 시작조차 못한다.
//  런타임에 GetProcAddress로 찾아 쓰면 두 문제가 동시에 사라진다.
//  (매니페스트의 PerMonitorV2 설정이 1차 경로이고, 이건 보강이다)
// -----------------------------------------------------------------------------
void enable_dpi_awareness() {
    using Fn = BOOL(WINAPI*)(HANDLE);
    if (HMODULE u = ::GetModuleHandleW(L"user32.dll")) {
        if (auto fn = reinterpret_cast<Fn>(reinterpret_cast<void*>(
                ::GetProcAddress(u, "SetProcessDpiAwarenessContext")))) {
            // DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 == (HANDLE)-4
            if (fn(reinterpret_cast<HANDLE>(static_cast<INT_PTR>(-4)))) return;
        }
    }
    // 폴백: Vista~8.1 경로
    using Fn2 = BOOL(WINAPI*)(void);
    if (HMODULE u = ::GetModuleHandleW(L"user32.dll")) {
        if (auto fn2 = reinterpret_cast<Fn2>(reinterpret_cast<void*>(
                ::GetProcAddress(u, "SetProcessDPIAware")))) {
            fn2();
        }
    }
}

}  // namespace

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int nShow) {
    enable_dpi_awareness();
    INITCOMMONCONTROLSEX icc{sizeof(icc), ICC_STANDARD_CLASSES};
    ::InitCommonControlsEx(&icc);

    WNDCLASSW wc{};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = L"CfpMainWindow";
    if (!::RegisterClassW(&wc)) return 1;

    HWND h = ::CreateWindowExW(0, wc.lpszClassName,
        L"비접촉 지문인식 MVP — 논문 구현 검증 하네스",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1500, 900,
        nullptr, nullptr, hInst, nullptr);
    if (!h) return 1;

    ::ShowWindow(h, nShow);
    ::UpdateWindow(h);

    MSG msg;
    while (::GetMessageW(&msg, nullptr, 0, 0) > 0) {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }
    return 0;
}

#endif  // _WIN32
