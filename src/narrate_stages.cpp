#include "cfp/narrate_stages.hpp"

#include <opencv2/imgproc.hpp>
#include <sstream>
#include <iomanip>

namespace cfp::narrate {

namespace {
std::string f(double v, int prec = 2) {
    std::ostringstream o; o << std::fixed << std::setprecision(prec) << v; return o.str();
}
}  // namespace

// =============================================================================
//  1. 획득
// =============================================================================
StageNarration capture(const cv::Mat& bgr, double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "획득 (Capture)";
    n.what  = "비접촉 상태의 손가락 영상을 카메라/파일에서 받아온다";
    n.ref = {Paper::Chowdhury2022, "§3 Contactless Fingerprint Capturing Methods / Table 2",
             "The NIST published a document to assess contactless fingerprint capturing methods; "
             "it describes the smartphone's uniform light lighting, backdrop segmentation, "
             "and motion reduction during capture.",
             "NIST SP 500-305 (Libert et al. 2018)"};
    n.implements =
        "리뷰가 Table 2에서 정리한 2D 획득 경로(모바일폰/디지털카메라/웹캠) 중 "
        "'단일 카메라 + 균일 조명' 구성. 3D(포토메트릭 스테레오/구조광/스테레오비전)는 "
        "리뷰 §3.2에 정리되어 있으나 본 MVP 범위 밖이다.";
    n.how =
        "Windows에서는 Media Foundation으로 노출/초점/화이트밸런스를 수동 고정한 뒤 "
        "프레임을 획득한다. 평가 모드에서는 동일 인터페이스(IFrameSource)로 폴더를 읽는다.";
    n.paper_expectation =
        "리뷰 §1: 접촉식의 잠재지문·압력변형 문제는 사라지지만, 대신 조명 변화·촬영거리·"
        "각도 왜곡·초점 불량이 새 오류원으로 등장한다. "
        "Oduah 2021은 거리 3.5cm, 청색광 500nm 280lm에서 상관계수 97.51%로 최적이라 보고.";

    if (!bgr.empty()) {
        n.observed.push_back("입력 해상도 " + std::to_string(bgr.cols) + "x" +
                             std::to_string(bgr.rows) + ", 채널 " +
                             std::to_string(bgr.channels()));
        // 지문 융선 판별에는 500ppi 등가가 필요하다(리뷰가 인용한 FBI/ISO 기준).
        // 손가락 폭을 약 15mm(0.59in)로 가정했을 때 가로 픽셀로 환산해 본다.
        const double ppi_equiv = bgr.cols / 0.59;
        n.observed.push_back("손가락 폭 15mm 가정 시 가로 해상도 등가 = " +
                             f(ppi_equiv, 0) + " ppi");
        n.verdict = (ppi_equiv >= 500.0)
            ? "500 ppi 등가 충족 → 융선/미뉴셔 수준(level-2) 분석 가능"
            : "500 ppi 등가 미달 → 융선이 표본화 한계에 걸린다. 리뷰 §6 'Dataset quality'가 "
              "지적한 획득 품질 병목에 해당하며, 매크로 렌즈/고해상 센서로 해결해야 한다";
    }
    return n;
}

// =============================================================================
//  2. 품질 게이트
// =============================================================================
StageNarration quality(const QualityReport& q, double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "품질 게이트 (Quality Gate)";
    n.what  = "융선 분석이 가능한 프레임인지 촬영 즉시 판정해, 불량 프레임을 되돌린다";
    n.ref = {Paper::Rachel2025, "§3.2.1 CLFP database (전처리 규격)",
             "...processed through resizing, grayscale conversion, histogram equalization, "
             "Gaussian blur, contrast enhancement, and normalization with quality assessment "
             "via Laplacian variance, contrast measurement, and brightness analysis.",
             "Rachel & Devarasan 2025"};
    n.implements =
        "Rachel 2025가 PolyU 전처리에서 명시한 3대 품질 지표(Laplacian 분산 / 대비 / 밝기)를 "
        "그대로 구현하고, 포화(clipping) 비율을 추가했다. "
        "Chowdhury 리뷰 §4의 'sharpness 기반 전략(VMLOG)'과 같은 계열의 초점 판정이다.";
    n.how =
        "Laplacian(2차 미분)은 고주파에 반응한다. 초점이 흐려지면 저역통과된 것과 같아 "
        "고주파가 사라지므로 Laplacian 응답의 분산이 초점 상태에 단조 반응한다.";
    n.paper_expectation =
        "Chowdhury §1: 'Distortions can be caused by non-uniformity of the finger pressure, "
        "different illumination on finger skin, or motion artifacts during image capturing.' "
        "→ 저품질 프레임을 초기에 거르지 않으면 이후 모든 단계의 오류로 전파된다.";
    n.observed.push_back("선명도(Laplacian 분산) = " + f(q.sharpness, 1));
    n.observed.push_back("밝기 평균 = " + f(q.brightness, 1) +
                         " / 대비(표준편차) = " + f(q.contrast, 1));
    n.observed.push_back("포화 픽셀 비율 = " + f(q.clipping * 100.0, 2) + " %");
    n.verdict = q.pass
        ? "게이트 통과 → 후속 스택으로 진행"
        : "게이트 차단 (" + q.reason + ") → 재촬영 유도. "
          "리뷰가 말한 '획득 단계 오류의 하류 전파'를 여기서 끊는다";
    return n;
}

// =============================================================================
//  3. 세그멘테이션 + ROI 정규화
// =============================================================================
StageNarration segment(const SegmentResult& s, double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "손가락 분할 + ROI 정규화 (Segmentation)";
    n.what  = "배경에서 손가락을 떼어내고, 지두를 잘라 자세를 정규화한다";
    n.ref = {Paper::Chowdhury2022, "§4 Table 3 (Finger Segmentation) + §4 본문",
             "To make the skin color contrast and segment the skin and background color, "
             "the analysis of the YCbCr color space represents a very vital approach.",
             "Wang et al. 2017 (Hand color estimation in YCbCr)"};
    n.implements =
        "리뷰 Table 3의 첫 행 'Finger Segmentation / Wang et al. 2017 / "
        "Hand color estimation in YCbCr'를 직접 구현. "
        "이어서 Table 3의 'Rotated pitched principal orientation estimation'(Zaghetto 2015, ANN) "
        "역할을 고전 기하(minAreaRect 주축)로 대체 구현했다.";
    n.how =
        "RGB는 밝기와 색상이 세 채널에 섞여 있어 조명이 바뀌면 임계 박스가 무너진다. "
        "YCbCr은 휘도(Y)와 색차(Cb,Cr)를 분리하므로 Y를 버리고 Cb/Cr만 보면 "
        "조명 밝기에 대체로 불변인 피부 판정이 된다. "
        "이후 최대 연결성분 → 주축 각도 → 역회전 → 지두 크롭 → 고정크기 리샘플.";
    n.paper_expectation =
        "리뷰 §4: 'Misplaced or rotated fingers and the lack of skin deformation also cause "
        "processing issues.' → 자세 정규화 없이는 이후 방향장/주파수 파라미터가 "
        "촬영마다 달라져 매칭이 불안정해진다.";
    if (s.ok) {
        n.observed.push_back("주축 각도 = " + f(s.axis_angle_deg, 1) + "° → 역회전으로 세로 정렬");
        n.observed.push_back("ROI 크기 = " + std::to_string(s.roi_gray.cols) + "x" +
                             std::to_string(s.roi_gray.rows) + " (고정 리샘플)");
        if (!s.mask.empty()) {
            const double ratio = cv::countNonZero(s.mask) /
                                 static_cast<double>(s.mask.total());
            n.observed.push_back("피부 마스크 면적 비율 = " + f(ratio * 100.0, 1) + " %");
        }
        n.verdict = "분할 성공 → 자세·크기 정규화된 ROI 확보. "
                    "리뷰가 지적한 회전/오배치 문제를 이 지점에서 흡수했다";
    } else {
        n.observed.push_back("실패 사유: " + s.reason);
        n.verdict = "고정 임계 YCbCr 박스의 한계. 리뷰 §5가 제시한 딥 세그멘테이션"
                    "(FCN/시맨틱 세그멘테이션, softmax loss)으로 교체해야 하는 사례";
    }
    return n;
}

// =============================================================================
//  4. 정규화 + CLAHE
// =============================================================================
StageNarration normalize(const EnhanceResult& e, double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "대비 정규화 (CLAHE + 표준화)";
    n.what  = "조명 불균일을 제거해 이후 단계의 임계값이 영상마다 흔들리지 않게 만든다";
    n.ref = {Paper::Chowdhury2022, "§4 Fingerprint Image Enhancement / Table 3 (Low contrast)",
             "Three different methods to achieve this appeared in the literature: "
             "a normalization using mean and variance filters, histogram enhancements like "
             "contrast-limited adaptive histogram equalization (CLAHE), and local binary "
             "patterns (LBP) for enhancing the ridge-valley contrast.",
             "Wang et al. 2016 (CLAHE and extensions)"};
    n.implements =
        "리뷰가 나열한 세 향상 기법 중 두 가지를 순서대로 결합했다: "
        "① CLAHE(국소 히스토그램 평활) → ② 평균/분산 정규화. "
        "LBP는 특징기술자 성격이 강해 본 파이프라인에서는 제외했다.";
    n.how =
        "CLAHE를 먼저 걸어 국소 대비를 살린 뒤 전역 표준화를 한다. "
        "순서를 뒤집으면 CLAHE가 표준화로 맞춘 분포를 다시 흩뜨린다. "
        "clip limit로 잡음 증폭을 제한하는 것이 일반 히스토그램 평활화와의 결정적 차이.";
    n.paper_expectation =
        "리뷰: 'Finger image enhancement should result in a fingerprint image with "
        "uniform illumination.' "
        "Oduah 2021은 조도를 50→280 lm으로 올렸을 때 상관계수가 49.55%→78.12%로 "
        "올랐다고 보고 — 대비가 곧 인식률이라는 같은 이야기다.";
    if (!e.normalized.empty()) {
        cv::Scalar mu, sd;
        cv::meanStdDev(e.normalized, mu, sd);
        n.observed.push_back("정규화 후 평균 = " + f(mu[0], 3) +
                             ", 표준편차 = " + f(sd[0], 3) + " (목표: 0, 1)");
        n.verdict = "조명 성분 제거 완료 → 이후 방향장/주파수 추정이 "
                    "절대 밝기가 아닌 구조에만 반응하게 된다";
    }
    return n;
}

// =============================================================================
//  5. 방향장
// =============================================================================
StageNarration orientation(const EnhanceResult& e, double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "융선 방향장 (Orientation Field)";
    n.what  = "픽셀마다 융선이 흐르는 방향을 추정한다. 이후 모든 단계의 기준축이 된다";
    n.ref = {Paper::Chowdhury2022, "§4 본문 + Table 3 (Core/principal singular point detection)",
             "Several operations used the ridge-line orientation and shape to detect the core "
             "point. ... Poincare-based ridge orientation analysis.",
             "Labati et al. 2010"};
    n.implements =
        "리뷰가 코어/특이점 검출의 전제로 언급한 '융선 방향장'을 구조텐서로 구현. "
        "Rachel 2025의 HOG도 같은 기울기 통계(Eq.1: G=sqrt(Gu²+Gv²), Eq.2: θ=arctan(Gv/Gu))에 "
        "기반하므로, 이 스택은 두 논문의 공통 기반에 해당한다.";
    n.how =
        "Sobel 기울기 (gx,gy)로부터 Vx=2·gx·gy, Vy=gx²-gy²를 만든다. "
        "이 (Vy,Vx)는 크기가 |g|²인 '배각 벡터'이므로, 이것을 블록 평균하는 행위 자체가 "
        "기울기 세기로 가중된 방향 평균이 된다. "
        "각도를 직접 평균하면 0°와 179°가 89.5°로 뭉개지는 wraparound 오류가 나는데, "
        "배각 공간에서는 그 두 각이 같은 벡터라 문제가 사라진다. "
        "마지막에 +90°를 더해 기울기 방향(법선)을 융선 방향으로 돌린다.";
    n.paper_expectation =
        "리뷰: 방향장이 부정확하면 코어 검출·향상·미뉴셔 방향이 연쇄적으로 틀어진다. "
        "비접촉 영상은 곡면·원근 왜곡 때문에 접촉식보다 방향장이 불안정하다.";
    if (!e.orientation.empty() && !e.mask.empty()) {
        // 방향장의 국소 일관성(coherence)을 배각 공간에서 측정한다.
        cv::Mat c2, s2;
        cv::Mat th = e.orientation;
        cv::Mat cosv(th.size(), CV_32F), sinv(th.size(), CV_32F);
        for (int y = 0; y < th.rows; ++y)
            for (int x = 0; x < th.cols; ++x) {
                const float a = th.at<float>(y, x) * 2.0f;
                cosv.at<float>(y, x) = std::cos(a);
                sinv.at<float>(y, x) = std::sin(a);
            }
        cv::boxFilter(cosv, c2, CV_32F, {9, 9});
        cv::boxFilter(sinv, s2, CV_32F, {9, 9});
        cv::Mat coh;
        cv::magnitude(c2, s2, coh);                    // 0..1, 1이면 완전 일관
        const double mean_coh = cv::mean(coh, e.mask)[0];
        n.observed.push_back("방향장 일관성(coherence) 평균 = " + f(mean_coh, 3) +
                             "  (1.0 = 완전 평행, 0.0 = 무작위)");
        n.verdict = mean_coh > 0.6
            ? "방향장이 국소적으로 잘 정렬됨 → Gabor 튜닝의 전제 충족"
            : "방향장 일관성이 낮음 → 융선 구조가 약하거나 잡음이 우세. "
              "리뷰 §4가 지적한 '저초점 융선/흐린 ROI' 상태에 해당";
    }
    return n;
}

// =============================================================================
//  6. 융선 주파수
// =============================================================================
StageNarration frequency(const EnhanceResult& e, double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "융선 주파수 (Ridge Frequency)";
    n.what  = "융선이 얼마나 촘촘한지(주기)를 블록마다 추정한다";
    n.ref = {Paper::Chowdhury2022, "§4 Table 3 (Distance to the sensor, ridge line frequency)",
             "Frequency map, sensor-finger distance approximation.",
             "Zaghetto et al. 2017"};
    n.implements =
        "리뷰 Table 3의 '센서-손가락 거리 / 융선 주파수' 항목을 x-signature 법으로 구현. "
        "리뷰가 이 둘을 한 행에 묶은 이유가 중요하다 — 비접촉에서는 촬영거리가 변하면 "
        "배율이 변하고, 배율이 변하면 융선 주파수가 통째로 이동한다.";
    n.how =
        "블록 중심에서 융선을 가로지르는 방향으로 한 줄을 뜨면 밝기가 융선-골-융선으로 "
        "주기적으로 진동한다. 이 1차원 신호의 봉우리 간 평균 간격이 융선 주기다. "
        "각 위치에서 융선을 '따라' L픽셀을 평균내면 신호는 그대로 두고 잡음만 1/sqrt(L)로 "
        "줄어든다. 블록이 작아 FFT는 주파수 분해능이 2~3 bin뿐이라 봉우리 간격 직접 측정이 낫다.";
    n.paper_expectation =
        "Oduah 2021이 거리 0.5~9.5cm를 스윕해 3.5cm에서 최고 상관계수를 얻은 것도 "
        "같은 현상이다 — 거리가 배율을, 배율이 융선 주파수를 결정한다. "
        "V3의 고정크기 ROI 리샘플이 이 변동을 1차로 흡수한다.";
    if (!e.frequency.empty() && !e.mask.empty()) {
        cv::Mat valid = (e.frequency > 0) & (e.mask > 0);
        const int cnt = cv::countNonZero(valid);
        if (cnt > 0) {
            const double mf = cv::mean(e.frequency, valid)[0];
            n.observed.push_back("평균 융선 주파수 = " + f(mf, 4) + " cycles/px  → 주기 " +
                                 f(mf > 0 ? 1.0 / mf : 0.0, 2) + " px");
            n.observed.push_back("주파수 추정 성공 영역 = " +
                                 f(100.0 * cnt / std::max(1, cv::countNonZero(e.mask)), 1) +
                                 " % (마스크 대비)");
            const double period = mf > 0 ? 1.0 / mf : 0.0;
            n.verdict = (period >= 4.0 && period <= 20.0)
                ? "융선 주기가 정상 범위 → 배율이 적절하다"
                : "융선 주기가 비정상 → 촬영거리/ROI 배율 재조정 필요. "
                  "Table 3이 '센서 거리'와 '융선 주파수'를 한 항목으로 묶은 이유가 여기서 드러난다";
        }
    }
    return n;
}

// =============================================================================
//  7. Gabor 향상 + 이진화 + 세선화
// =============================================================================
StageNarration gabor(const EnhanceResult& e, double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "Gabor 향상 → 이진화 → 세선화";
    n.what  = "추정한 방향/주파수에 맞춘 필터로 융선만 남기고, 1픽셀 골격으로 만든다";
    n.ref = {Paper::Chowdhury2022, "§4 Fingerprint Image Enhancement",
             "To improve image contrast and sharpness, image enhancement techniques such as "
             "spatial domain techniques and frequency domain techniques can be used to improve "
             "the quality. ... Reducing the blurred image from the original image is another "
             "challenge in contactless 3D fingerprint enhancement.",
             "Tang et al. 2017 (Hessian matrix + STFT)"};
    n.implements =
        "리뷰가 말한 '공간영역 + 주파수영역 향상'을 Gabor 필터로 동시에 만족시킨다. "
        "Gabor는 가우시안(공간 국소성) × 정현파(주파수 선택성)의 곱이므로 두 영역을 함께 잡는다. "
        "리뷰가 인용한 Tang et al.의 Hessian+STFT와 같은 목적의 다른 수단이다.";
    n.how =
        "커널 축을 융선 방향 u와 법선 v로 분리하고, 코사인을 v(법선)를 따라 진동시킨다. "
        "u를 따라 진동시키면 융선을 따라가며 켜졌다 꺼져 완전히 반대가 된다. "
        "방향/주파수를 각각 양자화해 커널을 미리 만들어 두고 등장하는 조합만 필터링해 "
        "연산량을 (bin 수)×(영상×커널)로 묶는다. "
        "마지막에 Zhang-Suen 세선화로 1픽셀 골격을 얻는다 — Crossing Number의 전제조건.";
    n.paper_expectation =
        "리뷰: 비접촉 영상은 초점/블러 문제로 융선-골 대비가 접촉식보다 낮으므로 "
        "향상 단계가 접촉식보다 더 결정적이다.";
    if (!e.binary.empty() && !e.mask.empty()) {
        const int m = std::max(1, cv::countNonZero(e.mask));
        n.observed.push_back("융선 픽셀 비율 = " +
                             f(100.0 * cv::countNonZero(e.binary) / m, 1) +
                             " % (이상적 융선/골 = 약 50%)");
        if (!e.skeleton.empty())
            n.observed.push_back("세선화 후 골격 픽셀 = " +
                                 std::to_string(cv::countNonZero(e.skeleton)));
        const double ratio = 100.0 * cv::countNonZero(e.binary) / m;
        n.verdict = (ratio > 30.0 && ratio < 70.0)
            ? "융선/골 균형이 정상 → 이진화 임계가 적절"
            : "융선/골 균형이 무너짐 → Gabor 파라미터 또는 극성(invert_polarity) 재점검 필요";
    }
    return n;
}

// =============================================================================
//  8. 미뉴셔 추출
// =============================================================================
StageNarration minutiae(const std::vector<Minutia>& ms, const EnhanceResult& e,
                        double ms_time, int index) {
    StageNarration n;
    n.index = index; n.ms = ms_time;
    n.stage = "미뉴셔 추출 (Crossing Number)";
    n.what  = "융선 골격에서 끝점과 분기점을 찾아 방향까지 부여한다";
    n.ref = {Paper::Chowdhury2022, "§4 Minutiae-Based Feature Extraction / §5 (ContactlessMinuNet)",
             "ROI such as minutiae, ridge valley extraction, and finger orientation estimation "
             "must also be handled with a machine-learning approach. ... The accuracy of minutiae "
             "detection and its location increased to 94.10% compared to 89.61%.",
             "Zhang et al. (ContactlessMinuNet, multi-task FCN)"};
    n.implements =
        "리뷰 §5가 소개한 딥 미뉴셔 검출(ContactlessMinuNet: hourglass 인코더 + "
        "위치 브랜치 sigmoid / 방향 브랜치 tanh)의 **고전 대응물**을 구현했다. "
        "동일한 IMinutiaExtractor 계약을 쓰므로, ONNX 모델이 준비되면 코드 변경 없이 교체된다. "
        "리뷰가 보고한 딥 방식 94.10% vs 고전 89.61%의 격차를 직접 재현·비교하는 것이 목적.";
    n.how =
        "8-이웃을 원형으로 돌며 0↔1 전이 횟수의 절반(Crossing Number)을 센다. "
        "CN=1은 끝점, CN=3은 분기점, CN=2는 그냥 지나가는 융선이다. "
        "방향은 골격을 12px 따라 걸어간 변위각으로 구한다 — 한 픽셀만 보면 45° 단위로 "
        "양자화되기 때문이다. 분기점은 세 가지 중 '줄기'를 골라야 재촬영 시 방향이 재현된다.";
    n.paper_expectation =
        "리뷰 Table 4: 고전 NIST 미뉴셔 매처는 비접촉 데이터에서 EER 43.83%로 크게 무너졌고, "
        "딥 방식은 7.93%였다. 즉 이 스택의 고전 구현은 '기준선'이지 최종 답이 아니다.";
    int ending = 0, bifur = 0;
    for (const auto& m : ms) {
        if (m.type == MinutiaType::RidgeEnding) ++ending;
        else if (m.type == MinutiaType::Bifurcation) ++bifur;
    }
    n.observed.push_back("검출 미뉴셔 = " + std::to_string(ms.size()) +
                         "개 (끝점 " + std::to_string(ending) +
                         ", 분기점 " + std::to_string(bifur) + ")");
    if (!e.skeleton.empty()) {
        const int sk = cv::countNonZero(e.skeleton);
        n.observed.push_back("골격 픽셀 대비 미뉴셔 밀도 = " +
                             f(sk > 0 ? 100.0 * ms.size() / sk : 0.0, 2) + " %");
    }
    n.verdict = (ms.size() >= 20 && ms.size() <= 120)
        ? "미뉴셔 수가 정상 범위(20~120) → 템플릿 생성 가능"
        : (ms.size() < 20
           ? "미뉴셔 부족 → 향상 단계 품질 저하. 리뷰가 지적한 비접촉 고유의 저대비 문제"
           : "미뉴셔 과다 → 세선화 잔재(스퍼/브릿지) 위양성 의심");
    return n;
}

// =============================================================================
//  9. 매칭 (파이프라인 밖, V7)
// =============================================================================
StageNarration matching(double score, int inliers, double rotation_deg,
                        std::size_t n_probe, std::size_t n_gallery,
                        double ms, int index) {
    StageNarration n;
    n.index = index; n.ms = ms;
    n.stage = "템플릿 정합 (Matching)";
    n.what  = "두 템플릿이 같은 손가락인지 스코어로 판정한다";
    n.ref = {Paper::Chowdhury2022, "§5 (Siamese networks) / Table 4",
             "The Siamese convolutional network worked very well to match image pairs "
             "(matched and unmatched) in the same network. ... they showed the estimation of "
             "EER; minutiae matching rate was 11.39% and 4.09%, respectively.",
             "Lin & Kumar 2018 / multi-Siamese cross-comparison"};
    n.implements =
        "리뷰 §5의 Siamese 유사도 학습에 대응하는 **고전 기하 정합**을 구현했다. "
        "리뷰 Table 4가 비교한 세 축(변형보정 모델 / NIST 미뉴셔 매처 / 제안 딥 기법) 중 "
        "두 번째 축의 자리다.";
    n.how =
        "2단계 구조다. ① 회전·이동 불변 국소구조 서술자로 후보쌍을 싸게 만든다. "
        "② 각 후보쌍이 함의하는 강체변환을 전체 미뉴셔에 적용해 몇 개가 맞는지 투표시킨다(RANSAC). "
        "국소만 보면 우연히 닮은 타인쌍에 속고, 전역만 보려면 정렬을 미리 알아야 하므로 "
        "두 단계가 모두 필요하다. "
        "스코어는 inliers²/(n_p·n_g)로 정규화해 미뉴셔가 적은 저품질 템플릿이 "
        "분모 효과로 부당한 고득점을 받는 것을 막는다.";
    n.paper_expectation =
        "리뷰 Table 4: 비접촉↔접촉 교차 매칭에서 NIST 고전 매처 EER 43.83%, "
        "딥 제안기법 7.93%. 고전 정합은 회전/왜곡이 큰 비접촉 조건에서 급격히 무너진다.";
    n.observed.push_back("스코어 = " + f(score, 4));
    n.observed.push_back("정합 미뉴셔(inlier) = " + std::to_string(inliers) +
                         " / probe " + std::to_string(n_probe) +
                         ", gallery " + std::to_string(n_gallery));
    n.observed.push_back("추정 회전각 = " + f(rotation_deg, 1) + "°");
    n.verdict = score > 0.3
        ? "동일 손가락으로 판정될 수준의 스코어"
        : "낮은 스코어 → 타인이거나, 리뷰가 지적한 비접촉 왜곡(원근/회전)으로 정합 실패";
    return n;
}

}  // namespace cfp::narrate
