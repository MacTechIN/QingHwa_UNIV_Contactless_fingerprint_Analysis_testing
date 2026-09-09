// =============================================================================
//  cfp/metrics.hpp — 생체인식 표준 성능 지표 (dev_plan V8 / research.md §6)
//
//  [왜 별도 모듈인가] 원논문 리뷰가 지적한 핵심 결함이
//  "정확도만 보고하고 EER/DET/CMC가 없다"는 것이었다.
//  지표 계산은 영상과 무관한 순수 수치 연산이므로 OpenCV 의존 없이
//  독립 단위테스트가 가능하도록 분리했다.
// =============================================================================
#pragma once

#include <cstddef>
#include <vector>

namespace cfp::metrics {

// FMR(False Match Rate)   : 타인쌍인데 임계값 이상이라 수락 → 오수락률
// FNMR(False Non-Match Rate): 본인쌍인데 임계값 미만이라 거절 → 오거절률
struct DetPoint {
    double threshold = 0.0;
    double fmr       = 0.0;
    double fnmr      = 0.0;
};

struct EerResult {
    double eer       = 0.0;   // FMR == FNMR 지점의 오류율
    double threshold = 0.0;   // 그때의 임계값
};

// 스코어는 "클수록 유사"하다고 가정한다(유사도 규약).
// 거리(작을수록 유사)를 쓰려면 부호를 뒤집어 넣을 것.
std::vector<DetPoint> compute_det(std::vector<double> genuine,
                                  std::vector<double> impostor);

// DET 곡선에서 FMR-FNMR 교차점을 선형보간으로 찾는다.
EerResult compute_eer(const std::vector<DetPoint>& det);
EerResult compute_eer(std::vector<double> genuine, std::vector<double> impostor);

// 운영점 지표: 주어진 FMR 상한에서의 FNMR.
//   fmr_target = 0.01  → FMR100
//   fmr_target = 0.001 → FMR1000
//   fmr_target = 0.0   → ZeroFMR
double fnmr_at_fmr(const std::vector<DetPoint>& det, double fmr_target);

// ROC AUC. 순위 기반(Mann-Whitney U)으로 계산하므로 임계값 스윕이 필요 없다.
double roc_auc(const std::vector<double>& genuine,
               const std::vector<double>& impostor);

// CMC(Cumulative Match Characteristic): 1:N 식별 성능.
// score_matrix[i] = i번째 probe에 대한 갤러리 전체 스코어
// true_index[i]   = i번째 probe의 정답 갤러리 인덱스
// 반환: rank_rate[r] = rank (r+1) 이내 정답 포함 비율
std::vector<double> compute_cmc(const std::vector<std::vector<double>>& score_matrix,
                                const std::vector<std::size_t>& true_index);

}  // namespace cfp::metrics
