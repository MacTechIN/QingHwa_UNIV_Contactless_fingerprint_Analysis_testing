# Research — 비접촉 지문인식 딥러닝 (체계적 문헌고찰 기반 구현 기술)

> **원논문**: Chowdhury, A. M. M. & Imtiaz, M. H.
> *"Contactless Fingerprint Recognition Using Deep Learning — A Systematic Review"*,
> **J. Cybersecur. Priv. 2022, 2(3), 714–730**. DOI: 10.3390/jcp2030036
> **원본 PDF**: `jcp-02-00036.pdf` · **변환본**: `./paper.md`

---

## 0. 논문 분석 요약 (구현 대상 정의)

이 논문은 **리뷰(systematic review)** 이므로 "재현할 단일 모델"이 없다.
대신 **8편의 원저 논문을 관통하는 3개 축**을 구현 대상으로 삼는다.

```
RQ1. 비접촉 지문 촬영 방법 + 이미지 센서   → §2  (하드웨어/획득)
RQ2. 고전 전처리(머신러닝) 파이프라인      → §3  (전처리/특징)
RQ3. 딥러닝 기반 인식                      → §4~§6 (모델/학습/평가)
```

### 0.1 리뷰가 정리한 촬영 장치 (Table 2)

| 장치 | 대표 연구 | 비용 | 조명 | 손가락 |
|---|---|---|---|---|
| 모바일폰 (2D) | Lee et al. | 저 | 추가조명 없음 | 단일 |
| 디지털카메라 (2D) | Hiew et al. | 저 | 탁상램프 | 단일 |
| 디지털카메라 (2D) | Genovese et al. | 중 | **녹색광** | 4지 slap |
| 웹캠 (2D) | Piuri et al. | 저 | 백색/무조명 가변 | 단일 |
| 웹캠 (2D) | Kumar & Zhou | 저 | 무조명 | slap |
| 스마트폰 (2D) | Derawi et al. | 저 | 무조명 | slap |
| 스마트폰 (2D) | Canrey et al. | 저 | 화면 가이드 + 플래시(Y/N) | slap |
| 스마트폰 (2D) | Deb et al. | 중 | 3기기 × 조명 변화 | 엄지+검지 |
| 스마트폰 (3D) | Xie et al. | 중 | 듀얼카메라 depth | slap |

### 0.2 3D 획득 3방식

| 방식 | 구성 | 장단점 |
|---|---|---|
| **Photometric stereo** | 고속카메라 1 + LED 다수, 표면 반사율(ToF 원리) | 저비용, 단순 / 정밀도 제한 |
| **Structured light** | 고속카메라 다수 + DLP 프로젝터, 패턴 조명 → 삼각측량 | 융선-골 복원 정확 / **고가·부피 큼** |
| **Stereo vision** | 카메라 2대 이상, 대응점 삼각측량 | 단순·저가·소형 / **대응점 계산 느림** |

### 0.3 리뷰가 정리한 전처리 과제 (Table 3)

| 과제 | 연구/연도 | 접근 |
|---|---|---|
| 손가락 세그멘테이션 | Wang et al. 2017 | **YCbCr 피부색 추정** |
| 회전·피치 주축 추정 | Zaghetto et al. 2015 | 인공신경망 |
| 저대비 | Wang et al. 2016 | **CLAHE 및 확장** |
| 센서 거리·융선 주파수 | Zaghetto et al. 2017 | 주파수맵, 거리 근사 |
| 코어/특이점 검출 | Labati et al. 2010 | **Poincaré 지수 기반 방향장 분석** |
| 변형 보정 | Lin et al. 2018 | **Robust Thin-Plate Spline 변형 보정 모델** |

### 0.4 리뷰가 분석한 딥러닝 모델 (Table 5)

