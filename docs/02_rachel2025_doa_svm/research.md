# Research — DOA + SVM 하이브리드 비접촉 지문 인증 (고전 ML 파이프라인)

> **원논문**: Rachel, J. & Devarasan, E.
> *"Robust contactless fingerprint authentication using dolphin optimization and SVM hybridization"*,
> **Frontiers in Big Data 8:1641714 (2025-12-05)**. DOI: 10.3389/fdata.2025.1641714
> **원본 PDF**: `fdata-8-1641714.pdf` · **변환본**: `./paper.md`

---

## 0. 논문 분석 요약 (구현 대상 정의)

### 0.1 제안 파이프라인

```
PolyU CLFP 이미지(BMP)
   └─ 전처리: resize 128×128 → grayscale → histogram equalization
              → Gaussian blur(σ=0.5) → contrast(α=1.2, β=10) → normalize
              → 품질검사(Laplacian variance / contrast / brightness)
   └─ 특징추출: HOG (cell 32×32, block 2×2, 9 bins) → 324-D 벡터
   └─ [선택] 클러스터링: FCM 또는 NCM → 소속도 행렬을 특징에 concat
   └─ 최적화: DOA(Dolphin Optimization Algorithm)로 SVM(C=A, γ=ϑ) 탐색 + 특징선택
   └─ 분류: SVM (RBF kernel)
   └─ 평가: accuracy / recall / specificity, 5-fold CV, bootstrap CI, 처리시간
```

### 0.2 논문의 4개 모델과 결과 (재현 목표치)

| 모델 | 정확도(%) | 처리시간(s) | CV 지표 | Bootstrap CI | 수렴 |
|---|---|---|---|---|---|
| DOA (standalone) | **91.00** | 3,902.5 | A=87.3±2.1, R=86.8±2.3, S=87.7±1.9 | [85.2, 89.4] | 32 iter |
| DOA-SVM | **94.07** | 5,039.8 | A=91.2±1.8, R=90.7±2.0, S=91.6±1.6 | [89.4, 93.0] | 28 iter |
| FCM-DOA-SVM | **96.03** | 1,684.9 | A=93.8±1.5, R=93.3±1.7, S=94.2±1.3 | [92.3, 95.3] | 25 iter |
| **NCM-DOA-SVM** | **98.00** | 1,352.0 | A=95.1±1.2, R=94.8±1.4, S=95.3±1.1 | [93.9, 96.3] | 23 iter |

**메타휴리스틱 비교** (Table 10): Baseline SVM 88.33 % / DOA-SVM 89.69 %(±0.0132, 2073 s) / PSO-SVM 89.65 %(3722 s) / GA-SVM 89.26 %(1548 s) / ACO-SVM 89.45 %(3305 s)

**HOG 민감도** (Table 3): Fine 16×16 → 0.8446 (1764-D) / **Medium 32×32 → 0.8562 (324-D)** / Coarse 64×64 → 0.8430. 특징 크기-정확도 상관 r = −0.2498, ANOVA p ≥ 0.05.

### 0.3 하이퍼파라미터 (Table 4 — 그대로 재현 대상)

| 항목 | 값 |
|---|---|
| 데이터 분할 | 75 % train / 25 % test (2,016 → 1,512 / 504) |
| 클래스 수 | 50 |
| 돌고래 개체수 | 30 |
| 최대 반복 | 50 |
| Early stopping | 15 iter (개선 임계 0.001) |
| CV fold | 5 (stratified) |
| Bootstrap | 50 |
| 이미지 해상도 | 128×128 |
| HOG block / cell | 2 / 32 |
| A1 (인지 계수) | `2 × (1 − I/MI)` |
| A2 (사회 계수) | `−1 + I × (−1/MI)` |
| ψ | `2 × (1 − I/MI)` |
| ω (관성) | 0.7 |
| SVM 탐색범위 | C(A) ∈ [0.1, 100], γ(ϑ) ∈ [0.001, 1] |
| FCM/NCM | n_clusters=3, m=2.0, max_iter=100, ε=1e−4 |

### 0.4 논문의 **결함/모순** — 구현 시 반드시 처리할 것

