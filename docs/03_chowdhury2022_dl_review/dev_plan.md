# MVP 개발 계획서 — 비접촉 지문인식 Windows 네이티브 테스트 앱

> **근거 문서**: [`research.md`](./research.md) (Chowdhury & Imtiaz, *JCP* 2022, 2(3):714–730 기반)
> **문서 성격**: 구현 계획서 (Vertical-Stackable / Micro-process step)
> **대상**: Windows 10 2004(19041)+ / Windows 11 · x64 · 데스크톱 네이티브
> **작성 기준일**: 2026-09-09

---

## 0. 이 계획서의 구조 원칙

### 0.1 Vertical Stackable — 수직 절편을 쌓는다

각 절편(V0~V9)은 **UI → 획득 → 처리 → 결과 → 저장** 의 전 계층을 관통하는 **독자 실행 가능한 앱**이다.
수평 레이어(예: "전처리 모듈 전부 먼저 완성")로 나누지 않는다.

```
        ┌───────────────────────────────────────────────┐
  V9    │ 패키징·보안              (MSIX / DPAPI)        │
  V8    │ 평가 하니스              (EER / DET / CMC)     │
  V7    │ 등록·검증 플로우         (SQLite 템플릿 DB)     │
  V6    │ 미뉴셔 추출              (CN → MinuNet ONNX)   │
  V5    │ 딥 세그멘테이션          (U-Net ONNX/DirectML) │
  V4    │ 융선 향상                (CLAHE/Gabor/Thin)    │
  V3    │ 손가락 ROI               (YCbCr 고전)          │
  V2    │ 품질 게이트              (Laplacian/밝기/대비)  │
  V1    │ 카메라 획득              (MediaCapture)        │
  V0    │ Walking Skeleton         (셸·DI·로깅·파이프라인)│
        └───────────────────────────────────────────────┘
             ↑ 각 절편은 아래 절편 위에 얹히며, 항상 실행 가능
```

**불변식(Invariant)**: 어느 절편이 끝난 시점에도 앱은 **빌드·실행·데모**가 가능하다.
절편이 깨지면 다음 절편으로 넘어가지 않는다.

### 0.2 Micro-process step — 절편을 미세 단계로 쪼갠다

각 절편은 **µ-step** 단위로 분해한다. µ-step 1개의 규격:

| 속성 | 규격 |
|---|---|
| 소요 | **0.5 ~ 3 시간** (넘으면 더 쪼갠다) |
| 산출 | 커밋 1개 = 빌드 성공 = 실행 가능 |
| 검증 | **DoD(Definition of Done)** 를 사람이 눈으로 또는 테스트로 확인 |
| 롤백 | 단일 커밋 revert로 복구 가능 |
| 의존 | 직전 µ-step에만 의존 (병렬 가능 시 명시) |

표기: `V3-µ2` = 절편 3의 두 번째 마이크로 스텝.

---

## 1. 기술 스택 결정

### 1.1 확정 스택

| 계층 | 선택 | 근거 |
|---|---|---|
| **언어/표준** | **C++20** | 사용자 지정. OpenCV의 **네이티브 언어**이므로 바인딩 계층·버전 지연·성능 손실이 없다 |
| **빌드** | **CMake 3.20+ / vcpkg** (`vcpkg.json` 매니페스트) | Windows·Linux 동일 빌드. CI 재현성 |
| **UI** | **WinUI 3 (C++/WinRT)** — 셸은 별도 vcxproj | 현행 Microsoft 네이티브 UI. Core는 UI 무관하게 분리 |
| **카메라** | **Media Foundation** (`IMFSourceReader`, COM) | C++ 네이티브. 수동 노출·초점·화이트밸런스 제어 ← research.md §2.1 필수 요건 |
| **영상처리** | **OpenCV 4.10 + contrib** (`ximgproc::thinning`) | research.md §3의 CLAHE/Gabor/Zhang-Suen을 전 API 커버리지로 사용 |
| **딥러닝 추론** | **ONNX Runtime C++ API / DirectML EP** | GPU 벤더 무관, CPU 폴백. `cv::Mat` → 텐서가 같은 프로세스 내 직결 |
| **모델 학습** | **Python(PyTorch) — 앱 외부, 오프라인** → ONNX export | 앱은 추론 전용 |
| **DB** | **SQLite (C API)** + SQLCipher(V9) | 파일 1개, 의존성 최소 |
| **보고서** | 자체 HTML 생성 + **WebView2 / Edge 인쇄 → PDF** | 화면과 인쇄가 어긋나지 않도록 HTML을 단일 진실원으로 |
| **테스트** | 자체 경량 하네스(`tests/test_util.hpp`) | 외부 의존 0으로 CI를 가볍게 유지 |
| **패키징** | **MSIX** | 카메라 권한(`webcam` capability)이 매니페스트로 해결 |

