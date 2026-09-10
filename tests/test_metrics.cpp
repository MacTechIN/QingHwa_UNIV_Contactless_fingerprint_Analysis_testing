#include "cfp/metrics.hpp"
#include "test_util.hpp"

#include <random>

using namespace cfp::metrics;

int main() {
    std::printf("== metrics ==\n");

    // --- 1) 완전 분리된 분포 → EER 0 ---
    {
        std::vector<double> gen, imp;
        for (int i = 0; i < 100; ++i) { gen.push_back(0.8 + i * 0.001); imp.push_back(0.1 + i * 0.001); }
        auto r = compute_eer(gen, imp);
        tu::close_to(r.eer, 0.0, 1e-9, "완전분리 EER == 0");
        tu::close_to(roc_auc(gen, imp), 1.0, 1e-9, "완전분리 AUC == 1");
    }

    // --- 2) 완전 동일 분포 → EER 0.5, AUC 0.5 ---
    {
        std::vector<double> gen, imp;
        for (int i = 0; i < 200; ++i) { gen.push_back(i * 0.01); imp.push_back(i * 0.01); }
        auto r = compute_eer(gen, imp);
        tu::close_to(r.eer, 0.5, 0.02, "동일분포 EER ~= 0.5");
        tu::close_to(roc_auc(gen, imp), 0.5, 1e-9, "동일분포 AUC == 0.5 (동점 평균순위)");
    }

    // --- 3) 대칭 부분중첩 → 해석적 기대값과 비교 ---
    // gen ~ U(0.4,1.0), imp ~ U(0.0,0.6): 대칭이므로 교차점은 0.5, EER = 1/6
    {
        std::vector<double> gen, imp;
        const int N = 6000;
        for (int i = 0; i < N; ++i) {
            gen.push_back(0.4 + 0.6 * i / (N - 1.0));
            imp.push_back(0.0 + 0.6 * i / (N - 1.0));
        }
        auto r = compute_eer(gen, imp);
        tu::close_to(r.eer, 1.0 / 6.0, 0.005, "대칭중첩 EER == 1/6");
        tu::close_to(r.threshold, 0.5, 0.01, "EER 임계값 == 0.5");
    }

    // --- 4) 운영점 지표 단조성 ---
    {
        std::mt19937 rng(42);
        std::normal_distribution<double> g(0.75, 0.10), i(0.35, 0.10);
        std::vector<double> gen, imp;
        for (int k = 0; k < 4000; ++k) { gen.push_back(g(rng)); imp.push_back(i(rng)); }
        auto det = compute_det(gen, imp);
        const double f100  = fnmr_at_fmr(det, 0.01);
        const double f1000 = fnmr_at_fmr(det, 0.001);
        const double fzero = fnmr_at_fmr(det, 0.0);
        tu::check(f100 <= f1000 + 1e-12, "FMR100 <= FMR1000 (엄격할수록 FNMR 증가)");
        tu::check(f1000 <= fzero + 1e-12, "FMR1000 <= ZeroFMR");
        tu::check(compute_eer(det).eer < 0.05, "가우시안 분리 EER < 5%");
    }

    // --- 5) CMC ---
    {
        // probe 3개, 갤러리 4개. 정답이 각각 1위/2위/1위가 되도록 구성.
        std::vector<std::vector<double>> S = {
            {0.9, 0.1, 0.2, 0.3},   // 정답 0 → 1위
            {0.8, 0.5, 0.2, 0.3},   // 정답 1 → 2위
            {0.1, 0.2, 0.7, 0.3},   // 정답 2 → 1위
        };
        std::vector<std::size_t> truth = {0, 1, 2};
        auto cmc = compute_cmc(S, truth);
        tu::close_to(cmc[0], 2.0 / 3.0, 1e-9, "rank-1 == 2/3");
        tu::close_to(cmc[1], 1.0,       1e-9, "rank-2 == 1.0");
    }

    // --- 6) 퇴화 매처 방어: 모든 스코어 동일 ---
    {
        std::vector<std::vector<double>> S = {{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}};
        std::vector<std::size_t> truth = {0, 1};
        auto cmc = compute_cmc(S, truth);
        tu::check(cmc[0] < 0.5, "전부 동점인 매처는 rank-1을 못 받는다(동점=불리 처리)");
    }

    return tu::summary("metrics");
}