1. **클래스 수 불일치**: 데이터셋은 336 클래스 × 6장 = 2,016장인데 Table 4는 "Class count 50". 50 클래스만 쓴 서브셋인지, 오기인지 불명 → **두 설정 모두 실험하고 명시**.
2. **데이터 누수 위험**: §3.6.1에서 "적합도 = **테스트셋**의 SSE"라고 기술 (§3.6 본문은 validation set). 테스트셋으로 하이퍼파라미터를 고르면 98 %는 낙관 편향. → **train/val/test 3분할 강제**.
3. **DOA ≈ PSO**: Algorithm 1의 위치·속도 갱신식(Eq.4–5)은 사실상 **표준 PSO** (인지+사회 성분). "echolocation"의 차별점은 Eq.6–9의 랜덤 재배치·순위 기반 스위칭뿐. → 구현 시 **PSO를 정직한 baseline으로 반드시 병기**.
4. **DOA standalone(91 %)이 baseline SVM(88.33 %)보다 높은데, DOA-SVM은 Table 10에서 89.69 %** — Table 9와 Table 10의 수치 체계가 다름(다른 프로토콜). 재현 시 프로토콜을 하나로 통일.
5. **처리시간 이상치**: DOA-SVM 실험 4에서 37,772 s (다른 회차는 ~1,350 s). 하드웨어 스로틀링/스왑 추정 → 시간 측정은 중앙값 + IQR로 보고.
6. **분산 0**: NCM-DOA-SVM 10회 전부 정확히 98.0 % → 504장 중 정확히 10장 오분류가 10회 반복 모두 동일. 시드 고정이거나 결정론적. **의심스러울 정도로 안정적** → 재현 시 시드를 바꿔가며 변동성 확인.
7. **정규화 적합도(Eq.3)** 에 `I(Accuracy > 0.98)` 페널티가 있는데 최종 보고 정확도가 정확히 98.0 % — 페널티 경계에 딱 붙음. 우연 아닐 가능성.
8. **인구통계 편향**: 92.3 % 동아시아, 18–35세, 60.1 % 남성 (논문이 스스로 명시). 일반화 한계.

---

## 1. 데이터셋 관련기술

| 데이터셋 | 규모 | 특징 | 입수 |
|---|---|---|---|
| **PolyU Contactless 2D/3D FP** (본 논문) | 2,016장 / 336 클래스, BMP 128×128 | Canon EOS 450D, LED 4개 45°, 12 cm, 균일 배경 | 홍콩폴리텍대 신청 (Lin & Kumar, TIP 2018) |
| **IITI-CFD** | 스마트폰 촬영 | 실사용 조건 | IIT Indore 공개 |
| **RidgeBase** (Jawade et al., IJCB 2022) | 크로스센서 멀티핑거 | contactless↔contact | 공개 |
| **ISPFDv1/v2**, **UNFIT** | 다양 배경/조명 | ViT 논문에서 사용 | 공개 |
| **NIST SD 300/301** | 접촉/N2N | 교차 비교용 | NIST |

**전처리 재현 코드 골격** (논문 §3.2 그대로):
```python
img = cv2.imread(p, cv2.IMREAD_COLOR)
img = cv2.resize(img, (128, 128), interpolation=cv2.INTER_AREA)
g   = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)      # 논문 명시: COLOR_RGB2GRAY
g   = cv2.equalizeHist(g)                         # histogram equalization
g   = cv2.GaussianBlur(g, (0, 0), sigmaX=0.5)     # σ=0.5
g   = cv2.convertScaleAbs(g, alpha=1.2, beta=10)  # contrast enhancement
g   = g.astype(np.float32) / 255.0                # normalization
# 품질 지표
qual = dict(lap_var=cv2.Laplacian(g, cv2.CV_64F).var(),
            contrast=g.std(), brightness=g.mean())
```

> **주의**: `equalizeHist` 후 `GaussianBlur(σ=0.5)`는 융선 고주파를 일부 죽인다. 128×128에서 융선 피치가 이미 수 픽셀이라 정보 손실이 크다 — **ablation으로 σ=0(무블러) 대조군 필수**.

---

## 2. 특징추출: HOG 관련기술

- **라이브러리**: `skimage.feature.hog` (논문 명시)
- **파라미터**:
  ```python
  from skimage.feature import hog
  feat = hog(g, orientations=9,
             pixels_per_cell=(32, 32),      # Medium 설정
             cells_per_block=(2, 2),
             block_norm='L2-Hys',
             feature_vector=True)
  # 128/32 = 4×4 cells, block 2×2 → 3×3 blocks × 4 cells × 9 bins = 324-D ✓ (논문 324와 일치)
  ```