> **구현 현황(2026-09-09)**: Core 라이브러리·영상 파이프라인·매칭·평가·보고서·CLI·테스트 완료.
> 상세 검증 결과와 발견된 결함은 [`code_review.md`](./code_review.md) 참조.

### 1.2 선택하지 않은 대안과 이유

| 대안 | 배제 이유 |
|---|---|
| C# / OpenCvSharp | 래핑 계층(P/Invoke)이 끼고 contrib 커버리지가 약 90 %. `ximgproc::thinning` 등 일부 모듈 접근이 제한적 |
| Python + PySide6 | 개발은 빠르나 "Windows 네이티브"가 아니고 배포물 300 MB+/기동 5 s+ |
| Rust (opencv-rs) | 바인딩 불안정, 커버리지 ~80 % |
| Java / JS(opencv.js) | 커버리지 70 % / 30 %. 데스크톱 네이티브에 부적합 |
| CUDA EP | NVIDIA 종속. 테스트 PC 다양성 대응 불가 → DirectML |
| PDF 직접 생성 라이브러리(libharu 등) | 화면(HTML)과 인쇄(PDF)를 따로 그리면 반드시 어긋나고, 한글 폰트 임베딩·서브셋팅 비용이 크다 → 브라우저 엔진 인쇄 채택 |

> **폴백 레인**: V5/V6의 ONNX 모델 학습이 지연되면 앱은 **고전 알고리즘 경로(V3/V4 + Crossing Number)만으로 완전 동작**한다.
> 딥 경로는 DI 스위치로 붙는 **선택 구현체**이므로, 모델 부재가 일정 전체를 막지 않는다. ← 이것이 수직 절편 설계의 목적.

### 1.3 교체 가능성을 보장하는 핵심 인터페이스

절편이 "쌓이려면" 계약이 먼저 있어야 한다. V0에서 아래를 정의하고 이후 절편은 **구현체만 추가**한다.

```cpp
// include/cfp/abstractions.hpp — V0에서 정의, 이후 "추가"만 하고 "변경"하지 않는다
class IFrameSource {          // V1: Media Foundation / V8: 파일 폴더
public:
    virtual ~IFrameSource() = default;
    virtual bool next(cv::Mat& bgr_out) = 0;
};

class IQualityGate {          // V2: Laplacian 분산 / (후) NFIQ2
public:
    virtual QualityReport evaluate(const cv::Mat& bgr) const = 0;
};

class IFingerSegmenter {      // V3: YCbCr 고전 / V5: U-Net ONNX
public:
    virtual SegmentResult segment(const cv::Mat& bgr) const = 0;
};

class IRidgeEnhancer {        // V4: CLAHE + 방향장 + Gabor
public:
    virtual EnhanceResult enhance(const cv::Mat& roi_gray,
                                  const cv::Mat& roi_mask = cv::Mat()) const = 0;
};

class IMinutiaExtractor {     // V6a: Crossing Number / V6b: MinuNet ONNX
public:
    virtual std::vector<Minutia> extract(const EnhanceResult& enh) const = 0;
};
```

**파이프라인 조립**은 한 곳(생성자 주입)에서만 일어난다:
```cpp
Pipeline pipe(std::make_shared<SimpleQualityGate>(),
              cfg.use_deep_seg ? std::static_pointer_cast<IFingerSegmenter>(
                                     std::make_shared<OnnxUNetSegmenter>(model))
                               : std::make_shared<YCbCrSegmenter>(),
              std::make_shared<GaborRidgeEnhancer>(),
              std::make_shared<CrossingNumberExtractor>());
```

---

## 2. 솔루션 구조 (실제 구현됨)

```
ContactlessFp/
├── CMakeLists.txt · vcpkg.json
├── include/cfp/
│   ├── types.hpp          # 값 타입 (OpenCV 비의존)
│   ├── abstractions.hpp   # 확장 지점 인터페이스 6종
│   ├── quality.hpp / segmenter.hpp / enhancer.hpp / minutiae.hpp
│   ├── matcher.hpp        # 미뉴셔 정합 (OpenCV 비의존)
│   ├── metrics.hpp        # EER/DET/CMC/AUC (OpenCV 비의존)
│   ├── narration.hpp      # 스택↔논문 대응 (OpenCV 비의존)
│   ├── narrate_stages.hpp # 단계별 실행 해설
│   ├── report.hpp         # HTML 시각화 + PDF 내보내기
│   └── pipeline.hpp
├── src/                   # 위 헤더의 구현 12개
├── app/                   # WinUI 3 셸 (Windows 전용)
├── tools/cfp_cli.cpp      # 플랫폼 독립 실행기
├── tests/
│   ├── test_util.hpp      # 경량 테스트 하네스
│   ├── synthetic.hpp      # 합성 지문 생성기 (위상 특이점 주입)
│   ├── test_metrics.cpp · test_matcher.cpp · test_pipeline.cpp
├── assets/real/           # 검증용 실촬영 샘플 (논문 도판 추출본)
└── models/                # *.onnx (Git LFS)
```

