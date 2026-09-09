#include "cfp/matcher.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace cfp {

namespace {
constexpr double kPi     = 3.14159265358979323846;
constexpr double kTwoPi  = 2.0 * kPi;
}  // namespace

// =============================================================================
//  각도 정규화
//
//  [핵심 로직 해설] 각도 비교에서 가장 흔한 버그는 wraparound다.
//  359도와 1도의 "차이"는 358도가 아니라 2도다. 단순 뺄셈은 이를 놓친다.
//
//  wrap_pi     : 미뉴셔 방향(direction)용. 2pi 주기이므로 (-pi, pi]로 접는다.
//  wrap_half_pi: 융선 방향장(orientation)용. 융선은 방향성이 없어
//                theta 와 theta+pi 가 같은 상태다 → pi 주기, (-pi/2, pi/2]로 접는다.
//  이 둘을 혼동하면 "정반대 방향 융선"이 완전 불일치로 잘못 처리된다.
// =============================================================================
//  구간 경계 규약: 반열린구간의 "닫힌 쪽"을 양수 끝으로 둔다.
//  즉 wrap_pi(pi) == pi 이고 wrap_pi(-pi) == pi 다(둘은 같은 각이므로).
//  `< 0` 이 아니라 `<= 0` 으로 접는 것이 이 규약을 만드는 지점이며,
//  헤더에 적힌 (-pi, pi] 계약과 정확히 일치시키기 위한 것이다.
double wrap_pi(double a) noexcept {
    a = std::fmod(a + kPi, kTwoPi);
    if (a <= 0.0) a += kTwoPi;
    return a - kPi;
}

double wrap_half_pi(double a) noexcept {
    a = std::fmod(a + kPi / 2.0, kPi);
    if (a <= 0.0) a += kPi;
    return a - kPi / 2.0;
}

namespace {

// -----------------------------------------------------------------------------
//  국소구조 서술자(local structure descriptor)
//
//  [핵심 로직 해설] 미뉴셔 하나를 "그 주변 K개 이웃과의 상대 배치"로 기술한다.
//  기준 미뉴셔의 방향(theta)을 축으로 삼아 모든 값을 상대화하므로
//  영상 전체가 회전/평행이동해도 서술자는 불변이다.
//  → 전역 정렬을 모르는 상태에서 후보쌍을 만들 수 있게 하는 장치.
//
//  성분:
//    d    : 이웃까지 거리          (스케일 불변은 아님 — 비접촉은 거리에 따라
//                                   배율이 변하므로 ROI 정규화가 선행되어야 함.
//                                   V3 회전정규화+고정크기 리샘플이 그 역할)
//    phi  : 기준 방향 대비 이웃의 방위각
//    psi  : 기준 방향 대비 이웃의 방향
//  이웃은 거리 오름차순으로 정렬해 순서 의존성을 제거한다.
// -----------------------------------------------------------------------------
struct NeighborFeat { double d, phi, psi; };

std::vector<std::vector<NeighborFeat>>
build_descriptors(const std::vector<Minutia>& ms, int k) {
    const std::size_t n = ms.size();
    std::vector<std::vector<NeighborFeat>> desc(n);

    for (std::size_t i = 0; i < n; ++i) {
        std::vector<std::pair<double, std::size_t>> byd;
        byd.reserve(n);
        for (std::size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            const double dx = ms[j].x - ms[i].x;
            const double dy = ms[j].y - ms[i].y;
            byd.emplace_back(std::sqrt(dx * dx + dy * dy), j);
        }
        std::sort(byd.begin(), byd.end());

        const std::size_t take = std::min<std::size_t>(k, byd.size());
        desc[i].reserve(take);
        for (std::size_t t = 0; t < take; ++t) {
            const std::size_t j = byd[t].second;
            const double dx = ms[j].x - ms[i].x;
            const double dy = ms[j].y - ms[i].y;
            NeighborFeat f;
            f.d   = byd[t].first;
            f.phi = wrap_pi(std::atan2(dy, dx) - ms[i].theta);   // 상대 방위각
            f.psi = wrap_pi(ms[j].theta - ms[i].theta);          // 상대 방향
            desc[i].push_back(f);
        }
    }
    return desc;
}

// 서술자 유사도: 대응 순번끼리 세 성분이 모두 허용오차 안이면 1점.
// (더 정교한 방법은 성분별 가우시안 가중이지만, MVP에서는 해석 가능성을 우선한다)
double descriptor_similarity(const std::vector<NeighborFeat>& a,
                             const std::vector<NeighborFeat>& b,
                             const MatcherConfig& cfg) {
    const std::size_t n = std::min(a.size(), b.size());
    if (n == 0) return 0.0;
    int hit = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (std::fabs(a[i].d - b[i].d) > cfg.desc_dist_tol) continue;
        if (std::fabs(wrap_pi(a[i].phi - b[i].phi)) > cfg.desc_angle_tol) continue;
        if (std::fabs(wrap_pi(a[i].psi - b[i].psi)) > cfg.desc_angle_tol) continue;
        ++hit;
    }
    return static_cast<double>(hit) / static_cast<double>(n);
}

}  // namespace

