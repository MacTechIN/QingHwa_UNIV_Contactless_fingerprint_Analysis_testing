#include "cfp/minutiae.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>

#include <opencv2/imgproc.hpp>

namespace cfp {

namespace {
constexpr double kPi = 3.14159265358979323846;

// 8-이웃을 "원형 순서"로 도는 오프셋.
// 순서가 반시계/시계 어느 쪽이든 상관없지만, 반드시 인접한 것끼리 이어져야 한다.
// 이 순서가 깨지면 Crossing Number 공식이 통째로 무의미해진다.
constexpr int kNx[8] = { 1,  1,  0, -1, -1, -1,  0,  1};
constexpr int kNy[8] = { 0, -1, -1, -1,  0,  1,  1,  1};

inline int px(const cv::Mat& skel, int x, int y) {
    if (x < 0 || y < 0 || x >= skel.cols || y >= skel.rows) return 0;
    return skel.at<uchar>(y, x) ? 1 : 0;
}

// ---------------------------------------------------------------------------
//  Crossing Number
//
//  [핵심 로직 해설]
//  스켈레톤 위의 한 점을 중심으로 8-이웃을 원형으로 한 바퀴 돌면서
//  0->1 또는 1->0 으로 바뀌는 횟수를 세고 2로 나눈다.
//      CN = 0.5 * sum_{i=1..8} |P_i - P_{i+1}|,  P_9 = P_1
//  이 값은 "그 점에서 뻗어나가는 융선 가지의 개수"와 같다.
//      CN = 1 -> 가지 하나  = 융선 끝점(ridge ending)
//      CN = 2 -> 가지 둘    = 그냥 지나가는 융선 (미뉴셔 아님)
//      CN = 3 -> 가지 셋    = 분기점(bifurcation)
//      CN >= 4 -> 교차점. 정상 지문에는 없으며 대개 세선화 잔재 -> 버린다.
//  전제: 입력이 정확히 1픽셀 두께 스켈레톤이어야 한다.
//        두꺼우면 CN=2가 아닌 값이 대량 발생해 위양성이 폭증한다.
// ---------------------------------------------------------------------------
inline int crossing_number(const cv::Mat& skel, int x, int y) {
    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        const int a = px(skel, x + kNx[i],           y + kNy[i]);
        const int b = px(skel, x + kNx[(i + 1) % 8], y + kNy[(i + 1) % 8]);
        sum += std::abs(a - b);
    }
    return sum / 2;
}

// ---------------------------------------------------------------------------
//  스켈레톤 추적으로 가지 방향 구하기
//
//  [핵심 로직 해설]
//  미뉴셔 방향은 "융선이 어디로 뻗어가는가"이므로,
//  시작점에서 스켈레톤을 따라 max_len 픽셀만큼 걸어간 뒤
//  시작점 -> 도착점 벡터의 각도를 취한다.
//
//  한 픽셀 이웃만 보고 각도를 재면 8방향(45도 간격)으로 양자화되어 버린다.
//  여러 픽셀 걸어가면 그 사이 곡률이 평균화되어 연속적인 각도가 나온다.
//  걸음 수가 너무 길면 융선 곡률 때문에 방향이 왜곡되므로 10~15px가 적당하다.
//
//  되돌아감 방지를 위해 직전 픽셀을 기억한다(prev). 이게 없으면
//  두 점 사이를 무한 왕복한다.
// ---------------------------------------------------------------------------
struct Walk { double angle; int length; };

Walk walk_branch(const cv::Mat& skel, cv::Point start, cv::Point first, int max_len) {
    cv::Point prev = start, cur = first;
    int steps = 1;
    while (steps < max_len) {
        cv::Point next(-1, -1);
        for (int i = 0; i < 8; ++i) {
            const int nx = cur.x + kNx[i], ny = cur.y + kNy[i];
            if (!px(skel, nx, ny)) continue;
            if (nx == prev.x && ny == prev.y) continue;
            if (nx == start.x && ny == start.y) continue;
            next = {nx, ny};
            break;                     // 첫 유효 이웃 채택(분기 만나면 임의 선택)
        }
        if (next.x < 0) break;         // 막다른 길
        prev = cur;
        cur  = next;
        ++steps;
    }
    const double dx = cur.x - start.x, dy = cur.y - start.y;
    return {std::atan2(dy, dx), steps};
}
}  // namespace

