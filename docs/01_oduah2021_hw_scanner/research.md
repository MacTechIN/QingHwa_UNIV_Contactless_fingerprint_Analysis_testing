# Research — 고정밀 비접촉 지문 스캐너 (하드웨어/광학 중심)

> **원논문**: Oduah, U. I., Kevin, I. F., Oluwole, D. O., Izunobi, J. U.
> *"Towards a high-precision contactless fingerprint scanner for biometric authentication"*,
> **Array 11 (2021) 100083**, Elsevier. DOI: 10.1016/j.array.2021.100083
> **원본 PDF**: `Towards_a_high-precision_contactless_fingerprint_s.pdf` · **변환본**: `./paper.md`

---

## 0. 논문 분석 요약 (구현 대상 정의)

### 0.1 논문이 실제로 만든 것

| 항목 | 논문 값 |
|---|---|
| 목표 | 손가락이 기구물에 **전혀 닿지 않는** 2-D 비접촉 지문 획득 |
| 정렬 방식 | 카메라 양옆 **적외선 근접센서 2개** + **레이저 포인트 빔** 정렬 인디케이터 |
| 카메라 | ESP32-CAM (AI-Thinker, ESP32-WROOM + OV2640) |
| 조명 | 백색 LED(50–280 lm) → **청색 LED(λ=500 nm, 25 / 280 lm)** 로 교체 |
| 최적 작동거리 | 카메라 렌즈 ↔ 손가락 **3.5 cm** |
| 케이스 | 14 × 12 × 7 cm, 상단 개구부 4 cm |
| 제어 | 마이크로컨트롤러가 IR 센서 2개 · 레이저 · 셔터를 논리 게이트로 동기화 |
| 인터페이스 | ESP32 WiFi AP/STA + HTML/CSS 웹페이지 (ENROLL 버튼 / 검증 모드) |
| 매칭 | Python 기반 "이미지 → 바이너리 → 픽셀 단위 비교" 상관계수(correlation factor) |
| 최고 성능 | **97.51 %** (청색광 500 nm, 280 lm, 3.5 cm) / 백색광 280 lm 78.12 % / 3.5 cm 백색광 59.9 % |
| 전력 | 손가락 미검출 시 sleep mode, USB 급전 |

### 0.2 논문이 **구현하지 않고 넘긴 것** (우리가 채워야 할 부분)

1. **"correlation factor"의 정확한 정의가 논문에 없음.** Pearson/NCC 계열로 추정되나 수식·정규화 범위·전처리 미기재 → 우리가 명시적으로 정의해야 재현 가능.
2. **FAR/FRR/EER/DET 등 표준 생체인식 지표 부재.** 표 1~3은 단일 상관계수 5~10회 반복값뿐. 피험자 수·시도 수 미기재.
3. **Touch-Equivalent Image(TEI) 변환 미구현.** AFIS 호환성을 논문 서론에서 문제 제기했으나 실제 변환 파이프라인은 없음.
4. **보안 설계는 "별도 논문에서 보고 예정"이라고 명시적으로 미룸.** (템플릿 보호, 무결성, 취소가능성)
5. **PAD(위조 방지/생체 판별) 전무.**
6. λ = 500 nm를 "blue light"라 기술 — 500 nm는 엄밀히 **cyan/blue-green** 경계. 실제 구현 시 파장 선정 근거를 피부 광학으로 재검토 필요 (§3.3).

> **따라서 본 research.md의 구현 범위** = (A) 논문의 하드웨어·광학·펌웨어 재현 + (B) 논문이 비운 소프트웨어/평가/보안 스택 보강.

---

## 1. 시스템 아키텍처 (구현안)

```
┌───────────────────────── 비접촉 지문 스캐너 ──────────────────────────┐
│                                                                       │
│  [광학부]  Blue LED array(450–500nm) + 확산판 + 편광필름(교차편광)      │
│                    │                                                  │
│  [정렬부]  IR 근접센서 ×2  ──┐                                         │
│            ToF 거리센서(권장) ├─→ [MCU: ESP32-WROOM]                   │
│            레이저 포인터 ────┘        │  ├─ GPIO 논리 게이트           │
│                                       │  ├─ LEDC PWM (조명 밝기)      │
│  [촬상부]  OV2640 / OV5640 ───────────┘  ├─ esp32-camera (AEC/AGC lock)│
│                                          └─ light sleep + ext1 wakeup │
│                                       │                               │
│                        WiFi (HTTP/WebSocket, mDNS, TLS)               │
│                                       ▼                               │
│  [호스트: Python]  획득 → 세그멘테이션 → 향상 → TEI 변환 → 특징추출     │
│                    → 매칭(상관/미뉴셔) → 템플릿 DB(암호화) → 평가       │
└───────────────────────────────────────────────────────────────────────┘
```