**타깃 분리 원칙**
- `cfp_core` — OpenCV **비의존**. 매칭·평가·해설. 데이터셋도 카메라도 없이 CI에서 검증 가능.
- `cfp_imaging` — 영상 파이프라인(OpenCV 의존).
- `cfp_capture` — Media Foundation (Windows 전용).

---

## 3. 수직 절편 상세

---

### V0 — Walking Skeleton (앱 골격)

> **목표**: 정지 이미지 1장이 파이프라인을 통과해 화면에 나오는 최소 앱.
> **데모 문장**: "이미지를 열면 원본과 그레이스케일이 나란히 보인다."

| µ-step | 작업 | DoD |
|---|---|---|
| V0-µ1 | 솔루션·프로젝트 6개 생성, .NET 8 / WinUI 3 템플릿, x64 빌드 확인 | `dotnet build` 성공, 빈 창 뜸 |
| V0-µ2 | ShellPage 3-pane 레이아웃(좌 소스 / 중 뷰어 / 우 속성), NavigationView | 창 리사이즈·다크모드 정상 |
| V0-µ3 | Generic Host + DI + Serilog(파일 `%LOCALAPPDATA%\ContactlessFp\logs`) + `appsettings.json` | 앱 기동 로그가 파일에 남음 |
| V0-µ4 | **`Core/Abstractions.cs` 인터페이스 전체 정의** (§1.3) | 컴파일 통과, 구현체 0개 |
| V0-µ5 | `IFrameSource`의 `FileFrameSource` 구현 (파일 열기 → `Mat`) | 파일 다이얼로그로 PNG/BMP 로드 |
| V0-µ6 | `Mat` ↔ WinUI `Image` 브리지 (`Mat` → `WriteableBitmap`) 유틸 + 메모리 해제 검증 | 100장 연속 로드 시 메모리 누수 없음(작업관리자 확인) |
| V0-µ7 | `PipelineRunner`: 등록된 스텝을 순차 실행하고 **각 단계 중간 결과를 보관**하는 오케스트레이터 | 스텝 0개일 때도 무해하게 통과 |
| V0-µ8 | PipelinePage: 단계별 썸네일 스트립 UI (단계명 + 소요 ms) | 그레이스케일 1스텝이 썸네일로 표시 |

**절편 완료 기준**: 이미지 로드 → 그레이 변환 → 썸네일 스트립 표시 + 로그 기록.

---

### V1 — 카메라 획득 (research.md §2.1)

> **데모 문장**: "웹캠 프리뷰가 흐르고, 노출을 고정한 뒤 스틸을 찍어 저장한다."

| µ-step | 작업 | DoD |
|---|---|---|
| V1-µ1 | `Package.appxmanifest`에 `webcam` capability 추가, MSIX 디버그 실행 전환 | 권한 프롬프트 정상 표시 |
| V1-µ2 | `MediaFrameSourceGroup` 열거 → 카메라 선택 콤보박스 | 연결된 카메라 전체 목록화 |
| V1-µ3 | `MediaCapture` 초기화 + `MediaPlayerElement` 프리뷰 | 30fps 프리뷰 |
| V1-µ4 | 해상도/포맷 선택 (`VideoEncodingProperties` 열거, 최대 해상도 우선) | 1080p 이상 선택 가능 |
| V1-µ5 | **수동 제어 패널**: 노출(`VideoDeviceController.ExposureControl`), ISO, 초점(`FocusControl`, Macro 모드), 화이트밸런스 잠금 | 슬라이더 조작이 프리뷰에 즉시 반영 |
| V1-µ6 | **`LockAeAwbAf()` 원클릭 잠금 버튼** — research.md §2.1 "수동 고정 필수" 대응 | 잠금 후 조명 바꿔도 프리뷰 밝기 불변 |
| V1-µ7 | `MediaFrameReader` → `SoftwareBitmap` → `Mat` 변환 (BGRA8) | 프레임이 `Mat`으로 파이프라인 진입 |
| V1-µ8 | `CameraFrameSource : IFrameSource` 구현 + DI 등록(파일/카메라 토글) | 소스 전환이 UI 토글로 동작 |
| V1-µ9 | 스틸 캡처 → `captures/yyyyMMdd_HHmmssfff.png` + 사이드카 `.json`(노출/ISO/초점/해상도) | 저장물에 메타가 함께 남음 |

**리스크**: 노트북 내장캠은 `ExposureControl.Supported == false`인 경우가 흔함 → **미지원 시 UI에 명시적 경고 배지**를 띄우고 계속 진행(V1-µ5 DoD에 포함).

---

### V2 — 품질 게이트 (research.md §3 품질, §0.5 데이터셋 품질)