| 참조 | DB | 학습데이터 | 목적 | 입력 | 출력 | 지표 |
|---|---|---|---|---|---|---|
| [67] | Private | 275장 / 55인 | 지문 인식 | RGB→Gray 다중스케일 | 특징 매칭 | Accuracy (train 100 %, test 95 %) |
| [78] | Public | 5,780장 / 320지 | 미뉴셔 추출 | Grayscale | 미뉴셔 영상 | AUC, EER |
| [79] | Priv+Pub | 9,000/6,000/1,320 | 멀티뷰 인식 | Grayscale | 융선·골 표현 | EER |
| [80] | Public | 100장 | 미뉴셔 추출 | Grayscale | 미뉴셔 영상 | Accuracy |
| [82] | Priv+Pub | 500장 | CNN 프레임워크 | **HSV** | 두 영상 간 유사도 | ROC |
| [83] | Public | 9,920장 | 시점 보정 | Grayscale | 보정 영상 | ROC, CMC |

**주요 아키텍처 4종**
1. **ContactlessMinuNet** [79] — hourglass encoder–decoder 공유 인코더 + **2 브랜치**
   - 미뉴셔 위치 브랜치: `Conv 1×1 → BatchNorm → Sigmoid` (픽셀별 미뉴셔 확률)
   - 미뉴셔 방향 브랜치: `Conv 1×1 → BatchNorm → Tanh` (위상각 θ ∈ [0, 2π))
   - 백본: `Conv 3×3, stride=1, padding=same`
   - 결과: 미뉴셔 검출 정확도 **89.61 % → 94.10 %** (PolyU)
2. **JudgeNet / LocalNet** [82] — 전처리 없이 원본에서 미뉴셔
   - 원본 640×640 → maxpool → **45×65** → JudgeNet(미뉴셔 영역 판정) → **45×45** → LocalNet(정확 위치)
   - 멀티스케일 입력 + 4-CNN 레이어
3. **FCN + 3× Siamese (멀티뷰 3D)** [83]
   - FCN(VGG/AlexNet/GoogLeNet 백본)으로 **시맨틱 세그멘테이션**(전경/배경), softmax loss
   - Siamese 3개 = top view / side1 / side2
   - 6 conv + 1 FC, 1–5층 maxpool, 입력 패치 **256×192**, kernel 3×3 stride 2
   - feature map: 48 → 64, 96, 128, 256, 512
   - 글로벌 특징 Siamese: 입력 **310×240**, 첫 conv 3×3 + BatchNorm
   - **EER: CNN 특징 11.39 %, 미뉴셔 매칭 4.09 %**
4. **Multi-Siamese 크로스 비교** [78] — 비접촉↔접촉 매칭, 미뉴셔/융선맵/영역 3종 입력, 192×192, 5,780장(3,840 train), 증강 적용

**교차 매칭 실험 결과 (Table 4)**

| 실험 | EER | Rank-1 |
|---|---|---|
| Deformation correction model [87] / dataset A | 16.17 % | 41.82 % |
| NIST 미뉴셔 매처 [88] / dataset A | 43.83 % | 10.99 % |
| **제안기법 / dataset A** | **7.93 %** | **64.59 %** |
| Deformation correction [87] / dataset B | 21.60 % | 38.90 % |
| NIST 매처 [88] / dataset B | 38.01 % | 24.92 % |
| **제안기법 / dataset B** | **7.11 %** | **58.87 %** |

### 0.5 리뷰가 지목한 6대 미해결 과제 (§6 Discussion) — 구현 로드맵의 근거

1. **Feature learning** — 지역+전역 특징 동시 필요, 계층적 학습이 적합, 사전학습이 소규모 데이터에 유효
2. **Identification에 집중 필요** — 대부분 verification(1:1). 실제 난제는 수백만 identity의 **1:N identification**
3. **대규모 데이터셋 부재** — 실환경 고보안 요구를 만족하는 공개셋 없음
4. **데이터셋 품질** — 실환경 변이 미포착
5. **컴퓨팅 자원** — 모바일 단말에서 복잡 모델 구동/저장 곤란 → 클라우드 하이브리드
6. **학습 자원** — 데이터 효율적 학습 알고리즘 필요

