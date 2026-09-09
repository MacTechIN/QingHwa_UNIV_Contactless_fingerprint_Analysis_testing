// =============================================================================
//  cfp/report.hpp — 실행 결과 최종 보고서 (HTML 시각화 + PDF 저장)
//
//  [목적] 앱을 한 번 돌리고 나면, 각 스택이 논문의 무엇을 구현했고 이번 입력에서
//  실제로 어떤 수치가 나왔는지를 하나의 문서로 남긴다. 발표자료로 바로 쓸 수 있어야 한다.
//
//  [설계] HTML을 단일 진실원(single source of truth)으로 삼고 PDF는 그것을 인쇄한다.
//  두 포맷을 따로 그리면 반드시 어긋나기 때문이다.
//  이미지는 base64 data URI로 인라인해 HTML 파일 하나만으로 자족하게 만든다.
// =============================================================================
#pragma once

#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "cfp/narration.hpp"
#include "cfp/pipeline.hpp"

namespace cfp::report {

struct ImagePanel {
    std::string caption;
    std::string note;       // 이 그림이 논문의 무엇을 보여주는가
    cv::Mat     image;      // 8UC1 / 8UC3
};

struct ReportInput {
    std::string title       = "비접촉 지문인식 파이프라인 실행 보고서";
    std::string subtitle;
    std::string source_name;                 // 입력 파일/카메라 이름
    std::vector<StageNarration> narrations;  // 스택별 논문 대조 해설
    std::vector<ImagePanel>     panels;      // 단계별 시각 산출물
    std::vector<StageTiming>    timings;
    std::string                 summary;     // 총평
    // 매칭 결과(있을 때만)
    bool   has_match     = false;
    double match_score   = 0.0;
    int    match_inliers = 0;
};

// 자족형 HTML 문자열 생성 (외부 리소스 참조 없음)
std::string build_html(const ReportInput& in);

// PDF 저장.
//  1순위: Windows WebView2 PrintToPdf (앱에 이미 렌더러가 있으므로 화면과 100% 동일)
//  2순위: headless 브라우저 CLI (--headless --print-to-pdf)
//  실패 시 false를 반환하고 why에 사유를 담는다. HTML은 어떤 경우에도 남는다.
bool export_pdf(const std::string& html_path,
                const std::string& pdf_path,
                std::string& why);

// HTML + PDF를 한 번에. 반환값은 PDF 생성 성공 여부(HTML은 항상 생성).
bool save(const ReportInput& in,
          const std::string& out_dir,
          std::string& html_path_out,
          std::string& pdf_path_out,
          std::string& why);

}  // namespace cfp::report