> **데모 문장**: "흐릿하게 찍으면 빨간 배지가 뜨고 캡처 버튼이 잠긴다."

| µ-step | 작업 | DoD |
|---|---|---|
| V2-µ1 | `LaplacianVariance(Mat)` 선명도 지표 | 초점 흐림 시 값이 단조 감소 |
| V2-µ2 | 밝기(mean)·대비(std)·클리핑률(0/255 픽셀 비율) 계산 | 4개 지표 실시간 산출 |
| V2-µ3 | `QualityReport` 레코드 + `SimpleQualityGate : IQualityGate` (임계값 `appsettings.json`) | 설정 변경이 재빌드 없이 반영 |
| V2-µ4 | 프리뷰 오버레이 HUD: 지표 4개 실시간 + Pass/Warn/Fail 색상 배지 | 60fps UI 유지(계산은 3프레임마다 샘플링) |
| V2-µ5 | **캡처 게이팅**: Fail 상태에서 캡처 버튼 비활성 + 사유 툴팁 | 사용자에게 재촬영 유도가 동작 |
| V2-µ6 | 지표 시계열 스파크라인(최근 100프레임) | 손 움직임 시 값 변동이 시각화 |

> **확장 지점**: NFIQ 2.0은 네이티브 C++ 라이브러리라 P/Invoke 래핑 비용이 있다. MVP에서는 자체 4지표로 대체하고, V8 평가에서 **NFIQ2 CLI를 오프라인 배치로만** 병행 검증한다.

---

### V3 — 손가락 ROI (고전) (research.md §3 세그멘테이션)

> **데모 문장**: "손가락 끝이 초록 박스로 잡히고, 회전 정렬된 ROI가 오른쪽에 뜬다."

| µ-step | 작업 | DoD |
|---|---|---|
| V3-µ1 | BGR → YCrCb 변환 + 피부색 임계 마스크 (`Cr∈[133,173], Cb∈[77,127]` 초기값, 설정화) | 손 영역이 대략 분리 |
| V3-µ2 | 모폴로지 정리(open/close) + 최대 연결성분 선택 | 배경 잡음 제거 |
| V3-µ3 | 컨투어 → `MinAreaRect` → 주축 각도 산출 | 손가락 기울기 각도 출력 |
| V3-µ4 | **지두(fingertip) ROI 결정**: 주축 방향 상단 40 % 구간 크롭 | 손가락 끝 영역만 크롭 |
| V3-µ5 | 회전 정규화(`WarpAffine`) → 고정 크기(예: 320×480) 리샘플 | 기울여 찍어도 세로 정렬 출력 |
| V3-µ6 | `YCbCrSegmenter : IFingerSegmenter` 구현 + DI 등록 | 파이프라인에 세그 단계 추가됨 |
| V3-µ7 | 오버레이 렌더: 마스크 반투명 + ROI 박스 + 주축 화살표 | 시각 디버깅 가능 |
| V3-µ8 | 실패 케이스 처리(마스크 면적 < 임계 → `SegmentResult.Failed`) 및 UI 표시 | 손 없을 때 예외 대신 정상 실패 |

---

### V4 — 융선 향상 (research.md §3 향상)

> **데모 문장**: "ROI가 CLAHE → Gabor → 이진화 → 세선화 4단계로 변해가는 게 썸네일로 보인다."

| µ-step | 작업 | DoD |
|---|---|---|
| V4-µ1 | 그레이 변환 + mean/variance 정규화 | 조도 편차 완화 확인 |
| V4-µ2 | **CLAHE** (`Cv2.CreateCLAHE(clip, tile)`), 파라미터 슬라이더 | 실시간 파라미터 튜닝 |
| V4-µ3 | **방향장 추정** — Sobel 구조텐서 → 블록별 방향(16×16) | 방향장 화살표 오버레이 |
| V4-µ4 | **융선 주파수 추정** — x-signature 블록별 | 주파수 맵 히트맵 표시 |
| V4-µ5 | **Gabor 필터뱅크** 적용 (방향·주파수 적응, 8~16 방향) | 융선 대비 육안 개선 |
| V4-µ6 | 적응형 이진화(`AdaptiveThreshold`) | 이진 융선맵 생성 |
| V4-µ7 | **세선화** (`Cv2.XImgProc.Thinning`, Zhang-Suen) | 1픽셀 스켈레톤 |
| V4-µ8 | `GaborRidgeEnhancer : IRidgeEnhancer` 구현 + 각 중간단계 썸네일 노출 | 4단계 전부 스트립에 표시 |
| V4-µ9 | **스냅샷 회귀 테스트**: 고정 입력 3장 → 출력 해시/SSIM 고정 | CI에서 향상 로직 변경 감지 |

**성능 예산**: ROI 320×480 기준 V4 전체 **≤ 120 ms** (CPU). 초과 시 Gabor 방향 수를 8로 축소.