---

## 2. 하드웨어 관련기술

### 2.1 MCU + 카메라 모듈

| 기술 | 선택지 | 비고 |
|---|---|---|
| **ESP32-CAM (AI-Thinker)** | 논문 그대로. ESP32-WROOM-32 + **OV2640**(2 MP, 1600×1200) | 가장 저렴, PSRAM 4 MB, SD 슬롯. 단 **고정초점**이라 3.5 cm 매크로가 근본 문제(§3.1) |
| **ESP32-S3-EYE / Freenove ESP32-S3-WROOM** | **OV5640**(5 MP) + **오토포커스 VCM** 지원 | 3.5 cm 근접 초점을 소프트웨어로 잡을 수 있어 **강력 권장 대안** |
| **Raspberry Pi + Pi Camera HQ + 매크로 렌즈** | IMX477 + CS/C 마운트 매크로 | 해상도·ppi 확보에 가장 유리. 논문 대비 업그레이드 경로 |
| 프로그래머 | FTDI FT232RL / CP2102 (3.3 V) | ESP32-CAM은 USB 직결 불가 |

- **핵심 드라이버**: `esp32-camera` (espressif). `camera_config_t`에서 `pixel_format=PIXFORMAT_GRAYSCALE|JPEG`, `frame_size`, `jpeg_quality`, `fb_count`.
- **재현성의 급소**: `sensor_t` 로 **AEC(자동노출)·AGC(자동이득)·AWB(화이트밸런스)를 반드시 수동 고정**해야 조도 실험(표 2·3)이 의미를 가진다.
  ```c
  sensor_t *s = esp_camera_sensor_get();
  s->set_whitebal(s, 0); s->set_awb_gain(s, 0);
  s->set_exposure_ctrl(s, 0); s->set_aec_value(s, 300);
  s->set_gain_ctrl(s, 0);     s->set_agc_gain(s, 0);
  s->set_gainceiling(s, GAINCEILING_2X);
  s->set_brightness(s,0); s->set_contrast(s,0); s->set_saturation(s,-2);
  ```
  → 이걸 안 하면 "조도를 올렸더니 상관계수가 올랐다"는 논문의 결론이 **센서 AE 보상에 먹혀** 재현되지 않는다.

### 2.2 근접/거리 센싱 (논문의 IR 2개 → 업그레이드)

| 센서 | 원리 | 장단점 |
|---|---|---|
| TCRT5000 / E18-D80NK | IR 반사형 on/off | 논문 수준. **거리값이 아니라 임계 트리거**뿐 → 3.5 cm 정밀 유지 불가 |
| **Sharp GP2Y0A41SK0F** | IR 삼각측량 아날로그 | 4–30 cm, 비선형 → LUT 캘리브레이션 필요 |
| **VL53L0X / VL53L1X (ST ToF)** | 940 nm ToF, I²C | **강력 권장**: mm 단위, 3.5 cm ±1 mm 재현, 피부 반사율 의존 낮음 |
| VCNL4040 | 근접+주변광 통합 | 주변광(ALS) 동시 측정으로 조도 보정에 유용 |

- 논문의 "센서 2개로 x, y, z 정렬"은 사실상 **z(거리) + 좌우 대칭성**만 검증. 진짜 3축 정렬을 원하면 ToF 3개 삼각 배치 또는 카메라 프리뷰 기반 **비전 정렬**(MediaPipe Hands 랜드마크) 사용.

### 2.3 정렬 인디케이터 (레이저)

- 논문: 레드 스팟 레이저(650 nm) 점등 = "정렬 완료 + 동시 셔터".
- 안전: **IEC 60825-1 Class 1 / Class 2 (≤1 mW)** 준수 필수. 손가락·눈 방향 조사 금지 각도 설계.
- 대안(더 안전·직관적): 소형 OLED(SSD1306) 또는 RGB LED 3색 피드백(멀다/맞다/가깝다), 부저 톤 변화.

