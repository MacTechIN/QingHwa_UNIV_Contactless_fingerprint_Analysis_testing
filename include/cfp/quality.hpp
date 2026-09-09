// =============================================================================
//  cfp/quality.hpp — 촬영 품질 게이트 (dev_plan V2)
// =============================================================================
#pragma once
#include "cfp/abstractions.hpp"

namespace cfp {

struct QualityConfig {
    double min_sharpness = 60.0;    // Laplacian 분산 하한 (초점)
    double min_brightness = 45.0;   // 너무 어두우면 융선 대비 소실
    double max_brightness = 215.0;  // 너무 밝으면 포화
    double min_contrast   = 22.0;   // 표준편차 하한
    double max_clipping   = 0.06;   // 포화 픽셀 비율 상한
};

class SimpleQualityGate final : public IQualityGate {
public:
    explicit SimpleQualityGate(QualityConfig cfg = {}) : cfg_(cfg) {}
    QualityReport evaluate(const cv::Mat& bgr) const override;
private:
    QualityConfig cfg_;
};

}  // namespace cfp
