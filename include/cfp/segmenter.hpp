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
    // ---- 지두(pad) 크롭 ----
    // [핵심] 예전에는 "손가락 폭 × 전체 높이의 42%"를 잘랐다. 손가락이 프레임을
    // 세로로 가득 채우면 이 영역이 매우 길쭉해지고, 그것을 고정 크기(256×352)로
    // 밀어 넣으면서 종횡비가 심하게 왜곡됐다. 가로/세로 융선 주기가 서로 달라져
    // Gabor가 엉뚱한 대역을 본다(실제 촬영본에서 관측된 증상).
    //
    // 지문이 찍히는 지두 pad는 대략 "폭 : 높이 = 1 : 1.3"의 둥근 영역이다.
    // 손가락 폭을 기준으로 그 비율만큼만 잘라 종횡비를 보존한다.
    double pad_aspect     = 1.3;      // 크롭 높이 / 손가락 폭
    double tip_margin     = 0.10;     // 손끝 곡률 구간을 건너뛰는 비율(폭 대비)
    int    out_w          = 320;      // 출력 폭 (높이는 pad_aspect로 결정)
};

class YCbCrSegmenter final : public IFingerSegmenter {
public:
    explicit YCbCrSegmenter(YCbCrSegConfig cfg = {}) : cfg_(cfg) {}
    SegmentResult segment(const cv::Mat& bgr) const override;
private:
    YCbCrSegConfig cfg_;
};

}  // namespace cfp