// =============================================================================
//  매칭 본체 — 가설 생성(local) → 가설 검증(global)
//
//  [핵심 로직 해설] 2단계 구조인 이유:
//   1) 국소구조만으로 판정하면 융선 패턴이 우연히 닮은 타인쌍에 취약하다.
//   2) 전역 좌표만으로 판정하려면 정렬(회전/이동)을 알아야 하는데 그걸 모른다.
//  → 국소구조로 "정렬 후보"를 싸게 만들고, 각 후보가 함의하는 강체변환을
//    전체 미뉴셔에 적용해 몇 개가 맞아떨어지는지 투표시킨다(RANSAC 사고방식).
//
//  강체변환 가설: 후보쌍 (p, g)가 진짜 대응이라면
//      회전 R = g.theta - p.theta
//      이동 T = g.pos - R(p.pos)
//  이 변환을 probe 전체에 적용해 gallery와 겹치는 개수를 센다.
//
//  스코어 정규화: inliers^2 / (n_p * n_g)
//  단순히 inliers/min(n_p,n_g)를 쓰면 미뉴셔가 적은 저품질 템플릿이
//  분모가 작아져 부당하게 높은 점수를 받는다. 제곱/곱 형태는
//  양쪽 모두 충분한 미뉴셔를 가질 때만 높은 값이 나오도록 억제한다.
// =============================================================================
MatchResult MinutiaMatcher::match(const Template& probe,
                                  const Template& gallery) const {
    MatchResult best;
    const auto& P = probe.minutiae;
    const auto& G = gallery.minutiae;
    if (P.empty() || G.empty()) return best;

    const auto dp = build_descriptors(P, cfg_.neighbors);
    const auto dg = build_descriptors(G, cfg_.neighbors);

    // ---- 1단계: 국소구조 유사도 상위 후보쌍 수집 ----
    struct Cand { double sim; std::size_t i, j; };
    std::vector<Cand> cands;
    cands.reserve(P.size() * G.size() / 4 + 1);
    for (std::size_t i = 0; i < P.size(); ++i) {
        for (std::size_t j = 0; j < G.size(); ++j) {
            const double s = descriptor_similarity(dp[i], dg[j], cfg_);
            if (s > 0.0) cands.push_back({s, i, j});
        }
    }
    if (cands.empty()) return best;

    // 상위 max_hypotheses개만 검증 → 최악 연산량을 상한으로 묶는다.
    const std::size_t keep = std::min(cfg_.max_hypotheses, cands.size());
    std::partial_sort(cands.begin(), cands.begin() + keep, cands.end(),
                      [](const Cand& a, const Cand& b) { return a.sim > b.sim; });
    cands.resize(keep);

    // ---- 2단계: 각 후보가 정의하는 강체변환을 전역 검증 ----
    for (const auto& c : cands) {
        const Minutia& p = P[c.i];
        const Minutia& g = G[c.j];

        const double rot = wrap_pi(g.theta - p.theta);
        const double cs = std::cos(rot), sn = std::sin(rot);
        // p를 원점 기준 rot만큼 돌린 뒤 g 위치로 보내는 평행이동
        const double tx = g.x - (p.x * cs - p.y * sn);
        const double ty = g.y - (p.x * sn + p.y * cs);

        int inliers = 0;
        std::vector<char> used(G.size(), 0);   // 갤러리 미뉴셔 1:1 배타 매칭
        for (const auto& pm : P) {
            const double X = pm.x * cs - pm.y * sn + tx;
            const double Y = pm.x * sn + pm.y * cs + ty;
            const double TH = wrap_pi(pm.theta + rot);

            double best_d2 = cfg_.inlier_radius * cfg_.inlier_radius;
            long   best_j  = -1;
            for (std::size_t j = 0; j < G.size(); ++j) {
                if (used[j]) continue;
                const double dx = G[j].x - X, dy = G[j].y - Y;
                const double d2 = dx * dx + dy * dy;
                if (d2 > best_d2) continue;
                if (std::fabs(wrap_pi(G[j].theta - TH)) > cfg_.inlier_angle_tol) continue;
                best_d2 = d2;
                best_j  = static_cast<long>(j);
            }
            if (best_j >= 0) { used[static_cast<std::size_t>(best_j)] = 1; ++inliers; }
        }

        if (inliers > best.inliers) {
            best.inliers  = inliers;
            best.rotation = rot;
            best.tx = tx;
            best.ty = ty;
        }
    }

    if (best.inliers < cfg_.min_inliers) { best.score = 0.0; return best; }

    const double np = static_cast<double>(P.size());
    const double ng = static_cast<double>(G.size());
    const double s  = static_cast<double>(best.inliers) * static_cast<double>(best.inliers)
                      / (np * ng);
    best.score = std::min(1.0, s);
    return best;
}

std::vector<double> MinutiaMatcher::identify(
    const Template& probe, const std::vector<Template>& gallery) const {
    std::vector<double> scores;
    scores.reserve(gallery.size());
    for (const auto& g : gallery) scores.push_back(match(probe, g).score);
    return scores;
}

}  // namespace cfp