---

## 1. 구현 아키텍처 (리뷰 → 실제 시스템)

```
[획득]  스마트폰/웹캠/전용 카메라 ─┬─ 2D 단일/slap
                                   └─ 3D (photometric / structured light / stereo)
   │
[검출]  손·손가락 검출 (MediaPipe Hands / YOLO / SAM)
   │
[세그]  지두 ROI 분할 (U-Net / DeepLabv3+ / FPN)          ← Ruzicka et al. 2025 (FPN)
   │
[정규]  회전·스케일 정규화, 500 ppi 등가 리샘플, TPS 왜곡보정
   │
[향상]  CLAHE / Gabor / STFT / LBP / 학습기반 enhancement (U-Net, GAN)
   │
[특징]  ┌ 미뉴셔: ContactlessMinuNet 방식 (위치 sigmoid + 방향 tanh)
        ├ 텍스처: CNN/ViT 임베딩 (ArcFace/Triplet)
        └ 융합: score-level / feature-level fusion
   │
[매칭]  Siamese 유사도 · MCC/Bozorth3 · 코사인 임베딩 · C2CL 크로스매칭
   │
[평가]  ROC / DET / EER / CMC rank-1 / AUC / FMR-FNMR
```

---

## 2. RQ1 — 획득 관련기술

### 2.1 2D 스마트폰/카메라
| 기술 | 도구 |
|---|---|
| 카메라 제어 | **Android CameraX / Camera2 API** (수동 노출·ISO·초점 잠금 필수), iOS **AVFoundation** |
| 플래시·조명 | 토치 모드, 화면 백색 조명, **화면 가이드 오버레이**(Canrey et al. 방식) |
| 오토포커스 | 매크로 모드, `AF_MODE_MACRO`, 거리 고정 |
| 거리 유지 | ARCore Depth API, ToF 센서, 화면 내 손가락 크기 기반 피드백 |
| 손 검출 | **MediaPipe Hands**(21 랜드마크, 실시간), **YOLOv8-pose**, 손끝 4점 추출 |
| NIST 지침 | **NIST SP 500-305** — 균일 조명, 배경 분리, 모션 저감 |
| 해상도 | 지문 판별에 **≥500 ppi 등가** 필요 → 손가락 폭 기준 시야 계산 필수 |

### 2.2 3D 획득
| 방식 | 구현 기술 |
|---|---|
| **Photometric stereo** | 4~8개 LED 순차 점등 + 동기 촬영 → 표면 법선 추정 → 적분으로 depth. `numpy` 선형해, Frankot-Chellappa 적분 |
| **Structured light** | DLP 프로젝터(TI DLP LightCrafter) + gray code / phase-shifting, 카메라 캘리브레이션 후 삼각측량. `OpenCV structured_light` 모듈 |
| **Stereo vision** | `cv2.stereoRectify` + **SGBM / RAFT-Stereo / CREStereo**(딥), 대응점 병목은 GPU 딥 스테레오로 해소 |
| 3D→2D 언랩 | fit-sphere unwrapping (Wang et al.), 원통 전개, TPS |
| 라이브러리 | `Open3D`, `PCL`, `trimesh`, `kornia`(미분가능 기하) |

---

## 3. RQ2 — 고전 전처리 관련기술