### 2.4 조명 서브시스템 (논문 성능의 실제 주역)

논문 결론의 핵심은 "**청색광 + 고광속**"이다. 조명은 재현 실패의 1순위 원인이므로 가장 공들여야 한다.

| 요소 | 구현 기술 |
|---|---|
| 광원 | **450–505 nm 청색/시안 LED**. Cree XPE Blue(465 nm), Luxeon Rebel Cyan(505 nm). 논문 500 nm에 대응하려면 **505 nm cyan** 이 근접 |
| 구동 | 정전류 드라이버(AMC7135 / PT4115 / LM3405) — **PWM 밝기 제어 시 카메라 롤링셔터와의 비팅(banding) 주의** → PWM 주파수 ≥ 20 kHz 또는 아날로그 디밍 |
| 광속 제어 | 논문 축: 25 / 50 / 150 / 180 / 210 / 280 lm. **lm(광속) vs lux(조도) 혼동 주의** — 실제 측정은 **손가락 표면 조도(lux)** 로 하고 lm은 LED 스펙에서 환산 |
| 측정 | **BH1750 / TSL2591 / OPT3001** 조도센서 + 교정용 룩스미터. 분광 정밀 필요 시 **AS7341 11-ch 분광센서** |
| 균일화 | 확산판(PMMA opal), 링라이트 배치, 도광판. 논문의 "diffused white light는 균일하나 저해상"이라는 비판 지점 |
| **교차편광(cross-polarization)** | LED 앞 선형편광 필름 + 렌즈 앞 90° 직교 편광 필름 → **피부 표면 정반사(specular glare) 제거**, 융선 대비 극적 향상. 논문에 없는 저비용 고효과 기법 |
| 안전 | **IEC 62471 photobiological safety** — blue light hazard(400–500 nm) 위험군 평가. 노출 시간 짧게(플래시 수십 ms) |

---

## 3. 광학 설계 관련기술

### 3.1 근접 촬영(3.5 cm)과 해상도 — 논문의 최대 약점

- OV2640 기본 렌즈는 초점거리 f≈3.6 mm, **최소 초점 거리 ≈ 10–20 cm**. **3.5 cm는 정상 초점 밖**이다.
  → 논문 재현 시 반드시: (a) 렌즈 배럴 수동 회전으로 매크로 재초점, (b) **매크로 클립 렌즈 부착**, 또는 (c) OV5640 AF 모듈 사용.
- **필요 계산식**:
  - 배율 `m = f / (d - f)`  (d = 물체거리)
  - 지문 해상도(ppi) `= (센서 가로 픽셀 수) / (시야 폭 in inch)`
  - **목표: ≥ 500 ppi** (AFIS/FBI Appendix F, ISO/IEC 19794-4 기준). 손가락 폭 ~15 mm(0.59 in)를 500 ppi로 담으려면 **가로 ≥ 295 px가 손가락 폭에만** 배정되어야 함 → 실전은 1000 ppi 목표.
- **피사계심도(DoF)**: 근접 매크로에서 DoF가 mm 단위로 얇아짐 → 논문이 3.5 cm 고정에 집착한 진짜 이유. `DoF ≈ 2·N·c·(m+1)/m²` (N=F수, c=착란원).
- **모션블러**: 노출시간 × 손떨림. 지문 융선 피치 ~0.5 mm 기준 블러 < 1/3 융선 → **노출 ≤ 5–10 ms + 강한 플래시** 필요. 논문의 "고광속이 좋다"는 결과는 사실상 **노출시간 단축 효과**일 가능성이 큼 → 실험 설계에서 노출시간을 통제 변수로 분리해야 함.
- **MTF/해상력 검증**: **USAF 1951 resolution target**, Siemens star, slanted-edge (ISO 12233) MTF50 측정.

### 3.2 왜곡 보정
- 렌즈 배럴/핀쿠션 왜곡 → **OpenCV `calibrateCamera` + 체스보드/circle grid** 로 내부파라미터(K) 및 왜곡계수(k1,k2,p1,p2,k3) 산출 → `undistort`.
- 원근 왜곡(perspective distortion): 손가락이 곡면이라 중앙 대비 가장자리 융선 밀도 왜곡 → **원통 언랩(cylindrical unwrapping)** 또는 TPS(Thin-Plate Spline) 보정 (Zhao & Jain 2011, Lin & Kumar 2018).