---

### V5 — 딥 세그멘테이션 (ONNX) (research.md §4.2)

> **데모 문장**: "토글을 켜면 YCbCr 대신 U-Net이 돌고, 마스크 경계가 눈에 띄게 깨끗해진다."

| µ-step | 작업 | DoD |
|---|---|---|
| V5-µ1 | `tools/export_onnx/`: PyTorch U-Net(또는 `segmentation_models_pytorch`) → ONNX opset 17 export 스크립트 | `unet_finger.onnx` 생성 |
| V5-µ2 | ONNX Runtime 패키지 추가 + **DirectML EP** 세션 생성, 실패 시 CPU EP 폴백 | 기동 로그에 사용 EP 기록 |
| V5-µ3 | `OnnxSessionFactory` — 세션 싱글톤·워밍업(더미 1회 추론) | 첫 추론 지연이 워밍업으로 흡수 |
| V5-µ4 | 전처리(리사이즈·정규화·NCHW 텐서화) / 후처리(시그모이드·임계·원본 좌표 복원) | 마스크가 원본 좌표계로 복귀 |
| V5-µ5 | `OnnxUNetSegmenter : IFingerSegmenter` 구현 | V3와 **동일 인터페이스**로 교체 가능 |
| V5-µ6 | 설정 토글 + **A/B 나란히 보기 뷰**(좌: 고전, 우: 딥) | 한 화면에서 두 결과 비교 |
| V5-µ7 | 추론 지연 계측(ms) HUD 표시, EP별 벤치마크 기록 | DirectML vs CPU 수치 확보 |
| V5-µ8 | 모델 파일 부재/손상 시 자동으로 V3 고전 경로 폴백 + 경고 | 모델 없이도 앱 정상 동작 |

> **µ-step 병렬화**: V5-µ1(Python, 학습/변환)은 V4 진행과 **병렬 가능**. 앱 측 V5-µ2~µ8만 순차.

---

### V6 — 미뉴셔 추출 (research.md §4.3)

> **데모 문장**: "융선 스켈레톤 위에 미뉴셔 점과 방향 화살표가 찍힌다."

#### V6a — 고전 (Crossing Number)

| µ-step | 작업 | DoD |
|---|---|---|
| V6a-µ1 | 3×3 이웃 Crossing Number 계산 → 끝점(CN=1)/분기점(CN=3) 검출 | 후보 미뉴셔 리스트 생성 |
| V6a-µ2 | 방향 산출(스켈레톤 추적 N픽셀 기울기) | `ThetaRad` 채워짐 |
| V6a-µ3 | 위양성 제거: ROI 경계 마진, 짧은 가지, 근접 쌍(거리 < d), 스퍼/브릿지 | 미뉴셔 수가 합리적 범위(30~80)로 수렴 |
| V6a-µ4 | `CrossingNumberExtractor : IMinutiaExtractor` + 오버레이 렌더(점+화살표, 타입별 색) | 시각 확인 가능 |
| V6a-µ5 | **ISO/IEC 19794-2 호환 직렬화**(간이) — x,y,θ,type,quality | 템플릿 바이트 배열 생성 |

#### V6b — 딥 (ContactlessMinuNet ONNX)

| µ-step | 작업 | DoD |
|---|---|---|
| V6b-µ1 | Python: hourglass 인코더 + **2 브랜치**(위치 `Conv1×1→BN→Sigmoid`, 방향 `Conv1×1→BN→Tanh` **sin/cos 2채널**) 구현 | research.md §4.3 구조와 일치 |
| V6b-µ2 | 손실 = Focal(히트맵) + `1 − cos(θ_pred − θ_gt)` — **각도 순환성 대응** | 학습 발산 없음 |
| V6b-µ3 | NBIS `mindtct` 자동 라벨 + 사람 검수로 학습셋 구축 | 라벨 셋 확보 |
| V6b-µ4 | ONNX export (동적 배치 축 고정) | `minunet.onnx` 생성 |
| V6b-µ5 | C#: 히트맵 → **NMS 피크 추출** → 좌표 복원, `atan2(sin,cos)`로 각도 복원 | 미뉴셔 리스트 산출 |
| V6b-µ6 | `OnnxMinuNetExtractor : IMinutiaExtractor` + A/B 토글 | 고전 대비 비교 가능 |
| V6b-µ7 | 검출 지표(Precision/Recall/F1, 위치 오차 px, 각도 오차 °) 계산 뷰 | research.md §6 지표 산출 |

> **일정 리스크가 가장 큰 절편.** V6b가 지연되면 **V6a만으로 V7~V8을 완주**한다(폴백 레인).

---

### V7 — 등록·검증 플로우 (research.md §4.4 매칭)

> **데모 문장**: "사용자 A를 3회 등록하고, 다시 찍으면 '일치 0.87'이 뜬다."