| 단계 | 기법 | 구현 |
|---|---|---|
| 손가락 검출/분할 | **YCbCr 피부색 임계**, LUV 이진화 + ML, sharpness 기반 **VMLOG**(Variance-Modified Laplacian of Gaussian), depth 결합 | `cv2.cvtColor(COLOR_BGR2YCrCb)`, `cv2.Laplacian` |
| 배경 분리 | GrabCut, 초점 기반(전경 선명/배경 흐림) | `cv2.grabCut` |
| ROI 정규화 | 폭·높이·해상도 정규화, PCA 주축 회전 | `cv2.PCACompute`, `warpAffine` |
| 대비 향상 | **CLAHE**, mean/variance normalization, **LBP 기반 융선-골 대비 향상**, homomorphic | `cv2.createCLAHE`, `skimage.feature.local_binary_pattern` |
| 융선 향상 | **Gabor 필터뱅크**, **STFT 기반**(Chikkerur), **Hessian matrix**(Tang et al. 2017), Frangi | `cv2.getGaborKernel`, `skimage.filters.frangi` |
| 방향장 | 구조텐서 gradient covariance | `cv2.Sobel` + eigen |
| 코어/특이점 | **Poincaré index** (Labati et al. 2010) | 직접 구현 |
| 융선 주파수 | x-signature, 주파수맵 + 센서거리 근사 | 직접 구현 |
| 변형 보정 | **Robust Thin-Plate Spline** (Lin & Kumar 2018) | `cv2.createThinPlateSplineShapeTransformer`, `scipy.interpolate.Rbf` |
| 이진화·세선화 | 적응임계 + Zhang-Suen | `cv2.ximgproc.thinning` |
| 미뉴셔(고전) | Crossing Number, 위양성 제거 | **NBIS `mindtct`** |
| 품질 | **NFIQ 2.0** | `nfiq2` |
| Level-3 (땀샘 pore) | 영상처리+ML 결합 (Genovese et al.) | 고해상 전용 |
| 블러 저감 | Wiener/Richardson-Lucy 디컨볼루션, **DeblurGAN-v2**, NAFNet | `skimage.restoration`, PyTorch |

---

## 4. RQ3 — 딥러닝 모델 관련기술

### 4.1 프레임워크
`PyTorch 2.x` (권장) · `torchvision` · **`timm`**(백본 400+) · `lightning`(학습루프) · `kornia`(미분가능 CV) · `albumentations`(증강) · `MMSegmentation`/`MMDetection` · `ONNX`/`TensorRT`

### 4.2 세그멘테이션 (전경/배경, 지두)
| 모델 | 용도 |
|---|---|
| **U-Net / U-Net++** | 소규모 데이터에 강함, 지두 마스크 |
| **DeepLabv3+ (ResNet/Xception)** | 리뷰의 "semantic segmentation + softmax loss" 대응 |
| **FPN** | Ruzicka et al. 2025 fingertip segmentation |
| **SegFormer / Mask2Former** | Transformer 계열 SOTA |
| **SAM / SAM2** | 제로샷 프롬프트 분할, 라벨링 가속에 유용 |
| 손실 | Dice + BCE, Focal, Tversky (전경 비율 불균형 대응) |

### 4.3 미뉴셔 검출 — ContactlessMinuNet 재구현
```python
# 공유 인코더: hourglass (stacked hourglass / U-Net)
enc = Hourglass(in_ch=1, base=64, depth=4)          # downsample → upsample

# 브랜치 1: 미뉴셔 위치 (픽셀별 확률 히트맵)
loc = nn.Sequential(nn.Conv2d(C, 1, 1), nn.BatchNorm2d(1), nn.Sigmoid())

# 브랜치 2: 미뉴셔 방향 (θ ∈ [0,2π) → sin/cos 2채널 권장)
dir_ = nn.Sequential(nn.Conv2d(C, 2, 1), nn.BatchNorm2d(2), nn.Tanh())
```
- **손실**: 위치 = Focal/BCE(히트맵, 가우시안 타깃) + 방향 = **각도 순환성 고려**
  → `L_dir = 1 - cos(θ_pred - θ_gt)` 또는 sin/cos 2채널 MSE (**단순 MSE는 0/2π 경계에서 폭발**)