### 3.3 피부 광학 (파장 선정의 과학적 근거)
- 표피 흡수 주체: **멜라닌**(단파장일수록 흡수↑, 지수적 감소), **헤모글로빈**(415 nm Soret peak, 542/577 nm Q-band).
- 산란: Mie/Rayleigh — 단파장일수록 산란↑ → **침투 깊이↓ = 표면(융선) 정보 비중↑**. ← 이것이 청색이 유리한 진짜 메커니즘.
- 반면 **415 nm 부근은 헤모글로빈 흡수 피크**라 어두워짐. **450–505 nm가 "얕은 침투 + 흡수 골짜기"의 스윗스팟** → 논문의 500 nm 선택은 결과적으로 타당.
- 다크스킨(높은 멜라닌) 대상 성능 저하 위험 → **다파장(청/녹/적) 순차 촬영 후 채널 융합** 을 개선안으로 권장. 관련 기술: multispectral fingerprint imaging (Lumidigm 방식).

---

## 4. 펌웨어 관련기술 (ESP32)

| 영역 | 기술/라이브러리 |
|---|---|
| 개발환경 | **Arduino IDE + arduino-esp32 core** (논문 방식, C/C++) 또는 **ESP-IDF v5.x** (FreeRTOS 태스크 분리·전력제어에 유리) |
| 카메라 | `esp32-camera` 드라이버, `esp_camera_fb_get()`, PSRAM 프레임버퍼 |
| 웹서버 | `esp_http_server` (IDF) / `WebServer.h`(Arduino), **MJPEG 스트림 엔드포인트** `/stream`, 정지촬영 `/capture` |
| 실시간 UI | **WebSocket** (`AsyncWebSocket`) 로 정렬 상태(거리 mm, OK/NG) 푸시 |
| 페이지 서빙 | HTML/CSS/JS를 **SPIFFS/LittleFS** 에 임베드 (논문은 HTML/CSS 하드코딩) |
| 네트워크 | STA/AP 듀얼, **WiFiManager**(프로비저닝), **mDNS**(`fpscanner.local`), 정적 IP |
| 보안 통신 | **esp-tls / mbedTLS**, HTTPS 서버, 또는 MQTT over TLS. 최소한 **HTTP Digest/토큰 인증** |
| 저전력 | `esp_light_sleep_start()` + **`esp_sleep_enable_ext1_wakeup()`** 로 IR 센서 GPIO 웨이크업 (논문의 sleep mode 대응) |
| 조명제어 | **LEDC** PWM 채널 (`ledcSetup(ch, 20000, 10)`) — 고주파로 밴딩 회피 |
| 동기화 로직 | 논문의 "logic gate" = `IR_L && IR_R && dist∈[3.4,3.6]cm` → 레이저 ON + 셔터 트리거. **디바운스 + N프레임 연속 만족** 조건 추가 권장 |
| OTA | `ArduinoOTA` / `esp_https_ota` |
| 시각 확인 | ESP32 → 호스트 전송은 **JPEG**, 그러나 **처리는 무압축 GRAYSCALE** 권장(JPEG 아티팩트가 융선을 뭉갬) |

---

## 5. 영상처리 파이프라인 관련기술 (호스트 측, Python)

논문은 "이미지를 바이너리로 변환 후 픽셀 단위 비교"라고만 서술 — 실전 파이프라인으로 구체화한다.

### 5.1 스택
`Python 3.11` · `opencv-python` · `numpy` · `scipy` · `scikit-image` · `numba`(속도) · `pillow`

### 5.2 단계별 기술

