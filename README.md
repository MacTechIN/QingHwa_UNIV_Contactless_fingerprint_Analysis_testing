# Contactless Fingerprint Recognition — MVP 테스트 앱 (C++20)

비접촉 지문인식 논문 3편을 분석하고, 그중 Chowdhury & Imtiaz(2022) 리뷰가 정리한
**고전 파이프라인**을 Windows 네이티브 C++로 구현한 검증용 테스트 하네스.

각 처리 스택이 **논문의 어느 주장을 구현했는지**를 코드에 내장하고,
실행할 때마다 실측값과 대조한 **HTML/PDF 보고서를 자동 생성**한다.

## 문서

| 문서 | 내용 |
|---|---|
| [docs/README.md](docs/README.md) | 논문 3편 변환본 + 구현 기술 리서치 |
| [docs/03_.../research.md](docs/03_chowdhury2022_dl_review/research.md) | 구현에 필요한 관련기술 총정리 |
| [docs/03_.../dev_plan.md](docs/03_chowdhury2022_dl_review/dev_plan.md) | 개발 계획서 (수직 절편 V0~V9 / µ-step) |
| **[docs/03_.../code_review.md](docs/03_chowdhury2022_dl_review/code_review.md)** | **코드리뷰 + 실촬영 검증 결과 (발표자료용)** |

## 파이프라인

```
획득 → 품질게이트 → YCbCr 분할/ROI 정규화 → CLAHE 정규화
    → 방향장 → 융선주파수 → Gabor 향상 → 이진화 → 세선화
    → Crossing Number 미뉴셔 → 국소구조+RANSAC 정합 → EER/DET/CMC
```

## 빌드 & 실행 — Windows

### 원클릭 (권장)

```powershell
git clone https://github.com/MacTechIN/QingHwa_UNIV_Contactless_fingerprint_Analysis_testing.git
cd QingHwa_UNIV_Contactless_fingerprint_Analysis_testing
powershell -ExecutionPolicy Bypass -File build_windows.ps1 -Run
```

스크립트가 vcpkg 설치 → OpenCV 빌드 → 앱 빌드 → 테스트 → 실행까지 처리한다.
**최초 실행은 OpenCV 빌드 때문에 20~40분** 걸린다(이후 캐시되어 수 분).

선행 요구: **Git**, **CMake**, **Visual Studio 2022 빌드도구(C++ 데스크톱 개발 워크로드)**
```powershell
winget install Git.Git Kitware.CMake Microsoft.VisualStudio.2022.BuildTools
```

### 수동

```powershell
vcpkg install "opencv4[contrib,png,jpeg]:x64-windows"
cmake -S . -B build -A x64 -DCMAKE_TOOLCHAIN_FILE=<vcpkg>\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
.\build\Release\ContactlessFP.exe
```

### 앱 사용법

| 버튼 | 동작 |
|---|---|
| **카메라 시작** | Media Foundation으로 웹캠 프리뷰 (약 30fps) |
| **AE/AF 잠금** | 자동 노출·초점·화이트밸런스 수동 고정 ← 조도/거리 실험의 **필수 전제** |
| **사진 열기** | PNG/JPG 파일 입력 |
| **▶ 실행** | 파이프라인 실행 → 단계별 썸네일 + 논문 대조 해설 출력 |
| **보고서** | `out/report.html` · `out/report.pdf` 생성 후 자동 열기 |

> 노트북 내장 카메라는 수동 노출 제어를 지원하지 않는 경우가 많다.
> 미지원 시 앱이 경고를 띄운다 — 수동제어 지원 UVC 웹캠 사용을 권장한다.

## 빌드 — Linux (CI/검증)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=<opencv-prefix>
cmake --build build -j
ctest --test-dir build
```

## 실행

```bash
./build/cfp_cli --demo                                  # 합성 지문 데모
./build/cfp_cli --image finger.png --dump out/stages    # 실사진 + 단계별 PNG 덤프
./build/cfp_cli --image a.png --match b.png --out out   # 두 장 매칭 + 보고서
./build/cfp_cli --image a.png --no-pdf                  # PDF 생략(HTML만)
```

실행하면 `out/report.html` (자족형, 이미지 인라인) 과 `out/report.pdf` 가 생성된다.

## 현재 상태

| 항목 | 상태 |
|---|---|
| 빌드 | ✅ g++ 11.4 / C++20 / OpenCV 4.10 |
| 테스트 | ✅ 30/30 (metrics 12, matcher 10, pipeline 8) |
| 실촬영 파이프라인 | ✅ 미뉴셔 29개, coherence 0.991, 융선주기 7.36 px |
| 실촬영 매칭 | ⚠️ 단일 변형 0.22~0.47 / 복합 열화 0.09 — [code_review.md §4.2](docs/03_chowdhury2022_dl_review/code_review.md) |
| Windows 앱 (Win32) / MF 캡처 | ✅ 구현 (실기 실행은 Windows에서 확인 필요) |
| U-Net / MinuNet ONNX | ⬜ 미구현 (dev_plan V5/V6b) |

## 논문 원문

구현 대상 논문 3편의 PDF 원문이 저장소 루트에 포함되어 있다. 모두 오픈액세스이며,
출처·라이선스·파생물 관계는 **[NOTICE.md](NOTICE.md)** 에 정리했다.

> Oduah et al.(2021)만 **CC BY-NC-ND**(비영리·변경금지)이므로, 상업적 이용 시
> 해당 PDF와 변환본을 제외해야 한다.

## 데이터 취급

- 생체 원본은 **저장소에 커밋하지 않는다** (`.gitignore`: `datasets/`, `captures/`, `*.bmp`)
- `--dump`는 디버그 전용. 운영 빌드에서는 원본 비저장이 기본이어야 한다 (dev_plan V9-µ2)
- `assets/real/`의 샘플은 구현 대상 논문에 실린 도판에서 추출한 것

---

## 최종제작

**Sam LEE**
[hotnewton@korea.ac.kr](mailto:hotnewton@korea.ac.kr) · [wooriszhome@gmail.com](mailto:wooriszhome@gmail.com)