- 멀티태스크 가중: `L = λ1·L_loc + λ2·L_dir`, uncertainty weighting(Kendall) 권장
- 관련 기법: **CenterNet 스타일 히트맵 + offset 회귀**, NMS로 피크 추출
- 참고 SOTA: **FingerNet**(Tang et al. 2017), **MinNet**(Artan 2024), **G-MSGINet**(Peddi et al. 2025, graph-involution)

### 4.4 매칭 — 메트릭 러닝
| 기술 | 설명 |
|---|---|
| **Siamese network** | 리뷰의 [78][82][83] 핵심. contrastive loss |
| **Triplet loss** + hard mining | Shi et al. 2022 Triplet-GAN |
| **ArcFace / CosFace / AdaFace** | 얼굴인식 SOTA 손실 → 지문 identification(1:N)에 직접 적용 가능. 리뷰가 지목한 "identification 부족" 해결책 |
| **DeepPrint** (Engelsma et al.) | 고정길이 지문 표현 학습 |
| **C2CL** (Grosz et al. TIFS 2022) | contact↔contactless 크로스 매칭 — 리뷰의 최대 난제 직결 |
| 미뉴셔 매칭 | **MCC (Minutia Cylinder-Code)**, NBIS `bozorth3`, SourceAFIS |
| 융합 | score-level(sum/LR), feature-level concat, rank-level Borda |

### 4.5 백본 (리뷰 언급 + 현행 SOTA)
- 리뷰 시점: **AlexNet, VGG, GoogLeNet**
- 현행 권장: **ResNet-50/101, EfficientNet-V2, ConvNeXt, ViT/DeiT, Swin Transformer**
  (Kaplesh et al. 2024가 ViT를 CLFP 분류에 적용 — 리뷰 이후 SOTA)
- 소규모 데이터 대응: **사전학습(ImageNet) + fine-tune**, **self-supervised (DINOv2, MAE, SimCLR)** — 리뷰의 "unlabeled 사전학습" 제안 직결

### 4.6 데이터 증강 (리뷰 [78]의 augmentation 대응)
- 기하: 회전(±15°), 스케일, 원근 변환, 탄성변형(**elastic/TPS — 접촉식 압력 변형 모사**)
- 광학: 밝기/대비/감마, 조명 그라디언트, 모션블러, 가우시안 노이즈, JPEG 압축
- 도메인: **CycleGAN / Triplet-GAN** 으로 contactless↔contact 도메인 변환
- 합성: **SFinGe**(NIST 합성지문), **PrintsGAN**, diffusion 기반 합성
- 라이브러리: `albumentations`, `kornia.augmentation`, `imgaug`

---

## 5. 데이터셋 관련기술

| 데이터셋 | 규모 | 특성 |
|---|---|---|
| **PolyU Contactless 2D/3D** | 2,016 / 336 클래스 | 리뷰의 벤치마크 |
| **RidgeBase** (IJCB 2022) | 크로스센서 멀티핑거 | contactless↔contact |
| **IITI-CFD** | 스마트폰 | 실사용 |
| **ISPFDv1/v2, UNFIT** | 다양 배경/조명 | ViT 연구 |
| **NIST SD 300 / SD 301(N2N)** | 접촉/롤드 | 크로스 비교 |
| **FVC2000–2006** | 접촉식 | 전통 벤치마크 |
| **SOKOTO / CASIA-FingerprintV5** | 대규모 | 사전학습 |
- **리뷰가 지적한 근본 문제**: 대규모·실환경·다인종 CLFP 공개셋 부재
  → 대응: 자체 수집(IRB/동의), **합성 데이터 사전학습 + 실데이터 파인튜닝**, **federated learning**

---

## 6. 평가 관련기술