| 단계 | 기법 | 라이브러리/함수 |
|---|---|---|
| 1. 손가락 검출/세그멘테이션 | 피부색 **YCbCr/HSV 임계**, Otsu, GrabCut, **MediaPipe Hands 21 랜드마크**로 지두(fingertip) ROI | `cv2.cvtColor(COLOR_BGR2YCrCb)`, `cv2.grabCut`, `mediapipe` |
| 2. ROI 정규화 | 지두 중심·주축(PCA) 정렬 → 회전 정규화, 고정 크기 크롭 | `cv2.PCACompute`, `cv2.warpAffine` |
| 3. 해상도 정규화 | 거리(ToF) 기반 **스케일 팩터 = f(d)** 로 500 ppi 등가 리샘플 (논문 §resolution normalization 대응) | `cv2.resize(INTER_CUBIC)` |
| 4. 조명 정규화 | 배경 제거(모폴로지 top-hat), **CLAHE**, homomorphic filtering, mean/variance normalization | `cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))` |
| 5. 융선 향상 | **Gabor 필터뱅크**(방향·주파수 적응), **STFT 기반 향상**(Chikkerur), **Hessian/Frangi vesselness**, 이방성 확산 | `skimage.filters.frangi`, 직접 구현 Gabor |
| 6. 방향장/주파수 추정 | 구조텐서(sobel gradient covariance) 기반 orientation field, x-signature 융선 주파수 | `cv2.Sobel` + 고유벡터 |
| 7. 이진화·세선화 | 적응형 임계 → **Zhang-Suen thinning** | `cv2.adaptiveThreshold`, `cv2.ximgproc.thinning` (opencv-contrib) |
| 8. 미뉴셔 추출 | Crossing Number(CN) 알고리즘, 가짜 미뉴셔 제거(경계·짧은 가지·근접쌍) | 직접 구현 or **NIST NBIS `mindtct`** |
| 9. 품질평가 | **NFIQ 2.0** (NIST, 오픈소스) — 0~100 품질 점수로 재촬영 유도 | `nfiq2` CLI/lib |

### 5.3 TEI (Touch-Equivalent Image) 변환 — 논문이 문제제기만 한 부분
비접촉 영상을 기존 접촉식 AFIS와 호환시키려면:
1. **그레이 반전** (비접촉은 융선이 밝고 골이 어두움 ↔ 접촉식은 반대)
2. **500 ppi 스케일 정규화**
3. **원통형 언랩 / TPS 왜곡 보정** (곡면 → 평면)
4. **융선 폭 정규화** (접촉 시 압력에 의한 융선 굵기 재현: 모폴로지 dilation 튜닝)
5. **탄성 변형 모델링** — 논문이 지적한 "등록은 접촉식, 검증은 비접촉식일 때의 불일치" 해소
   - 참고: Lin & Kumar (2018) TIP, **C2CL: Contact to Contactless** (Grosz et al., TIFS 2022) — 딥러닝 기반 변환의 SOTA
6. 표준: **ISO/IEC 19794-4**(image), **19794-2**(minutiae), **ANSI/NIST-ITL 1-2011 NIEM**, **NIST SP 500-305**(비접촉 획득장치 평가 지침)

---

## 6. 매칭 & "correlation factor" 구현

### 6.1 논문의 상관계수 재정의 (필수)
논문 표1~3의 값을 재현하려면 정의를 고정해야 한다. 가장 합리적 해석 = **정규화 상호상관(NCC) / Pearson r**:

```
r = Σ(I - Ī)(T - T̄) / sqrt( Σ(I - Ī)² · Σ(T - T̄)² )      →  correlation factor(%) = 100·r
```
- 구현: `cv2.matchTemplate(img, tpl, cv2.TM_CCOEFF_NORMED)` (평행이동 탐색 포함)
- **주의**: 이 지표는 회전·스케일에 매우 취약 → 논문 값의 변동(같은 조건에서 46~59 % 산포)의 원인.

### 6.2 실전 매칭기 (권장 대체·병행)
| 방식 | 기술 |
|---|---|
| 위상 상관 | **POC / BLPOC (Band-Limited Phase-Only Correlation)** — 조도 변화에 강건, 서브픽셀 정렬 |
| 미뉴셔 | **NIST NBIS `bozorth3`**, **MCC (Minutia Cylinder-Code)**, **SourceAFIS**(오픈소스, .NET/Java/Python 포팅) |
| 특징점 | SIFT/ORB + RANSAC 호모그래피 (비접촉 회전 대응) |
| 딥러닝 | Siamese/Triplet 임베딩, **DeepPrint**, ArcFace 손실 (논문3 research.md 참조) |
| 융합 | score-level fusion (sum/weighted/likelihood ratio) — 상관 + 미뉴셔 |