| µ-step | 작업 | DoD |
|---|---|---|
| V7-µ1 | SQLite 스키마: `subjects`, `templates`(BLOB, 메타), `attempts`(감사 로그) + EF-less `Microsoft.Data.Sqlite` | DB 파일 생성·마이그레이션 |
| V7-µ2 | `ITemplateStore` 구현 (등록/조회/삭제/열거) | CRUD 단위 테스트 통과 |
| V7-µ3 | **등록 화면**: 사용자ID 입력 → N회 캡처(품질 게이트 통과분만) → 템플릿 N개 저장 | 3장 등록 성공 |
| V7-µ4 | **미뉴셔 매칭기**: 지역 구조 기반 대응점 탐색 + RANSAC 유사변환 → 정합 미뉴셔 비율 스코어 | `ITemplateMatcher` 구현 |
| V7-µ5 | **1:1 검증 화면**: 캡처 → 스코어 → 임계값 대비 Accept/Reject + 대응점 시각화 | 동일인 고스코어/타인 저스코어 확인 |
| V7-µ6 | **1:N 식별 화면**: 전체 갤러리 스캔 → 상위 5위 랭킹 표시 | research.md §0.5 "identification 집중" 대응 |
| V7-µ7 | 임계값 슬라이더 + 실시간 판정 변화 | 운영점 감각 확보 |
| V7-µ8 | 시도 로그(스코어·소요·판정) 기록 → V8 입력으로 사용 | `attempts` 테이블 적재 |

---

### V8 — 평가 하니스 (research.md §6)

> **데모 문장**: "PolyU 폴더를 지정하면 전수 비교가 돌고, DET 곡선과 EER 수치가 앱 안에 그려진다."

| µ-step | 작업 | DoD |
|---|---|---|
| V8-µ1 | `FolderFrameSource : IFrameSource` — 데이터셋 폴더 재귀 스캔, 파일명 규칙으로 클래스 라벨 파싱 | 2,016장 열거 |
| V8-µ2 | **배치 러너**: `Parallel.ForEachAsync`(`MaxDegreeOfParallelism` 설정) + 진행률/취소 | 중단·재개 가능 |
| V8-µ3 | 전수 비교 스코어 행렬 생성 (genuine / impostor 분리) | CSV 내보내기 |
| V8-µ4 | **지표 계산**: FMR/FNMR 곡선, **EER**, FMR100/FMR1000/ZeroFMR, AUC | 합성 스코어 단위테스트로 정확성 검증 |
| V8-µ5 | **CMC / Rank-1, Rank-5** 계산 | 식별 성능 산출 |
| V8-µ6 | **ScottPlot으로 DET·ROC·CMC·스코어 분포 히스토그램** 렌더 | 앱 내 차트 4종 |
| V8-µ7 | **프로토콜 스위치**: closed-set / open-set / cross-sensor | research.md §6 "일반화 검증" 대응 |
| V8-µ8 | **구성 비교 실행**(고전 vs 딥 세그, CN vs MinuNet) → 결과 나란히 표 | A/B 정량 비교 리포트 |
| V8-µ9 | 실행 리포트 내보내기: JSON(수치) + PNG(차트) + 사용 설정 스냅샷 | 재현 가능한 실험 기록 |

**MVP 성공 기준(정량)** — research.md Table 4의 비교 대상:

| 항목 | MVP 목표 | 참고(논문 Table 4) |
|---|---|---|
| PolyU 동일센서 EER | **≤ 15 %** | 제안기법 7.93 % / NIST 매처 43.83 % |
| Rank-1 | **≥ 40 %** | 제안기법 64.59 % / NIST 매처 10.99 % |
| 세그멘테이션 IoU | ≥ 0.90 | — |
| 단일 프레임 종단 지연 | **≤ 400 ms** (고전) / ≤ 700 ms (딥) | — |

> MVP는 SOTA 재현이 목표가 아니라 **파이프라인이 측정 가능하게 동작함**을 증명하는 것이다.
> 목표치는 "NIST 고전 매처 수준을 넘고, 논문 제안기법의 2배 이내"로 설정했다.

---

### V9 — 패키징·보안 (research.md §8)

> **데모 문장**: "MSIX를 더블클릭하면 설치되고, DB를 열어봐도 템플릿이 평문이 아니다."

