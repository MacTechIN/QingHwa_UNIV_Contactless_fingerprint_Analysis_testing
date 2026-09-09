// =============================================================================
//  cfp/narrate_stages.hpp — 스택별 논문 대조 해설 생성기
//
//  각 함수는 "그 스택이 논문의 무엇을 구현했는지"(정적)와
//  "이번 입력에서 실제로 무엇이 관측됐는지"(동적)를 합쳐 반환한다.
//  실측값이 논문의 주장과 맞는지 verdict에 기록해, 발표 시 그대로 읽을 수 있게 한다.
// =============================================================================
#pragma once

#include <opencv2/core.hpp>

#include "cfp/abstractions.hpp"
#include "cfp/narration.hpp"

namespace cfp::narrate {

StageNarration capture   (const cv::Mat& bgr,           double ms, int index);
StageNarration quality   (const QualityReport& q,       double ms, int index);
StageNarration segment   (const SegmentResult& s,       double ms, int index);
StageNarration normalize (const EnhanceResult& e,       double ms, int index);
StageNarration orientation(const EnhanceResult& e,      double ms, int index);
StageNarration frequency (const EnhanceResult& e,       double ms, int index);
StageNarration gabor     (const EnhanceResult& e,       double ms, int index);
StageNarration minutiae  (const std::vector<Minutia>& m,
                          const EnhanceResult& e,       double ms, int index);

// 매칭은 파이프라인 밖(V7)이라 별도 시그니처.
StageNarration matching(double score, int inliers, double rotation_deg,
                        std::size_t n_probe, std::size_t n_gallery,
                        double ms, int index);

}  // namespace cfp::narrate