### 6.3 평가 프로토콜 (논문에 없는 것 보강)
- **FMR/FNMR, EER, ROC/DET 곡선, FMR100/FMR1000, CMC/rank-1, AUC**
- 표준: **ISO/IEC 19795-1** (성능시험 및 보고 원칙)
- 통계: 부트스트랩 신뢰구간, McNemar/DET 비교, **ANOVA로 거리·조도 요인 유의성 검정** (논문은 통계검정 없이 표만 제시 → 반드시 보강)
- 실험 설계: **DOE(요인설계)** — 요인 = {거리 7수준 × 조도 5수준 × 파장 2수준}, 반복 n≥10, 피험자 ≥30명, 손가락 ≥5개

---

## 7. 백엔드 / 데이터 관련기술

| 영역 | 기술 |
|---|---|
| API 서버 | **FastAPI** + `uvicorn` (등록/검증/평가 엔드포인트), Pydantic 스키마 |
| 비동기 처리 | Celery + Redis (배치 매칭), 또는 `asyncio` |
| DB | **PostgreSQL**(운영) / SQLite(프로토타입). 템플릿은 BYTEA + 메타 분리 |
| 이미지 저장 | 원본은 **저장하지 않는 것이 원칙**(§8). 필요 시 암호화 오브젝트 스토리지(MinIO/S3 SSE) |
| 프론트 | 논문은 HTML/CSS. 실전은 **React/Vite** or 경량 **Alpine.js**, WebSocket 실시간 정렬 가이드 UI |
| 실험관리 | **MLflow** or Weights & Biases (거리/조도/파장 조합별 지표 추적) |
| 재현성 | `poetry`/`uv` 락파일, Docker, 시드 고정, 데이터 버저닝(DVC) |

---

## 8. 보안·프라이버시 관련기술 (논문이 "별도 보고"로 미룬 영역)

| 요구사항 | 기술 |
|---|---|
| 템플릿 보호 | **Cancelable biometrics**(BioHashing, random projection), **Fuzzy Vault**, **Fuzzy Commitment**, **Bloom filter 기반 IrreversibleTemplate** |
| 표준 | **ISO/IEC 24745** (Biometric information protection), **ISO/IEC 30136** (템플릿 보호 성능평가) |
| 위조 방지(PAD) | **ISO/IEC 30107-3**. 기법: 다파장 반사율 차(실리콘/젤라틴 판별), 맥동(PPG) 검출, 표면 텍스처 LBP/CNN, 편광 특성 |
| 저장/전송 | AES-256-GCM at rest, TLS 1.3 in transit, 키는 **ATECC608A / ESP32 eFuse + Flash Encryption + Secure Boot v2** |
| 장치 무결성 | Secure Boot, 서명된 OTA, 롤백 방지 |
| 법규 | **개인정보보호법(생체정보 = 민감정보)**, GDPR Art.9, BIPA(미국 일리노이). **원본 이미지 비저장 + 템플릿 일방향 변환 + 목적 제한 + 파기 정책** |
| 논문 강점 | 비접촉이라 **잠재지문(latent print) 잔류 위험 자체가 없음** — 이는 유지해야 할 설계 자산 |

---

## 9. 재현 실험 계획 (논문 표 1–3 재현)

| 실험 | 독립변수 | 통제변수 | 종속변수 |
|---|---|---|---|
| E1 최적거리 | d = 0.5, 2.0, 3.5, 5.0, 6.5, 8.0, 9.5 cm | 백색광 고정 광속, 노출 고정, AE/AWB lock | correlation factor, MTF50, NFIQ2 |
| E2 백색광 광속 | 50, 150, 180, 210, 280 lm @ d=3.5 | 파장, 노출 | 동일 |
| E3 청색광 | 25, 280 lm @ 500 nm, d=3.5 | 노출 | 동일 |
| E4 (추가) 노출 분리 | 노출 5/10/20 ms × 조도 | — | "고광속 효과 = 노출단축 효과인가?" 검정 |
| E5 (추가) 교차편광 on/off | — | — | glare 면적, 융선 대비 |
| E6 (추가) 접촉식 등록 ↔ 비접촉 검증 | 교차 매칭 | — | EER (논문이 지적한 핵심 문제) |

**필요 계측장비**: 룩스미터(또는 BH1750 교정), 분광기(AS7341), USAF 1951 타깃, z축 마이크로 스테이지, 광학 브레드보드/스탠드.