| µ-step | 작업 | DoD |
|---|---|---|
| V9-µ1 | 템플릿 암호화 — **DPAPI(`ProtectedData`, CurrentUser)** 로 BLOB 봉인 | DB 직접 열람 시 판독 불가 |
| V9-µ2 | **원본 이미지 비저장 정책** 기본 ON (디버그 모드에서만 저장 허용) | 설정 기본값 확인 |
| V9-µ3 | 데이터 삭제 기능(사용자별 완전 삭제 + VACUUM) | 삭제 후 잔존 레코드 0 |
| V9-µ4 | MSIX 패키징 프로젝트, self-contained, 코드서명(테스트 인증서) | 다른 PC에서 설치·실행 |
| V9-µ5 | 최초 실행 온보딩: 카메라 권한·데이터 취급 고지·동의 기록 | 동의 없이는 캡처 불가 |
| V9-µ6 | 크래시 핸들러 + 로그 수집 폴더 바로가기 | 미처리 예외가 로그로 남음 |
| V9-µ7 | `README` / 설치·사용 가이드 / 알려진 한계 문서화 | 3자 인수인계 가능 |

> **MVP 범위 밖(명시적 제외)**: PAD(위조방지), Cancelable/Fuzzy Vault 템플릿 보호, 동형암호 매칭, 페더레이티드 학습.
> 이들은 research.md §8의 항목이나 MVP 테스트 앱의 목적(파이프라인 검증)을 넘어선다. V10+ 백로그로 이관.

---

## 4. 일정 (단일 개발자 기준 · 영업일)

| 절편 | µ-step 수 | 예상 | 누적 | 데모 가능 산출물 |
|---|---|---|---|---|
| V0 | 8 | 2.0d | 2.0d | 이미지 로드 → 파이프라인 뷰어 |
| V1 | 9 | 2.5d | 4.5d | 카메라 프리뷰 + 수동 노출 잠금 + 캡처 |
| V2 | 6 | 1.5d | 6.0d | 실시간 품질 HUD + 캡처 게이팅 |
| V3 | 8 | 2.0d | 8.0d | 손가락 ROI 자동 크롭 |
| V4 | 9 | 2.5d | 10.5d | 융선 향상 4단계 시각화 |
| V5 | 8 | 3.0d | 13.5d | U-Net ONNX A/B 비교 (※ µ1 병렬) |
| V6a | 5 | 2.0d | 15.5d | 미뉴셔 오버레이 |
| V6b | 7 | 5.0d | 20.5d | MinuNet ONNX (**최대 변동 구간**) |
| V7 | 8 | 3.0d | 23.5d | 등록/1:1검증/1:N식별 |
| V8 | 9 | 3.5d | 27.0d | DET·EER·CMC 리포트 |
| V9 | 7 | 2.0d | **29.0d** | MSIX 설치본 |

- **최소 데모 가능 시점**: V2 완료(6일차) — 카메라·품질까지.
- **기능 완결 시점**: V8 완료(27일차).
- **V6b 제외 시**: 24일차에 완주 가능 (폴백 레인).

---

## 5. 마일스톤 게이트

각 게이트를 통과하지 못하면 **다음 절편으로 넘어가지 않는다.**

| 게이트 | 시점 | 통과 조건 | 미통과 시 |
|---|---|---|---|
| **G1 획득 검증** | V1 종료 | 수동 노출 잠금이 실제로 동작하는 카메라 확보 | 외장 UVC 웹캠(수동제어 지원) 조달 후 재시도 |
| **G2 해상도 검증** | V2 종료 | ROI 기준 **≥ 500 ppi 등가** 확보 (USAF 타깃 또는 자 눈금 실측) | 매크로 렌즈 부착 / 카메라 교체. **미달 시 융선 인식 자체가 불가하므로 최우선 차단** |
| **G3 융선 가시성** | V4 종료 | 향상 후 융선-골이 육안으로 연속 판별 | Gabor 파라미터 재튜닝, 조명 개선(→ 01번 논문 research.md §2.4 청색광·교차편광) |
| **G4 미뉴셔 신뢰도** | V6a 종료 | 동일 손가락 재촬영 시 미뉴셔 30개 이상 안정 검출 | 향상 단계 회귀 |
| **G5 분리도** | V7 종료 | genuine/impostor 스코어 분포가 육안으로 분리 | 매칭 알고리즘 교체(MCC 도입) |
| **G6 정량 목표** | V8 종료 | EER ≤ 15 %, Rank-1 ≥ 40 % | 실패 원인을 절편별로 귀속시켜 리포트 (MVP는 "측정 가능"이 1차 목표) |

---

## 6. 테스트 전략

| 레벨 | 대상 | 도구 | 실행 시점 |
|---|---|---|---|
| 단위 | 지표 계산(EER/CMC), 미뉴셔 필터링, 좌표 변환 | xUnit + FluentAssertions | 매 커밋 |
| **스냅샷 회귀** | 영상 파이프라인 각 단계 출력 | `Verify` + SSIM 임계 비교 | 매 커밋 (V4-µ9에서 도입) |
| 통합 | 파일 소스 → 템플릿 생성 → 매칭 전 구간 | xUnit + 고정 테스트 이미지 3장 | PR |
| 성능 | 단계별 지연 예산 | BenchmarkDotNet | 주 1회 |
| 수동 E2E | 카메라 실사용 시나리오 체크리스트 | 문서화된 절차 | 절편 완료 시 |

