# 비접촉 지문 분석 — 논문 변환 및 구현 기술 리서치

원본 PDF 3편을 [microsoft/markitdown](https://github.com/microsoft/markitdown) 으로 Markdown 변환한 뒤,
논문별 분석 내용을 **실제로 구현하기 위한 관련기술**을 각각 `research.md` 로 정리했다.

## 디렉터리

| 폴더 | 논문 | 성격 | 파일 |
|---|---|---|---|
| `01_oduah2021_hw_scanner/` | Oduah et al., *Towards a high-precision contactless fingerprint scanner*, **Array 11 (2021) 100083** | **하드웨어/광학** — ESP32-CAM, IR 근접센서, 청색광(500 nm), 3.5 cm 고정거리 | [paper.md](01_oduah2021_hw_scanner/paper.md) · [research.md](01_oduah2021_hw_scanner/research.md) |
| `02_rachel2025_doa_svm/` | Rachel & Devarasan, *Robust contactless fingerprint authentication using dolphin optimization and SVM hybridization*, **Front. Big Data 8:1641714 (2025)** | **고전 ML** — HOG + DOA + SVM + FCM/NCM 클러스터링 | [paper.md](02_rachel2025_doa_svm/paper.md) · [research.md](02_rachel2025_doa_svm/research.md) |
| `03_chowdhury2022_dl_review/` | Chowdhury & Imtiaz, *Contactless Fingerprint Recognition Using Deep Learning — A Systematic Review*, **JCP 2022, 2(3):714–730** | **딥러닝 문헌고찰** — 획득/전처리/CNN·Siamese·미뉴셔 네트워크 | [paper.md](03_chowdhury2022_dl_review/paper.md) · [research.md](03_chowdhury2022_dl_review/research.md) · **[dev_plan.md](03_chowdhury2022_dl_review/dev_plan.md)** |

## 세 논문의 관계

```
① Oduah 2021  ─ 획득(하드웨어·광학·조명·정렬)          ← 시스템의 입구
② Rachel 2025 ─ 고전 ML 인식(HOG→메타휴리스틱→SVM)     ← 경량 baseline
③ Chowdhury 2022 ─ 딥러닝 인식(세그·미뉴셔·매칭)        ← 성능 상한
```
- ①은 **영상 품질**을 결정하고, ②·③은 그 위에서 동작한다. ③이 ②를 성능에서 대체하는 관계.
- 세 논문 모두 공통으로 미해결로 남긴 것: **TEI(접촉 등가 영상) 변환 / EER 등 표준 생체 지표 / PAD(위조방지) / 템플릿 보호**.
- ③의 리뷰가 ①을 인용하고 있으며(ref. 5), ②·③ 모두 **PolyU CLFP 데이터셋**을 벤치마크로 공유한다.

## 변환 방법 재현

```bash
uv tool install "markitdown[pdf]"
markitdown Towards_a_high-precision_contactless_fingerprint_s.pdf -o docs/01_oduah2021_hw_scanner/paper.md
markitdown fdata-8-1641714.pdf                                   -o docs/02_rachel2025_doa_svm/paper.md
markitdown jcp-02-00036.pdf                                      -o docs/03_chowdhury2022_dl_review/paper.md
```

> markitdown(0.1.7, pdfminer-six 백엔드)은 2단 조판 논문에서 컬럼을 교차 병합하거나 표를 파편화한다.
> `02`, `03`의 표 영역은 변환본만으로 판단하지 말고 원본 PDF와 대조할 것.
> 각 `research.md` 상단의 요약 표는 원본 PDF 기준으로 검증된 값이다.

## 구현 계획

- **[03_chowdhury2022_dl_review/dev_plan.md](03_chowdhury2022_dl_review/dev_plan.md)** — ③ research.md를 근거로 한
  **Windows 네이티브 MVP 테스트 앱** 개발 계획서 (.NET 8 + WinUI 3 + OpenCvSharp + ONNX Runtime/DirectML).
  수직 절편(V0~V9) × 마이크로 스텝(µ-step) 구조, 마일스톤 게이트 G1~G6, 29 영업일 일정.
