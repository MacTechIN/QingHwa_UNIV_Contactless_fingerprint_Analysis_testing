// =============================================================================
//  cfp/enhancer.hpp — 융선 향상 (dev_plan V4 / research.md §3)
//  Hong, Wan & Jain (1998) "Fingerprint Image Enhancement" 계열 파이프라인
// =============================================================================
#pragma once
#include "cfp/abstractions.hpp"

namespace cfp {

struct EnhancerConfig {
    int    block           = 16;     // 방향장/주파수 추정 블록 크기 (px)
    int    orient_blur     = 5;      // 방향장 평활 커널(배각 공간에서)
    double clahe_clip      = 2.0;
    int    clahe_tile      = 8;
    double min_period      = 3.0;    // 융선 주기 하한 (px) → f = 1/25 ~ 1/3
    double max_period      = 25.0;
    double gabor_sigma_mul = 0.55;   // sigma = mul * 융선주기
    int    gabor_orient_n  = 16;     // 방향 양자화 개수 (연산량/정확도 트레이드오프)
    double mask_var_thresh = 0.09;   // 정규화 영상 국소분산 하한 → 유효영역
    int    freq_bins       = 4;      // 주파수 양자화 개수 (Gabor 커널 캐시 크기)
    // [극성] 비접촉 지문은 융선이 "밝고" 골이 어둡다(research.md §3 TEI).
    // 접촉식 스캔 영상을 넣을 때는 true로 두어 극성을 뒤집는다.
    bool   invert_polarity = false;
};

class GaborRidgeEnhancer final : public IRidgeEnhancer {
public:
    explicit GaborRidgeEnhancer(EnhancerConfig cfg = {}) : cfg_(cfg) {}
    EnhanceResult enhance(const cv::Mat& roi_gray,
                          const cv::Mat& roi_mask = cv::Mat()) const override;

    // 단계별 함수를 공개해 두어 절편 단위 테스트/시각화가 가능하다.
    cv::Mat normalize(const cv::Mat& gray) const;
    cv::Mat orientation_field(const cv::Mat& norm) const;
    cv::Mat frequency_field(const cv::Mat& norm, const cv::Mat& orient) const;
    cv::Mat region_mask(const cv::Mat& norm) const;
    cv::Mat gabor_filter(const cv::Mat& norm, const cv::Mat& orient,
                         const cv::Mat& freq, const cv::Mat& mask) const;
private:
    EnhancerConfig cfg_;
};

}  // namespace cfp