- **수식 (논문 Eq.1–2)**: `G = sqrt(Gu² + Gv²)`, `θ = arctan(Gv/Gu)`
- **HOG의 한계와 대안(비교 실험용)**:
  | 기술 | 라이브러리 |
  |---|---|
  | LBP / Uniform LBP | `skimage.feature.local_binary_pattern` |
  | Gabor 필터뱅크 응답 | `cv2.getGaborKernel` (지문 융선에 이론적으로 더 적합) |
  | SIFT / ORB | `cv2.SIFT_create` |
  | BSIF, Dense-SIFT | 논문 대비 확장 |
  | 미뉴셔 기반 (NBIS mindtct) | 고전 AFIS 정석 |
  | CNN 임베딩 (ResNet/ViT) | 03번 논문 research.md 참조 |
- **차원의 저주**: 논문이 Fine(1764-D)에서 성능이 떨어진 것을 관찰. 이유는 **샘플 수(1512) 대비 차원** 문제 → PCA/LDA 비교군 추가 권장.

---

## 3. DOA (Dolphin Optimization Algorithm) 구현 관련기술

### 3.1 수식 (논문 Eq.4–9)

**추격(chasing) 단계** — 속도/위치:
```
Vel_{i,j}^{m+1}(s+1) = ω·Vel_{i,j}^m(s)
                     + A1·ν1·( ℘_{best,i,j}^m − χ_{i,j}^m(s) )     # 인지(개인 최적)
                     + A2·ν2·( G_{best,i,j}^m − χ_{i,j}^m(s) )     # 사회(전역 최적)

χ_{i,j}^{m+1}(s+1) = χ_{i,j}^m(s) + Vel_{i,j}^{m+1}(s+1)
```

**공격(attack) 단계** — 랜덤 재배치 (τ > ψ 일 때):
```
χ_{i,n}^{m+1}(s+1) = ν_{Q,ℓ}          (Eq.6)
χ_{i,n}^{m+1}(s+1) = ν_{i,ℓ}          (Eq.7)     ℓ = 무작위 돌고래 인덱스
```

**수영모드 전환(switching)** — 순위 기반:
```
Y_{i,j}^k = 1 / (2 · χ_{i,j}^k(s))                (Eq.8)  → 오름차순 정렬
φ_i^k(s)  = ( O_k(s) − 1 ) / ℓ                    (Eq.9)  ℓ = 전체 돌고래 수
if φ ≤ ζ:  Eq.6–7 로 위치 갱신
```

### 3.2 정규화 적합도 (Eq.3) — 과적합 방지
```
Fitness_reg = Accuracy_CV
            − λ1·Σ|p_i|          (λ1 = 0.01,  모델 파라미터 L1)
            − λ2·N_features      (λ2 = 0.005, 선택 특징 수)
            − λ3·I(Acc > 0.98)   (λ3 = 10,    비정상 고성능 페널티)
```

### 3.3 구현 스택

| 목적 | 라이브러리 |
|---|---|
| **직접 구현**(권장, 논문 수식 충실 재현) | `numpy` — 개체수 30 × 2차원(C, γ) 이므로 부담 없음 |
| 메타휴리스틱 프레임워크 | **`mealpy`** (200+ 알고리즘, PSO/GA/ACO/WOA/GWO 비교에 최적), `niapy`, `pyswarms`(PSO), `DEAP`(GA), `scikit-opt` |
| 베이지안 대조군 | **`optuna`** (TPE), `hyperopt`, `scikit-optimize` — "메타휴리스틱이 정말 필요한가" 검증용 |
| 병렬화 | `joblib.Parallel(n_jobs=-1)` — 개체별 SVM 학습이 독립적 |
| 캐싱 | `joblib.Memory` — 동일 (C, γ) 재평가 방지 |

