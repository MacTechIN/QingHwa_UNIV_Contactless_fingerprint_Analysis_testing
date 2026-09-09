// 파이프라인 통합 테스트 — 합성 지문으로 전 구간을 검증한다.
#include "cfp/enhancer.hpp"
#include "cfp/matcher.hpp"
#include "cfp/minutiae.hpp"
#include "cfp/pipeline.hpp"
#include "cfp/quality.hpp"
#include "cfp/segmenter.hpp"
#include "synthetic.hpp"
#include "test_util.hpp"

#include <memory>

using namespace cfp;

int main() {
    std::printf("== pipeline ==\n");

    auto pipe = Pipeline(std::make_shared<SimpleQualityGate>(),
                         std::make_shared<YCbCrSegmenter>(),
                         std::make_shared<GaborRidgeEnhancer>(),
                         std::make_shared<CrossingNumberExtractor>());

    // --- 1) 합성 지문 전 구간 통과 ---
    const cv::Mat a = synth::make_finger(420, 560, 7);
    const auto ra = pipe.run(a, /*enforce_quality_gate=*/false);
    tu::check(ra.segment.ok, "세그멘테이션 성공 (" + ra.segment.reason + ")");
    tu::check(!ra.enhance.skeleton.empty(), "세선화 산출물 존재");
    tu::check(ra.minutiae.size() >= 10,
              "미뉴셔 10개 이상 검출 (실제 " + std::to_string(ra.minutiae.size()) + ")");

    // --- 2) 방향장 값역 검증 ---
    // 융선 방향은 pi 주기이므로 [0, pi) 밖의 값이 나오면 구현 오류다.
    if (!ra.enhance.orientation.empty()) {
        double lo = 1e9, hi = -1e9;
        for (int y = 0; y < ra.enhance.orientation.rows; ++y)
            for (int x = 0; x < ra.enhance.orientation.cols; ++x) {
                const double v = ra.enhance.orientation.at<float>(y, x);
                lo = std::min(lo, v); hi = std::max(hi, v);
            }
        tu::check(lo >= 0.0 && hi < CV_PI + 1e-4,
                  "방향장이 [0, pi) 범위 내 (관측 " + std::to_string(lo) +
                  " ~ " + std::to_string(hi) + ")");
    }

    // --- 3) 융선 주파수가 생리학적 범위 안 ---
    if (!ra.enhance.frequency.empty() && !ra.enhance.mask.empty()) {
        cv::Mat valid = (ra.enhance.frequency > 0) & (ra.enhance.mask > 0);
        if (cv::countNonZero(valid) > 0) {
            const double mf = cv::mean(ra.enhance.frequency, valid)[0];
            const double period = mf > 0 ? 1.0 / mf : 0.0;
            tu::check(period >= 3.0 && period <= 25.0,
                      "추정 융선 주기가 3~25px (실제 " + std::to_string(period) + ")");
        }
    }

    // --- 4) 미뉴셔 방향이 [0, 2pi) ---
    {
        bool ok = true;
        for (const auto& m : ra.minutiae)
            if (!(m.theta >= 0.0f && m.theta < 2 * CV_PI + 1e-3)) ok = false;
        tu::check(ok, "모든 미뉴셔 방향이 [0, 2pi) 범위");
    }

    // --- 5) 본인쌍(회전+이동) vs 타인쌍 분리 ---
    {
        const cv::Mat a2 = synth::make_finger(420, 560, 7, 8.0, 10.0, {5.f, -3.f});
        const cv::Mat b  = synth::make_finger(420, 560, 31);
        const auto r2 = pipe.run(a2, false);
        const auto rb = pipe.run(b,  false);

        if (ra.ok && r2.ok && rb.ok) {
            MinutiaMatcher m;
            const auto ta = Pipeline::to_template(ra, "A", "1");
            const auto t2 = Pipeline::to_template(r2, "A", "2");
            const auto tb = Pipeline::to_template(rb, "B", "1");
            const double gen = m.match(ta, t2).score;
            const double imp = m.match(ta, tb).score;
            std::printf("       genuine=%.4f  impostor=%.4f\n", gen, imp);
            tu::check(gen > imp, "본인쌍 스코어 > 타인쌍 스코어");
        } else {
            tu::check(false, "본인/타인 비교용 파이프라인 실행 실패");
        }
    }

    // --- 6) 실패 경로: 배경만 있는 프레임 ---
    {
        const cv::Mat blank(300, 300, CV_8UC3, cv::Scalar(15, 15, 15));
        const auto r = pipe.run(blank, false);
        tu::check(!r.ok && !r.failure_stage.empty(),
                  "빈 프레임은 예외가 아니라 실패 상태로 보고 (" + r.failure_stage + ")");
    }

    return tu::summary("pipeline");
}
