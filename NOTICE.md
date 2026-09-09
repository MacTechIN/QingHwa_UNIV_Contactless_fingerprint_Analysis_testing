# 제3자 저작물 고지 (Third-Party Notices)

이 저장소는 구현 대상이 된 학술 논문 3편의 원문 PDF와, 그 변환본(`docs/*/paper.md`),
그리고 논문 도판에서 추출한 검증용 이미지(`assets/real/`)를 포함한다.
모두 **오픈액세스** 논문이며, 각 라이선스와 출처는 아래와 같다.

---

## 1. Oduah et al. (2021)

- **제목**: Towards a high-precision contactless fingerprint scanner for biometric authentication
- **저자**: Uzoma I. Oduah, Ifeanyichukwu F. Kevin, Daniel O. Oluwole, Josephat U. Izunobi
- **출처**: *Array* **11** (2021) 100083, Elsevier
- **DOI**: [10.1016/j.array.2021.100083](https://doi.org/10.1016/j.array.2021.100083)
- **라이선스**: **CC BY-NC-ND 4.0** — https://creativecommons.org/licenses/by-nc-nd/4.0/
- **파일**: `Towards_a_high-precision_contactless_fingerprint_s.pdf`, `docs/01_oduah2021_hw_scanner/paper.md`

> ⚠️ **NC(비영리) 및 ND(변경금지) 조건이 붙는 유일한 논문이다.**
> 이 저장소를 상업적 목적으로 이용할 경우 이 PDF와 그 변환본은 제외해야 한다.
> `paper.md`는 텍스트 추출에 의한 **포맷 변환**이며 내용을 변경하지 않았다.

## 2. Chowdhury & Imtiaz (2022)

- **제목**: Contactless Fingerprint Recognition Using Deep Learning—A Systematic Review
- **저자**: A. M. Mahmud Chowdhury, Masudul Haider Imtiaz (Clarkson University)
- **출처**: *Journal of Cybersecurity and Privacy* **2**(3), 714–730, MDPI
- **DOI**: [10.3390/jcp2030036](https://doi.org/10.3390/jcp2030036)
- **라이선스**: **CC BY 4.0** — https://creativecommons.org/licenses/by/4.0/
- **파일**: `jcp-02-00036.pdf`, `docs/03_chowdhury2022_dl_review/paper.md`
- **파생물**: `assets/real/contactless_s8_raw.png`, `contactless_s8_x3.png`,
  `contactless_s8_recapture.png`, `contactless_photostereo_x3.png`
  — 각각 본 논문의 **Figure 5(b)**(Samsung Galaxy S8 촬영 비접촉 지문)와
  **Figure 6**(포토메트릭 스테레오 촬영본)에서 크롭·확대한 것이다.
  CC BY 4.0은 출처 표시 하에 개작을 허용한다.

## 3. Rachel & Devarasan (2025)

- **제목**: Robust contactless fingerprint authentication using dolphin optimization and SVM hybridization
- **저자**: Jenisha Rachel, Ezhilmaran Devarasan (Vellore Institute of Technology)
- **출처**: *Frontiers in Big Data* **8**:1641714
- **DOI**: [10.3389/fdata.2025.1641714](https://doi.org/10.3389/fdata.2025.1641714)
- **라이선스**: **CC BY** — https://creativecommons.org/licenses/by/4.0/
- **파일**: `fdata-8-1641714.pdf`, `docs/02_rachel2025_doa_svm/paper.md`

---

## 변환 도구

PDF → Markdown 변환은 [microsoft/markitdown](https://github.com/microsoft/markitdown) 0.1.7
(MIT License, pdfminer-six 백엔드)을 사용했다.

```bash
markitdown <input>.pdf -o docs/<slug>/paper.md
```

2단 조판 논문에서 컬럼이 교차 병합되거나 표가 파편화되므로,
변환본은 원문 대조 없이 인용하지 말 것.

## 소프트웨어 의존성

| 구성요소 | 라이선스 |
|---|---|
| OpenCV 4.10 (+ contrib) | Apache 2.0 |
| ONNX Runtime | MIT |
| CMake / vcpkg | BSD-3 / MIT |

## 생체정보 취급

`assets/real/`의 이미지는 **공개 학술 논문에 이미 게재된 도판**에서 추출한 것이며,
새로 수집한 생체정보가 아니다.
새로 촬영한 지문 원본은 이 저장소에 커밋하지 않는다 (`.gitignore`: `datasets/`, `captures/`, `*.bmp`).