### 3.4 골격 코드
```python
class DolphinOptimizer:
    def __init__(self, n_dolphins=30, max_iter=50, w=0.7,
                 bounds=[(0.1, 100), (0.001, 1.0)],
                 patience=15, tol=1e-3, seed=42):
        ...
    def optimize(self, fitness_fn):
        for it in range(self.max_iter):
            A1 = 2 * (1 - it / self.max_iter)          # Table 4
            A2 = -1 + it * (-1 / self.max_iter)
            psi = 2 * (1 - it / self.max_iter)
            for i in range(self.n):
                v[i] = w*v[i] + A1*r1*(pbest[i]-x[i]) + A2*r2*(gbest-x[i])
                x[i] = np.clip(x[i] + v[i], lo, hi)     # "Clip position to bounds"
                f = fitness_fn(x[i])
                ...
            if it > psi:                                 # switching mode
                Y = 1.0 / (2.0 * x); rank = Y.argsort().argsort()
                phi = (rank + 1 - 1) / self.n
                x[phi <= zeta] = np.random.uniform(lo, hi, ...)
            if no_improve >= self.patience: break
```

---

## 4. 클러스터링 관련기술 (FCM / NCM)

### 4.1 FCM (Bezdek 1984)
- 라이브러리: **`scikit-fuzzy` (`skfuzzy.cluster.cmeans`)**
```python
import skfuzzy as fuzz
cntr, u, u0, d, jm, p, fpc = fuzz.cluster.cmeans(
    X.T, c=3, m=2.0, error=1e-4, maxiter=100, seed=42)
X_enh = np.hstack([X, u.T])       # 논문: χ_enh = [χ_train, U_train]  → 324 + 3 = 327-D
```
- 테스트 시: `cmeans_predict` 로 학습된 센트로이드에 대한 소속도 산출 (**테스트 데이터로 재학습 금지**).

### 4.2 NCM (Neutrosophic C-Means, Guo & Sengur 2015) — 논문의 핵심 기여
- **T(진리) / I(불확정) / F(허위)** 3중 소속도. 파이썬 표준 구현이 없어 **직접 구현 필요**.
- 목적함수 (Guo & Sengur, *Pattern Recognition* 48:2710–2724):
```
J(T,I,F,C) = Σ_i Σ_j (w1·T_ij)^m ‖x_i − c_j‖²
           + Σ_i (w2·I_i)^m ‖x_i − c̄_imax‖²
           + δ² Σ_i (w3·F_i)^m
제약: T_ij + I_i + F_i = 1
```
- 갱신식:
  - `c_j = Σ_i (w1 T_ij)^m x_i / Σ_i (w1 T_ij)^m`
  - `c̄_i` = 소속도 상위 2개 클러스터 중심의 평균 (경계 영역 표현)
  - `T, I, F`는 각각 거리 역수 기반 정규화 (원논문 Eq.11–15 참조)
- 하이퍼파라미터: `w1, w2, w3`(가중), `δ`(이상치 임계), `m=2.0`, `c=3`
- 결과: `X_enh = [X, T, I, F]` → 324 + 3(또는 c+2) 차원
- **대안 참고 구현**: `neutrosophic-py`(비공식), MATLAB 원저자 코드 포팅, 또는 possibilistic c-means(PCM)/PFCM으로 근사 비교

### 4.3 왜 NCM이 이겼는가 — 검증할 가설
논문 주장: I(불확정) 채널이 블러·그림자 등 **경계 애매 영역**을 포착, F가 이상치 식별.
→ **Ablation 필수**: `[X]` vs `[X,T]` vs `[X,T,I]` vs `[X,T,I,F]` vs `[X,U_fcm]` 비교로
소속도 3채널이 실제 기여인지, 단순 **차원 3 추가 효과**인지 분리.

---

## 5. 분류기: SVM 관련기술

```python
from sklearn.svm import SVC
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import StratifiedKFold, cross_val_score

pipe = Pipeline([("sc", StandardScaler()),
                 ("svm", SVC(kernel="rbf", C=A, gamma=theta,
                             decision_function_shape="ovr",
                             cache_size=1000, random_state=42))])
cv = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)
```
- **논문 미기재 항목 = 반드시 명시할 것**: 특징 스케일링 여부(RBF는 스케일 민감 — 필수), 다중클래스 전략(OvO/OvR), `class_weight`.
- **336 클래스 × 6장** 구조라면 클래스당 4장 학습 → 극단적 few-shot. SVM보다 **거리 기반(1-NN + cosine)** 이 더 자연스러움 → baseline 추가.
- **속도**: `sklearn.svm.SVC`는 O(n²~n³). 대안 — `ThunderSVM`(GPU), `cuML.SVC`(RAPIDS), `LinearSVC`/`SGDClassifier`(선형), `Nystroem + LinearSVC`(커널 근사).

