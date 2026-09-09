#include "cfp/quality.hpp"

#include <opencv2/imgproc.hpp>

namespace cfp {

// =============================================================================
//  품질 게이트 (dev_plan V2)
//
//  [핵심 로직 해설 — Laplacian 분산이 왜 초점 지표인가]
//  Laplacian은 2차 미분이므로 영상의 고주파 성분에 반응한다.
//  초점이 맞은 지문은 융선-골 경계에서 급격한 밝기 변화가 있어 고주파가 풍부하고,
//  흐려지면 저역통과 필터를 먹인 셈이 되어 고주파가 사라진다.
//  따라서 Laplacian 응답의 "분산"은 초점 상태에 단조롭게 반응한다.
//
//  [주의 — 이 지표의 한계]
//  Laplacian 분산은 절대 척도가 아니다. 같은 초점이라도
//    - 노출이 어두우면 값이 내려가고
//    - 센서 노이즈가 많으면 값이 올라간다(노이즈도 고주파다)
//  그래서 임계값은 카메라/조명 조합마다 재보정해야 하며,
//  brightness/contrast/clipping을 함께 봐야 오판을 줄일 수 있다.
//  → 임계값을 코드에 박지 않고 QualityConfig로 뺀 이유.
// =============================================================================
QualityReport SimpleQualityGate::evaluate(const cv::Mat& bgr) const {
    QualityReport r;
    if (bgr.empty()) { r.reason = "empty frame"; return r; }

    cv::Mat gray;
    if (bgr.channels() == 3)      cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);
    else if (bgr.channels() == 4) cv::cvtColor(bgr, gray, cv::COLOR_BGRA2GRAY);
    else                          gray = bgr;

    // --- 선명도: Laplacian 응답의 분산 ---
    cv::Mat lap;
    cv::Laplacian(gray, lap, CV_64F);
    cv::Scalar mu, sigma;
    cv::meanStdDev(lap, mu, sigma);
    r.sharpness = sigma[0] * sigma[0];      // 표준편차의 제곱 = 분산

    // --- 밝기 / 대비 ---
    cv::meanStdDev(gray, mu, sigma);
    r.brightness = mu[0];
    r.contrast   = sigma[0];

    // --- 포화(clipping): 0 또는 255에 붙어버린 픽셀 비율 ---
    // 포화된 영역은 정보가 이미 소실된 것이라 어떤 향상 알고리즘으로도 복구 불가.
    const int black = cv::countNonZero(gray <= 2);
    const int white = cv::countNonZero(gray >= 253);
    r.clipping = static_cast<double>(black + white) /
                 static_cast<double>(gray.total());

    if (r.sharpness < cfg_.min_sharpness)       r.reason = "out of focus";
    else if (r.brightness < cfg_.min_brightness) r.reason = "too dark";
    else if (r.brightness > cfg_.max_brightness) r.reason = "too bright";
    else if (r.contrast   < cfg_.min_contrast)   r.reason = "low contrast";
    else if (r.clipping   > cfg_.max_clipping)   r.reason = "clipped highlights/shadows";
    r.pass = r.reason.empty();
    return r;
}

}  // namespace cfp
