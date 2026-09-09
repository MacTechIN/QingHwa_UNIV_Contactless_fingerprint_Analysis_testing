// =============================================================================
//  cfp_cli — 파이프라인 실행기 (플랫폼 독립)
//
//  Windows GUI 앱(app/)과 동일한 Core를 사용하되, 카메라 없이 파일/합성 입력으로
//  전 구간을 돌리고 논문 대조 보고서(HTML+PDF)를 생성한다.
//  CI와 발표 준비 양쪽에서 쓰인다.
//
//  사용법:
//    cfp_cli --demo                     합성 지문으로 데모 실행
//    cfp_cli --image a.png [--match b.png]
//    cfp_cli --out <dir>                보고서 출력 폴더 (기본: ./out)
// =============================================================================
#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <fstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "cfp/enhancer.hpp"
#include "cfp/matcher.hpp"
#include "cfp/minutiae.hpp"
#include "cfp/narrate_stages.hpp"
#include "cfp/pipeline.hpp"
#include "cfp/quality.hpp"
#include "cfp/report.hpp"
#include "cfp/segmenter.hpp"
#include "../tests/synthetic.hpp"

using namespace cfp;

namespace {

// 미뉴셔를 ROI 위에 겹쳐 그린다. 발표 자료에서 가장 설득력 있는 그림.
cv::Mat draw_minutiae(const cv::Mat& base_gray, const std::vector<Minutia>& ms) {
    cv::Mat vis;
    cv::cvtColor(base_gray, vis, cv::COLOR_GRAY2BGR);
    for (const auto& m : ms) {
        // 끝점은 초록, 분기점은 주황 — 색으로 CN=1 / CN=3을 구분해 보여준다.
        const cv::Scalar c = (m.type == MinutiaType::RidgeEnding)
                             ? cv::Scalar(80, 220, 90) : cv::Scalar(60, 165, 245);
        const cv::Point p(static_cast<int>(m.x), static_cast<int>(m.y));
        cv::circle(vis, p, 3, c, 1, cv::LINE_AA);
        const cv::Point q(static_cast<int>(m.x + 11 * std::cos(m.theta)),
                          static_cast<int>(m.y + 11 * std::sin(m.theta)));
        cv::line(vis, p, q, c, 1, cv::LINE_AA);
    }
    return vis;
}

double ms_of(const PipelineOutput& o, const char* stage) {
    for (const auto& t : o.timings) if (t.stage == stage) return t.ms;
    return 0.0;
}

}  // namespace