---

## 6. 검증·통계 관련기술

| 기법 | 논문 설정 | 라이브러리 |
|---|---|---|
| Stratified 5-fold CV | 학습셋 1,512장 내부에서 | `StratifiedKFold` |
| Early stopping | 15 iter 무개선, 임계 0.001 | 직접 구현 |
| **Bootstrap (n=50) + OOB** | 안정성/일반화 추정 | `sklearn.utils.resample`, `scipy.stats.bootstrap` |
| ANOVA (HOG 설정 비교) | p ≥ 0.05 | `scipy.stats.f_oneway`, `statsmodels` |
| 상관 (특징수 vs 정확도) | r = −0.2498 | `scipy.stats.pearsonr` |
| 모델 간 비교 | 논문 미실시 | **`mlxtend` McNemar test**, Wilcoxon signed-rank, Nemenyi post-hoc |
| 지표 | Accuracy / Recall / Specificity | `sklearn.metrics` |
| **추가 권장** | 생체인식 표준 지표 | **EER, FMR/FNMR, ROC/DET, CMC rank-1** — `pyeer`, `bob.measure` |

> 논문은 **폐집합 분류(closed-set classification)** 정확도만 보고한다. 실제 인증(authentication)은 **개집합 검증(open-set verification)** 이므로 EER/DET가 없으면 "인증 시스템"이라 부를 수 없다. → 구현 시 반드시 추가.

---

## 7. 재현 프로젝트 구조

```
02_doa_svm/
├── pyproject.toml            # uv/poetry, 의존성 고정
├── configs/
│   ├── base.yaml             # 논문 Table 4 그대로
│   └── ablation_*.yaml
├── src/
│   ├── data/loader.py        # PolyU 로딩, 336/50 클래스 옵션, 3분할
│   ├── preprocess/pipeline.py# resize/gray/eq/blur/contrast/normalize/quality
│   ├── features/hog.py       # skimage hog wrapper + 민감도 스윕
│   ├── cluster/fcm.py        # skfuzzy wrapper (predict 포함)
│   ├── cluster/ncm.py        # ★ Guo&Sengur NCM 직접 구현
│   ├── optim/doa.py          # ★ Eq.4–9 + Eq.3 정규화 적합도
│   ├── optim/baselines.py    # PSO/GA/ACO/WOA/GWO (mealpy), Optuna TPE
│   ├── models/svm.py
│   ├── eval/metrics.py       # acc/recall/spec + EER/DET/CMC
│   └── eval/stats.py         # ANOVA/bootstrap/McNemar
├── experiments/
│   ├── exp01_hog_sensitivity.py    # Table 3 재현
│   ├── exp02_four_models.py        # Table 5–9 재현
│   ├── exp03_metaheuristic_cmp.py  # Table 10 재현
│   └── exp04_ablation_ncm.py       # T/I/F 기여도 분해
├── tests/
└── reports/                  # MLflow, 그림, 표
```

---

## 8. 재현 실험 계획

| ID | 목적 | 설정 | 검증 포인트 |
|---|---|---|---|
| R1 | 전처리 ablation | σ=0/0.5, eq on/off, α·β on/off | 논문 전처리가 실제로 도움 되는가 |
| R2 | HOG 민감도 (Table 3) | cell 16/32/64 | 0.8446 / 0.8562 / 0.8430 재현 |
| R3 | 4모델 비교 (Table 9) | DOA / DOA-SVM / FCM- / NCM- | 91 / 94.07 / 96.03 / 98.00 |
| R4 | 메타휴리스틱 (Table 10) | + PSO/GA/ACO + **Optuna TPE + GridSearch** | DOA 우위가 유의한가 |
| R5 | **누수 검증** | 적합도를 test로 vs val로 | 98 % 중 몇 %p가 누수 기여인가 |
| R6 | 클래스 수 | 50 vs 336 클래스 | Table 4 모순 해소 |
| R7 | NCM ablation | [X] / [X,U] / [X,T] / [X,T,I] / [X,T,I,F] / [X, random3] | 3채널이 정보인가 차원인가 |
| R8 | 시드 변동성 | seed 10종 | "10회 전부 정확히 98.0 %" 재현 가능한가 |
| R9 | 인증 지표 | EER / DET / rank-1 | 실사용 성능 |
| R10 | 실시간성 | 추론 지연, Jetson Orin Nano 포팅 | 논문의 "resource-conscious" 주장 검증 |

