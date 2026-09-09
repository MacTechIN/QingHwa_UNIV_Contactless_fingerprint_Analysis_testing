// =============================================================================
//  합성 지문 생성기 — 테스트를 데이터셋 없이 자족적으로 만들기 위한 도구
//
//  PolyU 같은 실데이터는 승인·반출 제약이 있어 CI에 넣을 수 없다.
//  대신 융선의 본질적 성질(국소적으로 평행한 주기 무늬 + 코어 중심의 소용돌이)을
//  수식으로 합성해 파이프라인 전 구간을 검증한다.
//  "실제 지문에서 잘 도는가"는 증명하지 못하지만
//  "기하/주파수 가정이 코드에 올바로 구현됐는가"는 증명할 수 있다.
// =============================================================================
#pragma once
#include <cmath>
#include <opencv2/imgproc.hpp>
#include <vector>

namespace synth {

// seed 별로 다른 지문 패턴. period = 융선 주기(px).
inline cv::Mat make_finger(int w, int h, int seed, double period = 8.0,
                           double rotate_deg = 0.0, cv::Point2f shift = {0, 0}) {
    cv::Mat bgr(h, w, CV_8UC3, cv::Scalar(18, 20, 22));   // 어두운 배경

    cv::RNG rng(static_cast<uint64_t>(seed) * 7919u + 13u);
    // 코어(소용돌이 중심)와 삼각주(delta) 위치를 seed로 흔들어 개인차를 만든다.
    const cv::Point2f core (w * 0.5f + rng.uniform(-8.f, 8.f),
                            h * 0.45f + rng.uniform(-10.f, 10.f));
    const cv::Point2f delta(w * 0.5f + rng.uniform(-14.f, 14.f),
                            h * 0.80f + rng.uniform(-10.f, 10.f));
    const double twist = rng.uniform(0.6, 1.5);

    // ---- 위상 특이점(dislocation) 주입 ----
    // [핵심] 순수한 cos(위상) 무늬는 융선이 끊기지 않으므로 미뉴셔가 코어/삼각주에만
    // 생긴다. 실제 지문에는 융선이 시작·분기하는 지점이 수십 개 있다.
    // 위상장에 ±atan2 항을 더하면 그 점을 한 바퀴 돌 때 위상이 2pi만큼 어긋나
    // 융선 하나가 끼어들거나 사라진다 — 이것이 곧 끝점/분기점이다.
    // (SFinGe 계열 합성 지문 생성기가 쓰는 것과 같은 원리)
    // 지두 ROI(손끝 쪽 약 40%)에 충분한 수가 들어가도록 위쪽에 더 촘촘히 배치한다.
    const int n_disloc = 40;
    std::vector<cv::Point2f> dis; dis.reserve(n_disloc);
    std::vector<double>      sgn; sgn.reserve(n_disloc);
    for (int i = 0; i < n_disloc; ++i) {
        dis.emplace_back(rng.uniform(w * 0.24f, w * 0.76f),
                         rng.uniform(h * 0.10f, h * 0.62f));
        sgn.push_back(rng.uniform(0, 2) ? 1.0 : -1.0);
    }

    const double rad = rotate_deg * CV_PI / 180.0;
    const double cs = std::cos(rad), sn = std::sin(rad);
    const cv::Point2f ctr(w * 0.5f, h * 0.5f);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // 회전/이동을 역변환해 원본 무늬 좌표를 얻는다(리샘플 대신 해석적 생성)
            const double dx = x - ctr.x - shift.x, dy = y - ctr.y - shift.y;
            const double ux =  dx * cs + dy * sn + ctr.x;
            const double uy = -dx * sn + dy * cs + ctr.y;

            // ---- 손가락 실루엣 ----
            // [중요] 대칭 타원으로 만들면 안 된다.
            // 세그멘테이션은 "손가락 끝이 밑동보다 좁다"는 해부학적 사실로
            // 위/아래(180도 모호성)를 판별하므로, 대칭 도형은 그 판별을 동전던지기로
            // 만들어 실제 사진에서는 드러나는 결함을 테스트가 놓치게 한다.
            // 실제 지두 촬영본처럼 끝은 둥글고 좁게, 아래로 갈수록 넓게 만든다.
            const double y_top = h * 0.06;
            const double flen  = h * 0.90;
            const double v = (uy - y_top) / flen;          // 0 = 손끝, 1 = 프레임 하단
            if (v < 0.0 || v > 1.0) continue;
            double hw = (w * 0.36) * (0.60 + 0.40 * std::min(1.0, v / 0.72));
            if (v < 0.22) {                                 // 끝을 둥글게 깎는다
                const double t = (0.22 - v) / 0.22;
                hw *= std::sqrt(std::max(0.0, 1.0 - t * t));
            }
            if (std::fabs(ux - w * 0.5) > hw) continue;
            // 주변부 감광 계산용 정규화 좌표
            const double ex = (ux - w * 0.5) / (w * 0.36);
            const double ey = v * 2.0 - 1.0;

            // 위상장: 코어 주변 소용돌이 + 삼각주 방향 성분
            const double r1 = std::hypot(ux - core.x,  uy - core.y);
            const double a1 = std::atan2(uy - core.y,  ux - core.x);
            const double r2 = std::hypot(ux - delta.x, uy - delta.y);
            double phase = (r1 + twist * a1 * period * 0.5 + 0.35 * r2)
                           * (2.0 * CV_PI / period);
            // 각 특이점은 위상에 ±1회전을 더한다 → 그 지점에서 융선이 분기/종료된다.
            for (std::size_t k = 0; k < dis.size(); ++k)
                phase += sgn[k] * std::atan2(uy - dis[k].y, ux - dis[k].x);

            const double ridge = 0.5 * (1.0 + std::cos(phase));   // 0..1
            // 비접촉 지문 극성: 융선이 밝고 골이 어둡다 (research.md §3)
            const double skin  = 120.0 + 70.0 * ridge;
            const double vign  = 1.0 - 0.25 * (ex * ex + ey * ey);  // 주변부 감광

            auto& p = bgr.at<cv::Vec3b>(y, x);
            // 피부색 비율 (BGR) — YCbCr 세그멘테이션이 잡을 수 있는 톤
            p[0] = cv::saturate_cast<uchar>(skin * vign * 0.62);
            p[1] = cv::saturate_cast<uchar>(skin * vign * 0.78);
            p[2] = cv::saturate_cast<uchar>(skin * vign * 1.00);
        }
    }
    cv::GaussianBlur(bgr, bgr, {3, 3}, 0);
    return bgr;
}

}  // namespace synth
