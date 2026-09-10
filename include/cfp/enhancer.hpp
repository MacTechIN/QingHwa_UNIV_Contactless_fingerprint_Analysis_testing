// =============================================================================
//  cfp/enhancer.hpp — 융선 향상 (dev_plan V4 / research.md §3)
//  Hong, Wan & Jain (1998) "Fingerprint Image Enhancement" 계열 파이프라인
// =============================================================================
#pragma once
#include "cfp/abstractions.hpp"

namespace cfp {

struct EnhancerConfig {
    // ---- 스케일 정규화 ----
    // [핵심] 촬영거리가 변하면 배율이 변하고, 배율이 변하면 융선 주기가 통째로 이동한다
    // (research.md §3 / Chowdhury Table 3의 '센서 거리 ↔ 융선 주파수').
    // 고정 픽셀 크기로 리샘플하면 이 변동이 그대로 남아 Gabor가 엉뚱한 대역을 본다.
    // 대신 "융선 주기가 target_period px가 되도록" 리샘플하면 이후 모든 단계가
    // 항상 같은 대역에서 동작하고, 템플릿도 배율 불변이 된다.
    bool   normalize_scale = true;
    double target_period   = 9.0;    // 정규화 후 목표 융선 주기 (px)
    double min_scale       = 0.25;   // 리샘플 배율 안전 범위
    double max_scale       = 4.0;

    int    block           = 16;     // 방향장/주파수 추정 블록 크기 (px)
    int    orient_blur     = 5;      // 방향장 평활 커널(배각 공간에서)
    double min_period      = 3.0;    // 융선 주기 하한 (px) → f = 1/25 ~ 1/3
    double max_period      = 25.0;
    double gabor_sigma_mul = 0.55;   // sigma = mul * 융선주기
    int    gabor_orient_n  = 16;     // 방향 양자화 개수 (연산량/정확도 트레이드오프)
    double mask_var_thresh = 0.05;   // 밴드패스 영상 국소분산 하한 → 유효영역
    // 방향장 일관성 하한. 융선처럼 한 방향으로 나란히 흐르는 구조만 남기고
    // 음영 그라디언트나 잡음처럼 방향이 없는 영역을 배제한다.
    double mask_coherence  = 0.35;
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
    // 융선 주기를 target_period에 맞추는 배율. 추정 실패 시 1.0.
    double  estimate_scale(const cv::Mat& gray) const;
    cv::Mat orientation_field(const cv::Mat& norm) const;
    cv::Mat frequency_field(const cv::Mat& norm, const cv::Mat& orient) const;
    cv::Mat region_mask(const cv::Mat& norm) const;
    // coherence_out에 방향장 일관성 맵(32FC1, 0..1)을 함께 돌려준다.
    cv::Mat region_mask(const cv::Mat& norm, cv::Mat& coherence_out) const;
    cv::Mat gabor_filter(const cv::Mat& norm, const cv::Mat& orient,
                         const cv::Mat& freq, const cv::Mat& mask) const;
private:
    EnhancerConfig cfg_;
};

}  // namespace cfp