| 지표 | 의미 | 도구 |
|---|---|---|
| **ROC / DET** | FMR-FNMR 트레이드오프 | `pyeer`, `bob.measure`, `sklearn.metrics.roc_curve` |
| **EER** | FMR=FNMR 지점 (리뷰 Table 4 핵심 지표) | `pyeer` |
| **AUC** | 곡선하면적 | `sklearn` |
| **CMC / Rank-1, Rank-5** | identification 성능 | 직접 구현 |
| FMR100 / FMR1000 / ZeroFMR | 운영점 성능 | `pyeer` |
| 미뉴셔 검출 | Precision/Recall/F1, 위치 오차(px), 각도 오차(°) | 직접 구현 |
| 세그멘테이션 | IoU, Dice | `torchmetrics` |
| 표준 | **ISO/IEC 19795-1**(성능시험), **ISO/IEC 30107-3**(PAD) | — |
| 프로토콜 | closed-set vs **open-set**, cross-sensor, cross-database (일반화 검증에 필수) | — |

---

## 7. 배포·컴퓨팅 관련기술 (리뷰 §6의 5·6번 과제)

| 과제 | 기술 |
|---|---|
| 모바일 추론 | **TFLite / ONNX Runtime Mobile / Core ML / NCNN / MNN**, **PyTorch ExecuTorch** |
| 경량 백본 | MobileNetV3, EfficientNet-Lite, MobileViT, ShuffleNet |
| 압축 | **양자화**(PTQ/QAT, INT8), **프루닝**, **지식증류**(teacher→student) |
| 엣지 가속 | Jetson Orin Nano + **TensorRT**, Coral Edge TPU, Hexagon DSP, Apple ANE |
| 클라우드 하이브리드 | 리뷰 제안 — 단말은 획득·품질검사, 서버는 매칭. gRPC + TLS, 템플릿만 전송 |
| 데이터 효율 학습 | **self-supervised 사전학습**, few-shot(ProtoNet), 능동학습, 합성데이터, transfer learning |
| MLOps | MLflow / W&B, DVC, Hydra, Docker, GitHub Actions, 모델 레지스트리 |

---

## 8. 보안·프라이버시 관련기술 (리뷰 §7이 위협으로 언급한 영역)

| 항목 | 기술 |
|---|---|
| 위조 방지(PAD) | **ISO/IEC 30107-3**. 다파장/편광 반사, 미세텍스처 CNN, rPPG 맥동, 3D 깊이 일관성 |
| 템플릿 보호 | Cancelable(BioHashing, random projection), Fuzzy Vault/Commitment, **homomorphic encryption(CKKS)로 암호화 매칭** |
| 표준 | ISO/IEC 24745, ISO/IEC 30136, ISO/IEC 2382-37 |
| 분산학습 | **Federated Learning**(Flower, FedAvg) — 원본 지문 미이동 |
| 차등 프라이버시 | Opacus (DP-SGD) |
| 적대적 공격 | adversarial patch/PGD 견고성 평가, MasterPrint(합성 마스터지문) 방어 |
| 법규 | 개인정보보호법(민감정보), GDPR Art.9, BIPA |

---

## 9. 구현 로드맵

| 단계 | 내용 | 산출물 |
|---|---|---|
| **P1** 데이터 | PolyU + RidgeBase 확보, 통합 로더·분할 프로토콜 정의(closed/open/cross-sensor) | `datasets/` |
| **P2** 고전 baseline | §3 전처리 + NBIS mindtct/bozorth3 → EER 기준선 | Table 4의 "NIST matcher" 열 재현 |
| **P3** 세그멘테이션 | U-Net/DeepLabv3+ 지두 분할, IoU ≥ 0.95 | `seg/` |
| **P4** 미뉴셔 딥모델 | ContactlessMinuNet 재구현, PolyU 94.10 % 목표 | `minunet/` |
| **P5** 매칭 | Siamese + ArcFace 임베딩, 미뉴셔 score fusion | EER 보고 |
| **P6** 크로스매칭 | contact↔contactless (C2CL 방식), Table 4의 7.93 % / 7.11 % 목표 | 핵심 성과 |
| **P7** 3D | stereo/photometric stereo 실험, 3D→2D 언랩 | 선택 |
| **P8** 경량화·배포 | 양자화 + TFLite/TensorRT, 모바일 데모 | `deploy/` |
| **P9** PAD/보안 | PAD 모델 + 템플릿 보호 | `security/` |