// =============================================================================
//  미뉴셔 추출 본체
// =============================================================================
std::vector<Minutia> CrossingNumberExtractor::extract(const EnhanceResult& enh) const {
    std::vector<Minutia> out;
    const cv::Mat& skel = enh.skeleton;
    if (skel.empty()) return out;

    // 마스크를 안쪽으로 깎는다.
    // ROI 경계에서는 융선이 "잘려서" 끝난 것이므로 진짜 끝점이 아니다.
    // 이 처리를 빼면 경계를 따라 위양성 끝점이 줄줄이 생긴다 — 실무에서 가장 흔한 실수.
    cv::Mat inner;
    if (!enh.mask.empty()) {
        const int m = std::max(1, cfg_.border_margin);
        cv::erode(enh.mask, inner,
                  cv::getStructuringElement(cv::MORPH_ELLIPSE, {2 * m + 1, 2 * m + 1}));
    } else {
        inner = cv::Mat(skel.size(), CV_8UC1, cv::Scalar(255));
        cv::rectangle(inner, {0, 0, inner.cols, inner.rows}, cv::Scalar(0),
                      cfg_.border_margin);
    }

    // --- 1) CN 스캔 ---
    std::vector<Minutia> cand;
    for (int y = 1; y < skel.rows - 1; ++y) {
        for (int x = 1; x < skel.cols - 1; ++x) {
            if (!skel.at<uchar>(y, x))  continue;
            if (!inner.at<uchar>(y, x)) continue;

            const int cn = crossing_number(skel, x, y);
            if (cn != 1 && cn != 3) continue;           // 2는 통과 융선, 4+는 잡재

            // --- 2) 방향 산출 ---
            std::vector<Walk> walks;
            for (int i = 0; i < 8; ++i) {
                const int nx = x + kNx[i], ny = y + kNy[i];
                if (px(skel, nx, ny))
                    walks.push_back(walk_branch(skel, {x, y}, {nx, ny}, cfg_.walk_length));
            }
            if (walks.empty()) continue;

            double theta = 0.0;
            if (cn == 1) {
                // 끝점: 가지가 하나뿐. 짧은 가지는 세선화 스퍼(spur)이므로 버린다.
                if (walks[0].length < cfg_.min_branch_len) continue;
                theta = walks[0].angle;
            } else {
                // 분기점: 세 가지 중 "줄기(stem)"의 방향을 쓴다.
                //
                // [핵심 로직 해설] 분기점에서 두 가지는 갈라져 나가고 하나는 들어온다.
                // 들어오는 줄기는 나머지 둘이 이루는 평균 방향의 정반대에 가깝다.
                // 따라서 각 가지에 대해 "나머지 둘의 평균 방향과의 각도 차"를 재고,
                // 그 차이가 가장 큰(=가장 반대인) 가지를 줄기로 택한다.
                // 이렇게 해야 같은 분기점을 다시 촬영해도 방향이 재현된다.
                if (walks.size() < 3) continue;
                double best_diff = -1.0;
                for (std::size_t i = 0; i < walks.size(); ++i) {
                    double sx = 0.0, sy = 0.0;
                    for (std::size_t j = 0; j < walks.size(); ++j) {
                        if (i == j) continue;
                        sx += std::cos(walks[j].angle);
                        sy += std::sin(walks[j].angle);
                    }
                    const double other = std::atan2(sy, sx);
                    double d = std::fabs(walks[i].angle - other);
                    while (d > kPi) d = 2.0 * kPi - d;      // (-pi, pi]로 접기
                    if (d > best_diff) { best_diff = d; theta = walks[i].angle; }
                }
            }

            Minutia m;
            m.x = static_cast<float>(x);
            m.y = static_cast<float>(y);
            m.theta = static_cast<float>(theta < 0 ? theta + 2 * kPi : theta);
            m.type = (cn == 1) ? MinutiaType::RidgeEnding : MinutiaType::Bifurcation;

            // --- 품질 = 국소 방향장 일관성 ---
            //
            // [핵심 로직 해설] 예전에는 Gabor 응답의 밝기 평균을 품질로 썼다.
            // 밝기는 "여기 무언가 있다"는 것만 말해줄 뿐, 그것이 융선인지
            // 음영 경계인지 구분하지 못한다. 그래서 위양성 미뉴셔가 상위 품질로
            // 올라와 템플릿을 오염시켰다(실제 촬영본에서 미뉴셔 120개 중 절반가량).
            //
            // 일관성(coherence)은 "융선이 국소적으로 나란한가"를 직접 재므로
            // 미뉴셔 신뢰도의 대용치로 훨씬 정확하다. 낮은 곳의 미뉴셔는
            // 대개 잡음이 만든 것이라 재촬영 시 재현되지 않는다.
            if (!enh.coherence.empty()) {
                cv::Rect w(x - 5, y - 5, 11, 11);
                w &= cv::Rect(0, 0, enh.coherence.cols, enh.coherence.rows);
                m.quality = w.area() > 0
                    ? static_cast<float>(cv::mean(enh.coherence(w))[0])
                    : 0.0f;
            } else if (!enh.enhanced.empty()) {
                cv::Rect w(x - 4, y - 4, 9, 9);
                w &= cv::Rect(0, 0, enh.enhanced.cols, enh.enhanced.rows);
                m.quality = w.area() > 0
                    ? static_cast<float>(cv::mean(enh.enhanced(w))[0] / 255.0)
                    : 0.0f;
            }
            // 일관성이 낮은 곳의 미뉴셔는 재현되지 않으므로 애초에 버린다.
            if (!enh.coherence.empty() && m.quality < cfg_.min_quality) continue;
            cand.push_back(m);
        }
    }

    // --- 3) 근접쌍 제거 ---
    // 세선화가 만드는 스퍼/브릿지는 짧은 거리에 끝점+분기점 쌍을 만든다.
    // 실제 지문에서 미뉴셔가 8px 이내로 붙는 일은 드물므로 둘 다 버린다.
    // (한쪽만 남기면 어느 쪽이 진짜인지 알 수 없어 오히려 노이즈가 된다)
    std::vector<char> drop(cand.size(), 0);
    for (std::size_t i = 0; i < cand.size(); ++i) {
        for (std::size_t j = i + 1; j < cand.size(); ++j) {
            const double dx = cand[i].x - cand[j].x;
            const double dy = cand[i].y - cand[j].y;
            if (dx * dx + dy * dy < cfg_.min_pair_dist * cfg_.min_pair_dist) {
                drop[i] = drop[j] = 1;
            }
        }
    }
    for (std::size_t i = 0; i < cand.size(); ++i)
        if (!drop[i]) out.push_back(cand[i]);

    // --- 4) 품질 상위 N개만 유지 ---
    // 템플릿 크기를 제한해 매칭 연산량(O(n_p * n_g))을 예측 가능하게 만든다.
    if (static_cast<int>(out.size()) > cfg_.max_minutiae) {
        std::partial_sort(out.begin(), out.begin() + cfg_.max_minutiae, out.end(),
                          [](const Minutia& a, const Minutia& b) {
                              return a.quality > b.quality;
                          });
        out.resize(static_cast<std::size_t>(cfg_.max_minutiae));
    }
    return out;
}

}  // namespace cfp
