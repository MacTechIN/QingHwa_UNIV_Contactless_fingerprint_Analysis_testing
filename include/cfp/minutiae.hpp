// =============================================================================
//  cfp/minutiae.hpp — Crossing Number 미뉴셔 추출 (dev_plan V6a)
// =============================================================================
#pragma once
#include "cfp/abstractions.hpp"

namespace cfp {

struct MinutiaeConfig {
    int    border_margin   = 12;   // ROI 경계 근처는 위양성이 많아 버린다
    int    walk_length     = 12;   // 방향 산출을 위한 스켈레톤 추적 길이 (px)
    double min_pair_dist   = 8.0;  // 이보다 가까운 미뉴셔 쌍은 스퍼/브릿지로 간주
    int    max_minutiae    = 120;  // 품질 상위 N개만 유지
    int    min_branch_len  = 8;    // 이보다 짧은 가지 끝점은 노이즈
};

class CrossingNumberExtractor final : public IMinutiaExtractor {
public:
    explicit CrossingNumberExtractor(MinutiaeConfig cfg = {}) : cfg_(cfg) {}
    std::vector<Minutia> extract(const EnhanceResult& enh) const override;
private:
    MinutiaeConfig cfg_;
};

}  // namespace cfp