int main(int argc, char** argv) {
    std::string image_path, match_path, out_dir = "out", dump_dir;
    bool demo = false, no_pdf = false;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--demo") demo = true;
        else if (a == "--image" && i + 1 < argc) image_path = argv[++i];
        else if (a == "--match" && i + 1 < argc) match_path = argv[++i];
        else if (a == "--out"   && i + 1 < argc) out_dir    = argv[++i];
        else if (a == "--dump"  && i + 1 < argc) dump_dir   = argv[++i];
        else if (a == "--no-pdf") no_pdf = true;
        else if (a == "--help") {
            std::puts("cfp_cli [--demo | --image <f> [--match <f>]] "
                      "[--out <dir>] [--dump <dir>] [--no-pdf]");
            return 0;
        }
    }
    if (!demo && image_path.empty()) demo = true;

    // ---- DI 조립: 여기가 절편을 갈아끼우는 유일한 지점 ----
    auto quality   = std::make_shared<SimpleQualityGate>();
    auto segmenter = std::make_shared<YCbCrSegmenter>();
    auto enhancer  = std::make_shared<GaborRidgeEnhancer>();
    auto extractor = std::make_shared<CrossingNumberExtractor>();
    Pipeline pipe(quality, segmenter, enhancer, extractor);

    // ---- 입력 ----
    cv::Mat input;
    std::string source_name;
    const auto t_cap0 = std::chrono::steady_clock::now();
    if (demo) {
        input = synth::make_finger(420, 560, /*seed=*/7);
        source_name = "합성 지문 (seed=7)";
    } else {
        input = cv::imread(image_path, cv::IMREAD_COLOR);
        source_name = image_path;
        if (input.empty()) { std::fprintf(stderr, "이미지를 열 수 없습니다: %s\n",
                                          image_path.c_str()); return 1; }
    }
    const double cap_ms = std::chrono::duration<double, std::milli>(
        std::chrono::steady_clock::now() - t_cap0).count();

    // ---- 실행 ----
    // 데모/평가에서는 품질 게이트를 강제하지 않는다.
    // 게이트에서 멈추면 이후 스택의 논문 대조를 보여줄 수 없기 때문.
    const PipelineOutput out = pipe.run(input, /*enforce_quality_gate=*/false);

    // ---- 스택별 논문 대조 해설 수집 ----
    int idx = 1;
    std::vector<StageNarration> ns;
    ns.push_back(narrate::capture(input, cap_ms, idx++));
    ns.push_back(narrate::quality(out.quality, ms_of(out, "quality"), idx++));
    ns.push_back(narrate::segment(out.segment, ms_of(out, "segment"), idx++));
    if (out.segment.ok) {
        // enhance 단계 내부 4개 스택은 시간을 균등 배분해 표기한다(개별 계측은 V4 백로그).
        const double e = ms_of(out, "enhance") / 4.0;
        ns.push_back(narrate::normalize  (out.enhance, e, idx++));
        ns.push_back(narrate::orientation(out.enhance, e, idx++));
        ns.push_back(narrate::frequency  (out.enhance, e, idx++));
        ns.push_back(narrate::gabor      (out.enhance, e, idx++));
        ns.push_back(narrate::minutiae(out.minutiae, out.enhance,
                                       ms_of(out, "minutiae"), idx++));
    }

    // ---- 선택: 두 번째 영상과 매칭 ----
    report::ReportInput ri;
    if (demo || !match_path.empty()) {
        cv::Mat second = match_path.empty()
            // 같은 손가락을 12도 회전 + 이동해 재촬영한 상황을 모사
            ? synth::make_finger(420, 560, 7, 8.0, 12.0, {6.f, -4.f})
            : cv::imread(match_path, cv::IMREAD_COLOR);
        if (!second.empty()) {
            const PipelineOutput o2 = pipe.run(second, false);
            if (out.ok && o2.ok) {
                const Template a = Pipeline::to_template(out, "A", "1");
                const Template b = Pipeline::to_template(o2,  "A", "2");
                const auto t0 = std::chrono::steady_clock::now();
                const MatchResult mr = MinutiaMatcher{}.match(a, b);
                const double mms = std::chrono::duration<double, std::milli>(
                    std::chrono::steady_clock::now() - t0).count();
                ns.push_back(narrate::matching(mr.score, mr.inliers,
                                               mr.rotation * 180.0 / CV_PI,
                                               a.minutiae.size(), b.minutiae.size(),
                                               mms, idx++));
                ri.has_match = true;
                ri.match_score = mr.score;
                ri.match_inliers = mr.inliers;
            }
        }
    }

    // ---- 콘솔 출력: 단계별 해설 ----
    std::printf("\n===== 비접촉 지문 파이프라인 실행 =====\n입력: %s\n",
                source_name.c_str());
    for (const auto& n : ns) std::fputs(render_text(n).c_str(), stdout);

    // ---- 보고서 ----
    ri.subtitle = "각 스택이 구현한 논문 내용과 이번 실행의 실측 결과를 대조한 자동 생성 문서";
    ri.source_name = source_name;
    ri.narrations = ns;
    ri.timings = out.timings;

    if (!out.segment.roi_gray.empty())
        ri.panels.push_back({"① ROI 정규화", "YCbCr 분할 + 주축 역회전 + 고정크기 리샘플 결과",
                             out.segment.roi_gray});
    if (!out.enhance.normalized.empty())
        ri.panels.push_back({"② CLAHE + 표준화", "조명 불균일 제거 (리뷰 §4 Enhancement)",
                             out.enhance.normalized});
    if (!out.enhance.mask.empty())
        ri.panels.push_back({"③ 유효영역 마스크", "국소분산 기준 융선 구조 존재 영역",
                             out.enhance.mask});
    if (!out.enhance.enhanced.empty())
        ri.panels.push_back({"④ Gabor 향상", "방향·주파수 적응 필터 응답",
                             out.enhance.enhanced});
    if (!out.enhance.binary.empty())
        ri.panels.push_back({"⑤ 이진 융선", "Gabor 응답 부호 기준 이진화",
                             out.enhance.binary});
    if (!out.enhance.skeleton.empty())
        ri.panels.push_back({"⑥ 세선화(Zhang-Suen)", "Crossing Number의 전제인 1픽셀 골격",
                             out.enhance.skeleton});
    if (!out.minutiae.empty() && !out.enhance.enhanced.empty())
        ri.panels.push_back({"⑦ 미뉴셔 검출",
                             "초록=끝점(CN=1), 주황=분기점(CN=3), 선분=방향",
                             draw_minutiae(out.enhance.enhanced, out.minutiae)});

    {
        std::string s = "총 " + std::to_string(ns.size()) + "개 스택 실행. ";
        s += out.ok ? ("미뉴셔 " + std::to_string(out.minutiae.size()) + "개 추출 성공. ")
                    : ("파이프라인 중단: " + out.failure_stage + ". ");
        s += "본 실행은 Chowdhury & Imtiaz(2022) 리뷰가 정리한 고전 경로(YCbCr 분할 → "
             "CLAHE → 방향장 → 주파수 → Gabor → 세선화 → Crossing Number)를 구현한 것으로, "
             "같은 리뷰 §5가 보고한 딥러닝 경로(ContactlessMinuNet 94.10%, Siamese EER 7.93%)의 "
             "기준선에 해당한다.";
        ri.summary = s;
    }

    // 단계별 중간 산출물을 PNG로 덤프 (디버깅/발표 소재용)
    if (!dump_dir.empty()) {
        std::error_code ec; std::filesystem::create_directories(dump_dir, ec);
        auto w = [&](const char* name, const cv::Mat& m) {
            if (m.empty()) return;
            cv::Mat o = m;
            if (o.type() == CV_32F)
                cv::normalize(o, o, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            cv::imwrite(dump_dir + "/" + name, o);
        };
        w("00_input.png",      input);
        w("01_mask.png",       out.segment.mask);
        w("02_roi.png",        out.segment.roi_gray);
        w("03_normalized.png", out.enhance.normalized);
        w("04_region_mask.png",out.enhance.mask);
        w("05_enhanced.png",   out.enhance.enhanced);
        w("06_binary.png",     out.enhance.binary);
        w("07_skeleton.png",   out.enhance.skeleton);
        if (!out.minutiae.empty() && !out.enhance.enhanced.empty())
            w("08_minutiae.png", draw_minutiae(out.enhance.enhanced, out.minutiae));
        std::printf("\n덤프: %s\n", dump_dir.c_str());
    }

    std::string html, pdf, why;
    if (no_pdf) {
        std::error_code ec; std::filesystem::create_directories(out_dir, ec);
        html = (std::filesystem::path(out_dir) / "report.html").string();
        std::ofstream f(html, std::ios::binary);
        f << report::build_html(ri);
        std::printf("\n----- 보고서 -----\nHTML: %s  (PDF 생략)\n", html.c_str());
    } else {
        const bool pdf_ok = report::save(ri, out_dir, html, pdf, why);
        std::printf("\n----- 보고서 -----\nHTML: %s\n", html.c_str());
        if (pdf_ok) std::printf("PDF : %s\n", pdf.c_str());
        else        std::printf("PDF : 생성 실패 — %s\n", why.c_str());
    }

    return out.ok ? 0 : 2;
}