---

## 10. BOM (참고 견적)

| 부품 | 예시 | 수량 |
|---|---|---|
| ESP32-S3 + OV5640 AF 모듈 | Freenove ESP32-S3-WROOM CAM | 1 |
| ToF 거리센서 | VL53L1X | 2 |
| 청색/시안 LED | 505 nm 3 W ×4 + 정전류 드라이버 | 1 set |
| 편광필름 | 선형편광 시트 2매 | 1 |
| 확산판 | PMMA opal 2 mm | 1 |
| 조도센서 | BH1750 / AS7341 | 1 |
| 레이저 모듈 | 650 nm Class 2, ≤1 mW | 1 |
| 매크로 렌즈 | 클립형 10× 또는 M12 매크로 | 1 |
| 케이스 | 3D 프린팅(PETG, 무광 흑색 내벽으로 미광 억제) | 1 |
| 전원 | 5 V 2 A USB, LDO/벅 3.3 V | 1 |

---

## 11. 구현 로드맵

1. **W1** — ESP32-CAM 부팅, 스트리밍, AE/AWB 수동 고정, 매크로 초점 확보, USAF 타깃으로 ppi 실측
2. **W2** — ToF 정렬 로직 + 레이저/LED 동기 셔터, 웹 UI(ENROLL/VERIFY)
3. **W3** — 조명 하드웨어(청색+편광+확산), 조도 캘리브레이션
4. **W4** — Python 파이프라인: 세그멘테이션 → 향상 → 이진화 → 미뉴셔 + NCC 상관계수
5. **W5** — 논문 E1–E3 재현 실험 + ANOVA
6. **W6** — TEI 변환 + NBIS bozorth3 교차매칭, EER/DET 산출
7. **W7** — 템플릿 보호(BioHashing) + TLS + 저장 정책
8. **W8** — PAD 기초(다파장 차분), 문서화

---

## 12. 리스크 및 대응

| 리스크 | 영향 | 대응 |
|---|---|---|
| OV2640가 3.5 cm에서 초점 불가 | 재현 전면 실패 | OV5640 AF 또는 매크로 렌즈 선행 검증 (W1에서 게이트) |
| 500 ppi 미달 | AFIS 호환 불가 | 센서 업그레이드(IMX477) 또는 시야 축소 |
| 논문 correlation factor 정의 불명 | 수치 직접 비교 불가 | 자체 정의 명시 + EER 병행 보고 |
| 표본/피험자 수 미기재 | 통계적 재현 불가 | 자체 프로토콜(피험자 30명 이상) 수립 |
| 청색광 안전성 | 규제 리스크 | IEC 62471 평가, 노출 ms 단위 제한 |
| 조명↑ 효과가 노출단축 교란 | 결론 오귀인 | E4 통제 실험으로 분리 |

---

## 13. 핵심 참고문헌 · 리소스

**표준**
- NIST SP 500-305 — *Guidance for Evaluating Contactless Fingerprint Acquisition Devices*
- ISO/IEC 19794-2/-4, ISO/IEC 19795-1, ISO/IEC 24745, ISO/IEC 30107-3, ISO/IEC 2382-37
- FBI EBTS Appendix F (image quality specification)
- IEC 62471(광생물학적 안전), IEC 60825-1(레이저 안전)

**오픈소스**
- NIST **NBIS** (`mindtct`, `bozorth3`), **NFIQ 2.0** — https://github.com/usnistgov
- **SourceAFIS** — 오픈소스 미뉴셔 매처
- **esp32-camera** (Espressif), **arduino-esp32**
- OpenCV / scikit-image / MediaPipe

**핵심 논문**
- Lin & Kumar, "Matching Contactless and Contact-based Conventional Fingerprint Images", *IEEE TIP* 27(4), 2018
- Grosz et al., "C2CL: Contact to Contactless Fingerprint Matching", *IEEE TIFS* 17, 2022
- Priesnitz et al., "An overview of touchless 2D fingerprint recognition", *EURASIP JIVP*, 2021
- Labati et al., "Touchless Fingerprint Biometrics: A Survey on 2D and 3D Technologies", *JIT* 15, 2014
- Chikkerur et al., "Fingerprint enhancement using STFT analysis", *Pattern Recognition* 40, 2007