---

## 10. 리스크 및 대응

| 리스크 | 영향 | 대응 |
|---|---|---|
| 대규모 CLFP 데이터 부재 (리뷰 3·4번 과제) | 딥모델 과적합 | 사전학습 + 합성데이터 + 강한 증강 + few-shot |
| 리뷰 시점(2022) 이후 SOTA 이동 | 구현이 낡음 | ViT/G-MSGINet/C2CL 등 2023–2025 기법 병행 |
| 미뉴셔 GT 라벨링 비용 | P4 병목 | NBIS 자동라벨 + 사람 검수, 반자동(SAM) |
| 방향 회귀 각도 순환 문제 | 학습 발산 | sin/cos 2채널 + cosine loss |
| 개인 데이터 수집 규제 | 법적 리스크 | IRB, 명시적 동의, 원본 비저장, 템플릿 보호 |
| 크로스센서 일반화 실패 | 실사용 불가 | cross-database 평가를 기본 프로토콜로 |
| 모바일 자원 한계 (리뷰 5번) | 배포 실패 | 경량 백본 + 양자화 + 클라우드 하이브리드 |

---

## 11. 핵심 참고문헌 · 리소스

**리뷰 내 핵심 원저**
- Lin, C. & Kumar, A., "Matching contactless and contact-based conventional fingerprint images", *IEEE TIP* 27:2008–2021, 2018
- Priesnitz, J. et al., "An overview of touchless 2D fingerprint recognition", *EURASIP JIVP* 2021
- Kumar, A., *Contactless 3D Fingerprint Identification*, Springer 2018
- Wang, Y., Hassebrook, L. G., Lau, D. L., "Data acquisition and processing of 3-D fingerprints", *IEEE TIFS* 5:750–760, 2010
- Zhao, Q., Jain, A., Abramovich, G., "3D to 2D fingerprints: Unrolling and distortion correction", IJCB 2011
- Tang, Y. et al., "Contactless fingerprint image enhancement based on Hessian matrix and STFT", ICMIP 2017
- Libert, J. et al., **NIST SP 500-305**, "Guidance for evaluating contactless fingerprint acquisition devices", 2018
- Stanton, B. et al., *Usability Testing of a Contactless Fingerprint Device*, NIST 2016

**리뷰 이후 SOTA (반드시 함께 볼 것)**
- Grosz, S. et al., **"C2CL: Contact to Contactless Fingerprint Matching"**, *IEEE TIFS* 17:196–210, 2022
- Jawade, B. et al., "RidgeBase: cross-sensor multi-finger contactless fingerprint dataset", IJCB 2022
- Kaplesh, P. et al., "Vision transformer for contactless fingerprint classification", *MTAP* 84, 2024
- Artan, Y., "MinNet based contactless fingerprint matching", SIU 2024
- Peddi et al., "G-MSGINet", 2025 · Ruzicka et al., "Fingertip segmentation with FPN", 2025
- Shi et al., "Triplet-GAN 기반 데이터 증강", 2022

**표준**
- ISO/IEC 19794-2/-4, ISO/IEC 19795-1, ISO/IEC 24745, ISO/IEC 30107-3, ISO/IEC 2382-37
- ANSI/NIST-ITL 1-2011, FBI EBTS Appendix F

**오픈소스**
- NIST **NBIS**(mindtct/bozorth3), **NFIQ 2.0**, **SFinGe**
- PyTorch, timm, MMSegmentation, albumentations, kornia, MediaPipe, Open3D
- `pyeer`, `bob.bio`(생체인식 평가 프레임워크)
