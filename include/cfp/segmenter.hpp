// =============================================================================
//  cfp/segmenter.hpp — 손가락 ROI 분할 (dev_plan V3, 고전 경로)
// =============================================================================
#pragma once
#include "cfp/abstractions.hpp"

namespace cfp {

struct YCbCrSegConfig {
    int cr_min = 133, cr_max = 180;   // 피부색 Cr 범위
    int cb_min = 77,  cb_max = 130;   // 피부색 Cb 범위
    double min_area_ratio = 0.02;     // 전체 대비 최소 손 면적
    double tip_fraction   = 0.42;     // 주축 상단 몇 %를 지두로 볼지
    int out_w = 256, out_h = 352;     // 회전정규화 후 출력 크기
};

class YCbCrSegmenter final : public IFingerSegmenter {
public:
    explicit YCbCrSegmenter(YCbCrSegConfig cfg = {}) : cfg_(cfg) {}
    SegmentResult segment(const cv::Mat& bgr) const override;
private:
    YCbCrSegConfig cfg_;
};

}  // namespace cfp