---

## 9. 실무 배포 관련기술 (논문 future work 대응)

| 항목 | 기술 |
|---|---|
| 모델 서빙 | `skl2onnx` → **ONNX Runtime**, FastAPI, gRPC |
| 엣지 | **Jetson Orin Nano** / Raspberry Pi 5, `cuML`/`ThunderSVM`, TensorRT(딥 대안 시) |
| 실험 추적 | **MLflow**, DVC(데이터 버저닝), Hydra(설정 관리) |
| 병렬 탐색 | Ray Tune, Dask, `joblib` |
| 재현성 | 시드 고정(numpy/sklearn/random), `PYTHONHASHSEED`, Docker, `uv.lock` |
| CI | GitHub Actions + pytest + 성능 회귀 테스트 |
| 프라이버시 | 특징벡터 저장 시 **cancelable transform**, ISO/IEC 24745 (논문 §3.2.2 윤리 서술 대응) |

---

## 10. 리스크 및 대응

| 리스크 | 영향 | 대응 |
|---|---|---|
| PolyU 데이터셋 접근 지연 | 착수 불가 | RidgeBase/IITI-CFD로 선행, 파이프라인은 데이터셋 무관하게 설계 |
| NCM 구현 부재 | 핵심 기여 재현 불가 | Guo&Sengur 원논문 수식 직접 구현 + 단위테스트(합성 데이터에서 T+I+F=1 검증) |
| 적합도 누수로 98 % 미재현 | 결론 반박 | R5로 정량화하여 **정직한 수치** 보고 |
| 클래스 50 vs 336 모호 | 비교 불가 | 양쪽 다 보고 |
| DOA=PSO 논란 | 기여도 의문 | R4에서 통계적 유의성 검정, 차이 없으면 명시 |
| 처리시간 이상치 | 시간 비교 왜곡 | 중앙값·IQR, 격리된 벤치마크 환경(고정 CPU affinity) |
| 인구통계 편향 | 실배포 실패 | 다인종 데이터셋 추가 평가, 그룹별 EER 보고(fairness) |

---

## 11. 핵심 참고문헌 · 리소스

**알고리즘 원전**
- Kaveh, A. & Farhoudi, N., "A new optimization method: Dolphin echolocation", *Advances in Engineering Software* 59:53–70, 2013
- Bezdek, J. C. et al., "FCM: The fuzzy c-means clustering algorithm", *Computers & Geosciences* 10:191–203, 1984
- **Guo, Y. & Sengur, A., "NCM: Neutrosophic c-means clustering algorithm", *Pattern Recognition* 48:2710–2724, 2015** ← NCM 구현의 필수 문헌
- Smarandache, F., *Neutrosophy: Neutrosophic Probability, Set, and Logic*, 2003
- Cortes, C. & Vapnik, V., "Support-vector networks", *Machine Learning* 20:273–297, 1995
- Dalal & Triggs, "Histograms of Oriented Gradients for Human Detection", CVPR 2005
- Huang & Wang, "A GA-based feature selection and parameters optimization for SVM", *ESWA* 31, 2006
- Lin, S.W. et al., "PSO for parameter determination and feature selection of SVM", *ESWA* 35, 2008

**데이터셋**
- Lin, C. & Kumar, A., "Matching contactless and contact-based conventional fingerprint images", *IEEE TIP* 27:2008–2021, 2018 (**PolyU CLFP**)
- Jawade et al., "RidgeBase: A Cross-Sensor Multi-Finger Contactless Fingerprint Dataset", IJCB 2022
- Attrish et al., "A contactless fingerprint recognition system", arXiv:2108.09048 (IITI-CFD)

**라이브러리**
- `scikit-learn`, `scikit-image`, `opencv-python`, `scikit-fuzzy`, **`mealpy`**, `optuna`, `pyeer`, `mlflow`, `statsmodels`, `mlxtend`

**비교 대상 SOTA (논문 Table 1–2)**
- Peddi et al. (2025) G-MSGINet · Artan (2024) MinNet · Siddiqui et al. (2024) · Shi et al. (2022) Triplet-GAN · Kaplesh et al. (2024) ViT · Grosz et al. (2021) C2CL
