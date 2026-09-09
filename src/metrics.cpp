#include "cfp/metrics.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace cfp::metrics {

// =============================================================================
//  DET 곡선 계산
//
//  [핵심 로직 해설]
//  임계값 t를 스윕하며 두 오류율을 잰다.
//      FMR(t)  = |{impostor >= t}| / N_impostor      (타인을 본인이라 함)
//      FNMR(t) = |{genuine  <  t}| / N_genuine       (본인을 타인이라 함)
//
//  나이브 구현은 t마다 전체 배열을 다시 세어 O(T*N)이 된다.
//  여기서는 양쪽을 미리 정렬해 두고 t를 오름차순으로 올리면서
//  포인터를 전진만 시키므로 O(N log N + N) 이다.
//  N = 100만 쌍(1000명 x 1000명) 규모에서 이 차이가 실행 가능/불가능을 가른다.
//
//  임계값 후보는 "실제로 관측된 스코어"만 쓴다. 등간격 격자를 쓰면
//  스코어가 특정 구간에 몰릴 때 EER이 격자 해상도에 갇혀 왜곡된다.
// =============================================================================
std::vector<DetPoint> compute_det(std::vector<double> genuine,
                                  std::vector<double> impostor) {
    std::vector<DetPoint> det;
    if (genuine.empty() || impostor.empty()) return det;

    std::sort(genuine.begin(), genuine.end());
    std::sort(impostor.begin(), impostor.end());

    // 후보 임계값 = 두 분포의 모든 관측값 합집합(정렬·중복제거)
    std::vector<double> th;
    th.reserve(genuine.size() + impostor.size() + 2);
    th.insert(th.end(), genuine.begin(), genuine.end());
    th.insert(th.end(), impostor.begin(), impostor.end());
    std::sort(th.begin(), th.end());
    th.erase(std::unique(th.begin(), th.end()), th.end());

    // 양 끝을 열어둔다: 전부 수락(FMR=1) / 전부 거절(FNMR=1) 지점이 있어야
    // 이후 보간이 항상 교차점을 포함한다.
    const double eps = 1e-9;
    th.insert(th.begin(), th.front() - eps);
    th.push_back(th.back() + eps);

    const double ng = static_cast<double>(genuine.size());
    const double ni = static_cast<double>(impostor.size());

    std::size_t gi = 0, ii = 0;   // 전진 포인터 (되돌아가지 않음)
    det.reserve(th.size());

    for (double t : th) {
        // genuine 중 t 미만 개수 → FNMR
        while (gi < genuine.size() && genuine[gi] < t) ++gi;
        // impostor 중 t 미만 개수 → 나머지가 FMR
        while (ii < impostor.size() && impostor[ii] < t) ++ii;

        DetPoint p;
        p.threshold = t;
        p.fnmr = static_cast<double>(gi) / ng;
        p.fmr  = (ni - static_cast<double>(ii)) / ni;
        det.push_back(p);
    }
    return det;
}

// =============================================================================
//  EER = FMR(t) 와 FNMR(t) 가 교차하는 지점의 오류율
//
//  [핵심 로직 해설]
//  t가 커지면 FMR은 단조 감소, FNMR은 단조 증가한다.
//  따라서 d(t) = FMR(t) - FNMR(t) 는 단조 감소하며 부호를 정확히 한 번 바꾼다.
//  부호가 바뀌는 구간 [i-1, i] 에서 두 직선의 교점을 선형보간한다.
//
//  이산 표본에서 정확히 교차하는 t가 없을 수 있으므로 보간이 필수다.
//  보간 없이 "가장 가까운 점"을 쓰면 표본이 성길 때 EER이 최대 수 %p 튄다.
// =============================================================================
EerResult compute_eer(const std::vector<DetPoint>& det) {
    EerResult r;
    if (det.size() < 2) return r;

    for (std::size_t i = 1; i < det.size(); ++i) {
        const double d0 = det[i - 1].fmr - det[i - 1].fnmr;
        const double d1 = det[i].fmr     - det[i].fnmr;

        if (d0 == 0.0) {
            r.eer = det[i - 1].fmr;
            r.threshold = det[i - 1].threshold;
            return r;
        }
        if ((d0 > 0.0) != (d1 > 0.0)) {          // 부호 전환 구간
            const double w = d0 / (d0 - d1);      // 0..1, 교점의 상대 위치
            const double fmr  = det[i - 1].fmr  + w * (det[i].fmr  - det[i - 1].fmr);
            const double fnmr = det[i - 1].fnmr + w * (det[i].fnmr - det[i - 1].fnmr);
            r.eer = 0.5 * (fmr + fnmr);           // 교점에서 두 값은 같으므로 평균 == 값
            r.threshold = det[i - 1].threshold +
                          w * (det[i].threshold - det[i - 1].threshold);
            return r;
        }
    }
    // 교차가 없다면(분포가 완전분리 등) 두 오류율 차가 최소인 점을 택한다.
    const auto it = std::min_element(
        det.begin(), det.end(), [](const DetPoint& a, const DetPoint& b) {
            return std::fabs(a.fmr - a.fnmr) < std::fabs(b.fmr - b.fnmr);
        });
    r.eer = 0.5 * (it->fmr + it->fnmr);
    r.threshold = it->threshold;
    return r;
}