**테스트 데이터 취급**: 실제 생체 데이터는 리포에 커밋하지 않는다. `assets/testdata/`에는 합성/공개 샘플만 두고, PolyU 등은 로컬 경로를 `appsettings.Local.json`(gitignore)으로 지정한다.

---

## 7. 리스크 관리

| # | 리스크 | 영향 | 확률 | 완화 |
|---|---|---|---|---|
| R1 | **카메라 해상도가 500 ppi 등가에 미달** | 치명 — 프로젝트 무의미 | 중 | **G2에서 조기 차단**. 매크로 렌즈/고해상 UVC 캠 사전 확보 |
| R2 | 내장캠이 수동 노출 미지원 | 고 — 재현성 붕괴 | 고 | G1. 수동제어 지원 UVC 웹캠 조달 |
| R3 | V6b 학습 데이터/라벨 확보 지연 | 중 — 딥 미뉴셔 미완 | 고 | **V6a 고전 경로로 완주 가능하게 설계** (폴백 레인) |
| R4 | DirectML EP가 특정 GPU에서 실패 | 중 | 중 | CPU EP 자동 폴백(V5-µ2), EP 선택 설정 노출 |
| R5 | WinUI 3 + OpenCvSharp 네이티브 DLL 배포 충돌 | 중 — 설치 실패 | 중 | V0-µ1에서 self-contained 배포로 **선제 검증** |
| R6 | PolyU 데이터셋 승인 지연 | 중 — V8 지연 | 중 | 공개 대체셋(RidgeBase/IITI-CFD) 병행 신청, 자체 소량 수집 |
| R7 | 파이프라인 지연이 예산 초과 | 저 | 중 | 단계별 ms 계측을 V0-µ8부터 상시 노출, 조기 감지 |
| R8 | 생체정보 수집 법적 이슈 | 고 | 저 | 개발자 본인 손가락만 사용, 동의 기록(V9-µ5), 원본 비저장 |

---

## 8. 다른 논문과의 연결

| 필요 시점 | 참조 |
|---|---|
| G2/G3 실패 시 **조명·광학 개선** | [`../01_oduah2021_hw_scanner/research.md`](../01_oduah2021_hw_scanner/research.md) §2.4(청색광·교차편광), §3(근접 광학·500 ppi 계산) |
| 딥 경로 대신 **경량 고전 ML 분류** 실험 | [`../02_rachel2025_doa_svm/research.md`](../02_rachel2025_doa_svm/research.md) — HOG+SVM은 C#에서도 이식 부담이 낮아 V6 대안 후보 |
| 접촉↔비접촉 **크로스 매칭** 확장 | 본 research.md §4.4 (C2CL), V10+ 백로그 |

---

## 9. V10+ 백로그 (MVP 이후)

1. **PAD** — 다파장/편광 기반 위조 판별 (ISO/IEC 30107-3)
2. **템플릿 보호** — BioHashing / Fuzzy Vault (ISO/IEC 24745)
3. **크로스 매칭** — C2CL 방식 contact↔contactless
4. **임베딩 매칭** — ArcFace/DeepPrint 고정길이 표현 → 1:N 대규모 식별
5. **NFIQ 2.0 P/Invoke 통합**
6. **3D 획득** — 스테레오/포토메트릭 스테레오 (research.md §2.2)
7. **모델 경량화** — INT8 양자화, 지연 50 % 단축
8. **다인종/다연령 공정성 평가** — 그룹별 EER 리포트

---

## 부록 A. 개발 환경 체크리스트

- [ ] Visual Studio 2022 17.9+ (워크로드: .NET 데스크톱 개발, **Windows App SDK C# 템플릿**)
- [ ] .NET 8 SDK (x64)
- [ ] Windows App SDK 1.5+ 런타임
- [ ] Git + **Git LFS** (`models/*.onnx` 추적)
- [ ] Python 3.11 + PyTorch (별도 venv, `tools/export_onnx/` 전용)
- [ ] 수동 노출 제어 지원 UVC 웹캠 1대 + 매크로 렌즈
- [ ] 해상도 검증용 자/USAF 1951 타깃
- [ ] 조명 리그(확산 LED) — G3 대비

## 부록 B. NuGet 패키지

```
Microsoft.WindowsAppSDK
Microsoft.Windows.SDK.BuildTools
CommunityToolkit.Mvvm
Microsoft.Extensions.Hosting
Serilog.Extensions.Hosting / Serilog.Sinks.File
OpenCvSharp4 / OpenCvSharp4.runtime.win / OpenCvSharp4.Extensions
Microsoft.ML.OnnxRuntime.DirectML
Microsoft.Data.Sqlite
ScottPlot.WinUI
xunit / FluentAssertions / Verify.Xunit   (tests)
BenchmarkDotNet                            (tests)
```
