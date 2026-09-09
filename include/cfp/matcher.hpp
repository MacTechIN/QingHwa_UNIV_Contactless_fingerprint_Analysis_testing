// =============================================================================
//  cfp/matcher.hpp — 미뉴셔 템플릿 정합 (dev_plan V7-µ4)
//
//  [알고리즘 개요] 회전·평행이동 불변 국소구조 서술자로 후보쌍을 만들고,
//  각 후보쌍이 정의하는 강체변환 가설을 투표로 검증한다(RANSAC 계열).
//  비접촉 지문은 접촉식 대비 회전 폭이 크므로 전역 정렬 없는
//  좌표 직접 비교는 실패한다 — 그래서 가설-검증 구조가 필요하다.
// =============================================================================
#pragma once

#include <cstddef>
#include <vector>

#include "cfp/types.hpp"

namespace cfp {

struct MatcherConfig {
    int    neighbors          = 5;      // 국소구조 서술자에 쓸 최근접 이웃 수 K
    double desc_dist_tol      = 12.0;   // 서술자 거리 성분 허용오차 (px)
    double desc_angle_tol     = 0.45;   // 서술자 각도 성분 허용오차 (rad, ~26deg)
    double inlier_radius      = 14.0;   // 강체변환 후 위치 일치 허용 반경 (px)
    double inlier_angle_tol   = 0.52;   // 방향 일치 허용오차 (rad, ~30deg)
    std::size_t max_hypotheses = 64;    // 검증할 상위 후보쌍 개수 (연산량 상한)
    int    min_inliers        = 6;      // 이 미만이면 스코어 0으로 절단
};

struct MatchResult {
    double      score      = 0.0;   // 0..1, 클수록 동일인
    int         inliers    = 0;     // 정합된 미뉴셔 수
    double      rotation   = 0.0;   // 추정 회전각 (rad)
    double      tx         = 0.0;   // 추정 평행이동
    double      ty         = 0.0;
};

class MinutiaMatcher {
public:
    explicit MinutiaMatcher(MatcherConfig cfg = {}) : cfg_(cfg) {}

    MatchResult match(const Template& probe, const Template& gallery) const;

    // 1:N 식별. 갤러리 전체에 대한 스코어를 반환(내림차순 정렬하지 않음).
    std::vector<double> identify(const Template& probe,
                                 const std::vector<Template>& gallery) const;

private:
    MatcherConfig cfg_;
};

// 각도 정규화 유틸 — 헤더에 노출해 테스트에서 직접 검증한다.
// wrap_pi   : (-pi, pi]   방향성 있는 각(미뉴셔 direction) 차이용
// wrap_half : (-pi/2, pi/2]  방향성 없는 각(융선 orientation) 차이용
double wrap_pi(double a) noexcept;
double wrap_half_pi(double a) noexcept;

}  // namespace cfp