EerResult compute_eer(std::vector<double> genuine, std::vector<double> impostor) {
    return compute_eer(compute_det(std::move(genuine), std::move(impostor)));
}

// =============================================================================
//  운영점 지표: FMR 상한을 만족하는 가장 낮은 FNMR
//  FMR100 = fnmr_at_fmr(det, 0.01), ZeroFMR = fnmr_at_fmr(det, 0.0)
// =============================================================================
double fnmr_at_fmr(const std::vector<DetPoint>& det, double fmr_target) {
    double best = 1.0;
    bool found = false;
    for (const auto& p : det) {
        if (p.fmr <= fmr_target + 1e-12) {
            best = std::min(best, p.fnmr);
            found = true;
        }
    }
    return found ? best : 1.0;
}

// =============================================================================
//  ROC AUC — Mann-Whitney U 통계량과 동치
//
//  [핵심 로직 해설]
//  AUC = P(무작위 본인쌍 스코어 > 무작위 타인쌍 스코어)
//  이 확률은 두 표본을 합쳐 순위를 매긴 뒤
//      AUC = (R_genuine - n_g(n_g+1)/2) / (n_g * n_i)
//  로 계산된다(R = genuine 순위합). 동점은 평균순위를 부여해 0.5로 처리된다.
//  임계값 스윕 없이 O(N log N)으로 끝나며 표본 밀도에 영향받지 않는다.
// =============================================================================
double roc_auc(const std::vector<double>& genuine,
               const std::vector<double>& impostor) {
    if (genuine.empty() || impostor.empty()) return 0.0;

    struct Item { double v; int is_gen; };
    std::vector<Item> all;
    all.reserve(genuine.size() + impostor.size());
    for (double v : genuine)  all.push_back({v, 1});
    for (double v : impostor) all.push_back({v, 0});
    std::sort(all.begin(), all.end(),
              [](const Item& a, const Item& b) { return a.v < b.v; });

    // 동점 구간은 평균 순위를 공유해야 AUC가 0.5로 수렴한다.
    double rank_sum_gen = 0.0;
    std::size_t i = 0;
    while (i < all.size()) {
        std::size_t j = i;
        while (j + 1 < all.size() && all[j + 1].v == all[i].v) ++j;
        const double avg_rank = 0.5 * (static_cast<double>(i + 1) +
                                       static_cast<double>(j + 1));
        for (std::size_t k = i; k <= j; ++k)
            if (all[k].is_gen) rank_sum_gen += avg_rank;
        i = j + 1;
    }
    const double ng = static_cast<double>(genuine.size());
    const double ni = static_cast<double>(impostor.size());
    return (rank_sum_gen - ng * (ng + 1.0) / 2.0) / (ng * ni);
}

// =============================================================================
//  CMC — 1:N 식별에서 정답이 상위 r위 안에 들어오는 누적 비율
//
//  [핵심 로직 해설]
//  정답의 순위 = "정답보다 스코어가 높은 갤러리 항목 수 + 1".
//  전체 정렬(O(G log G))이 필요 없고 한 번의 스캔(O(G))이면 된다.
//  동점은 보수적으로 처리한다 — 동점자를 앞선 것으로 세어
//  순위를 낮게(불리하게) 잡는다. 그렇지 않으면 모든 스코어가 같은
//  퇴화 매처가 rank-1 100%를 받는 착시가 생긴다.
// =============================================================================
std::vector<double> compute_cmc(const std::vector<std::vector<double>>& score_matrix,
                                const std::vector<std::size_t>& true_index) {
    if (score_matrix.empty() || score_matrix.size() != true_index.size())
        return {};

    const std::size_t gallery_n = score_matrix.front().size();
    std::vector<double> hit(gallery_n, 0.0);

    std::size_t valid = 0;
    for (std::size_t p = 0; p < score_matrix.size(); ++p) {
        const auto& row = score_matrix[p];
        const std::size_t ti = true_index[p];
        if (ti >= row.size() || row.size() != gallery_n) continue;

        const double true_score = row[ti];
        std::size_t rank = 0;   // 0-based
        for (std::size_t g = 0; g < row.size(); ++g) {
            if (g == ti) continue;
            if (row[g] >= true_score) ++rank;   // 동점은 불리하게
        }
        if (rank < gallery_n) hit[rank] += 1.0;
        ++valid;
    }
    if (valid == 0) return {};

    std::vector<double> cmc(gallery_n, 0.0);
    double acc = 0.0;
    for (std::size_t r = 0; r < gallery_n; ++r) {
        acc += hit[r];
        cmc[r] = acc / static_cast<double>(valid);
    }
    return cmc;
}

}  // namespace cfp::metrics
