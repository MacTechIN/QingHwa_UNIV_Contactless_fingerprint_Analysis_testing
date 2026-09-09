#include "cfp/report.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <sstream>

namespace cfp::report {

namespace {

// -----------------------------------------------------------------------------
//  base64 인코더
//  [핵심 로직 해설] 3바이트(24비트)를 6비트씩 4조각으로 잘라 각각을 문자로 사상한다.
//  입력이 3의 배수가 아니면 0으로 패딩하고, 패딩한 만큼 '='를 붙여
//  디코더가 원래 길이를 복원할 수 있게 한다.
//  외부 라이브러리를 끌어오지 않는 이유: 이 20줄 때문에 의존성을 늘릴 이유가 없다.
// -----------------------------------------------------------------------------
const char* kB64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64(const std::vector<uchar>& d) {
    std::string o;
    o.reserve((d.size() + 2) / 3 * 4);
    std::size_t i = 0;
    for (; i + 2 < d.size(); i += 3) {
        const unsigned v = (d[i] << 16) | (d[i + 1] << 8) | d[i + 2];
        o += kB64[(v >> 18) & 63]; o += kB64[(v >> 12) & 63];
        o += kB64[(v >>  6) & 63]; o += kB64[ v        & 63];
    }
    if (i < d.size()) {                       // 남은 1~2바이트 처리
        unsigned v = d[i] << 16;
        if (i + 1 < d.size()) v |= d[i + 1] << 8;
        o += kB64[(v >> 18) & 63];
        o += kB64[(v >> 12) & 63];
        o += (i + 1 < d.size()) ? kB64[(v >> 6) & 63] : '=';
        o += '=';
    }
    return o;
}

// cv::Mat → PNG → data URI. 폭이 크면 축소해 HTML 크기를 억제한다.
std::string data_uri(const cv::Mat& m, int max_w = 420) {
    if (m.empty()) return {};
    cv::Mat show = m;
    if (show.type() == CV_32F || show.type() == CV_32FC1)
        cv::normalize(show, show, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    if (show.cols > max_w) {
        const double s = static_cast<double>(max_w) / show.cols;
        cv::resize(show, show, {}, s, s, cv::INTER_AREA);
    }
    std::vector<uchar> buf;
    cv::imencode(".png", show, buf, {cv::IMWRITE_PNG_COMPRESSION, 6});
    return "data:image/png;base64," + base64(buf);
}

std::string esc(const std::string& s) {
    std::string o; o.reserve(s.size() + 16);
    for (char c : s) {
        switch (c) {
            case '&': o += "&amp;";  break;
            case '<': o += "&lt;";   break;
            case '>': o += "&gt;";   break;
            case '"': o += "&quot;"; break;
            default:  o += c;
        }
    }
    return o;
}

std::string fx(double v, int p = 2) {
    std::ostringstream o; o << std::fixed << std::setprecision(p) << v; return o.str();
}

// 파이프라인 다이어그램. 인라인 SVG라 외부 의존이 없고 PDF 인쇄 시에도 벡터로 남는다.
std::string pipeline_svg(const std::vector<StageNarration>& ns) {
    const int n = static_cast<int>(ns.size());
    if (n == 0) return {};
    const int bw = 150, bh = 62, gap = 22, pad = 12;
    const int w = pad * 2 + n * bw + (n - 1) * gap;
    std::ostringstream o;
    o << "<svg class=\"pipe\" viewBox=\"0 0 " << w << " " << (bh + 46)
      << "\" xmlns=\"http://www.w3.org/2000/svg\" role=\"img\">";
    for (int i = 0; i < n; ++i) {
        const int x = pad + i * (bw + gap);
        o << "<rect x=\"" << x << "\" y=\"14\" width=\"" << bw << "\" height=\"" << bh
          << "\" rx=\"9\" class=\"pbox\"/>";
        o << "<text x=\"" << (x + bw / 2) << "\" y=\"36\" class=\"pnum\">"
          << ns[i].index << "</text>";
        // 스택 이름은 괄호 앞까지만 잘라 한 줄에 넣는다
        std::string nm = ns[i].stage;
        const auto par = nm.find(" (");
        if (par != std::string::npos) nm = nm.substr(0, par);
        o << "<text x=\"" << (x + bw / 2) << "\" y=\"55\" class=\"pname\">"
          << esc(nm) << "</text>";
        o << "<text x=\"" << (x + bw / 2) << "\" y=\"" << (bh + 32)
          << "\" class=\"pms\">" << fx(ns[i].ms, 1) << " ms</text>";
        if (i + 1 < n) {
            const int ax = x + bw + 4, ay = 14 + bh / 2;
            o << "<path d=\"M" << ax << " " << ay << " L" << (ax + gap - 8) << " " << ay
              << "\" class=\"parrow\"/>"
              << "<path d=\"M" << (ax + gap - 8) << " " << ay << " l-6 -4 v8 z\" class=\"phead\"/>";
        }
    }
    o << "</svg>";
    return o.str();
}

const char* kCss = R"CSS(
:root{
  --bg:#ffffff; --fg:#1b1f24; --muted:#5b6470; --line:#e3e7ec;
  --card:#f7f9fb; --accent:#1f5fa8; --quote:#f2f6fb; --ok:#1a7f4b; --warn:#a8600f;
}
@media (prefers-color-scheme:dark){
  :root{--bg:#14171a;--fg:#e8ecf1;--muted:#9aa5b1;--line:#2a2f36;
        --card:#1b1f24;--accent:#7db3ee;--quote:#182230;--ok:#5fd39a;--warn:#e0a44c;}
}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--fg);
     font:15px/1.7 "Malgun Gothic","Apple SD Gothic Neo","Noto Sans KR",system-ui,sans-serif}
.wrap{max-width:1000px;margin:0 auto;padding:40px 28px 72px}
h1{font-size:27px;margin:0 0 6px;letter-spacing:-.4px}
h2{font-size:19px;margin:44px 0 14px;padding-bottom:8px;border-bottom:2px solid var(--line)}
.sub{color:var(--muted);margin:0 0 26px;font-size:14px}
.meta{display:flex;flex-wrap:wrap;gap:10px;margin:20px 0 8px}
.chip{background:var(--card);border:1px solid var(--line);border-radius:999px;
      padding:5px 13px;font-size:12.5px;color:var(--muted)}
.pipe{width:100%;height:auto;margin:14px 0 8px}
.pbox{fill:var(--card);stroke:var(--accent);stroke-width:1.4}
.pnum{fill:var(--accent);font-size:13px;font-weight:700;text-anchor:middle}
.pname{fill:var(--fg);font-size:11.5px;text-anchor:middle}
.pms{fill:var(--muted);font-size:10.5px;text-anchor:middle}
.parrow{stroke:var(--accent);stroke-width:1.4;fill:none}
.phead{fill:var(--accent)}
.stage{border:1px solid var(--line);border-radius:12px;margin:20px 0;overflow:hidden;
       background:var(--bg);break-inside:avoid;page-break-inside:avoid}
.stage>header{background:var(--card);padding:13px 18px;border-bottom:1px solid var(--line);
              display:flex;align-items:baseline;gap:11px}
.stage h3{margin:0;font-size:16.5px}
.idx{background:var(--accent);color:#fff;border-radius:6px;padding:1px 9px;
     font-size:12.5px;font-weight:700}
.ms{margin-left:auto;color:var(--muted);font-size:12.5px}
.body{padding:16px 18px}
.row{display:grid;grid-template-columns:104px 1fr;gap:8px 14px;margin:9px 0}
.k{color:var(--muted);font-size:13px;padding-top:2px}
.v{font-size:14.2px}
blockquote{margin:12px 0;padding:12px 15px;background:var(--quote);
           border-left:3px solid var(--accent);border-radius:0 7px 7px 0;
           font-size:13.4px;color:var(--fg)}
blockquote .src{display:block;margin-top:7px;color:var(--muted);font-size:12px}
ul.obs{margin:6px 0 0;padding-left:19px}
ul.obs li{margin:3px 0;font-size:14px}
.verdict{margin-top:12px;padding:10px 14px;border-radius:8px;font-size:13.8px;
         background:var(--card);border-left:3px solid var(--ok)}
.verdict.warn{border-left-color:var(--warn)}
table{width:100%;border-collapse:collapse;font-size:13.4px;margin:10px 0}
th,td{text-align:left;padding:8px 10px;border-bottom:1px solid var(--line);vertical-align:top}
th{color:var(--muted);font-weight:600;font-size:12.5px}
.gal{display:grid;grid-template-columns:repeat(auto-fill,minmax(210px,1fr));gap:14px;margin:12px 0}
.gal figure{margin:0;border:1px solid var(--line);border-radius:9px;overflow:hidden;
            background:var(--card);break-inside:avoid}
.gal img{width:100%;display:block;background:#000}
.gal figcaption{padding:9px 11px;font-size:12.5px}
.gal figcaption b{display:block;margin-bottom:3px;font-size:13px}
.gal figcaption span{color:var(--muted);font-size:11.8px}
footer{margin-top:46px;padding-top:16px;border-top:1px solid var(--line);
       color:var(--muted);font-size:12.3px}
@media print{
  body{font-size:11.4pt}
  .wrap{max-width:none;padding:0}
  .stage{border-color:#ccc}
  a{color:inherit;text-decoration:none}
  @page{size:A4;margin:14mm}
}
)CSS";

}  // namespace

// =============================================================================
//  HTML 생성
// =============================================================================
std::string build_html(const ReportInput& in) {
    std::ostringstream o;
    o << "<!DOCTYPE html><html lang=\"ko\"><head><meta charset=\"utf-8\">"
      << "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
      << "<title>" << esc(in.title) << "</title><style>" << kCss << "</style></head><body>"
      << "<div class=\"wrap\">";

    // ---- 표지 ----
    o << "<h1>" << esc(in.title) << "</h1>";
    if (!in.subtitle.empty()) o << "<p class=\"sub\">" << esc(in.subtitle) << "</p>";

    double total = 0.0;
    for (const auto& t : in.timings) total += t.ms;
    o << "<div class=\"meta\">"
      << "<span class=\"chip\">입력: " << esc(in.source_name) << "</span>"
      << "<span class=\"chip\">스택 " << in.narrations.size() << "단계</span>"
      << "<span class=\"chip\">총 소요 " << fx(total, 1) << " ms</span>";
    if (in.has_match)
        o << "<span class=\"chip\">매칭 스코어 " << fx(in.match_score, 4)
          << " (inlier " << in.match_inliers << ")</span>";
    o << "</div>";

    // ---- 파이프라인 다이어그램 ----
    o << "<h2>1. 파이프라인 구조</h2>";
    o << "<p class=\"sub\">각 상자가 하나의 수직 절편(vertical slice)이며, "
         "아래 2절에서 각 절편이 논문의 어느 주장을 구현했는지 대조한다.</p>";
    o << pipeline_svg(in.narrations);

    // ---- 논문 대조 요약표 ----
    o << "<h2>2. 스택 ↔ 논문 대응표</h2><table><thead><tr>"
      << "<th>#</th><th>스택</th><th>구현한 논문 내용</th><th>출처</th><th>소요</th>"
      << "</tr></thead><tbody>";
    for (const auto& n : in.narrations) {
        o << "<tr><td>" << n.index << "</td><td><b>" << esc(n.stage) << "</b></td><td>"
          << esc(n.what) << "</td><td>" << esc(paper_cite(n.ref.paper)) << "<br><span>"
          << esc(n.ref.locus) << "</span></td><td>" << fx(n.ms, 1) << " ms</td></tr>";
    }
    o << "</tbody></table>";

    // ---- 단계별 상세 ----
    o << "<h2>3. 단계별 상세 — 논문 근거와 실행 관측</h2>";
    for (const auto& n : in.narrations) {
        o << "<section class=\"stage\"><header>"
          << "<span class=\"idx\">" << n.index << "</span>"
          << "<h3>" << esc(n.stage) << "</h3>"
          << "<span class=\"ms\">" << fx(n.ms, 2) << " ms</span></header><div class=\"body\">";

        o << "<div class=\"row\"><div class=\"k\">무엇을</div><div class=\"v\">"
          << esc(n.what) << "</div></div>";

        o << "<blockquote>" << esc(n.ref.quote)
          << "<span class=\"src\">— " << esc(paper_title(n.ref.paper)) << ", "
          << esc(n.ref.locus);
        if (!n.ref.origin.empty()) o << " (원저: " << esc(n.ref.origin) << ")";
        o << "</span></blockquote>";

        o << "<div class=\"row\"><div class=\"k\">구현한 것</div><div class=\"v\">"
          << esc(n.implements) << "</div></div>"
          << "<div class=\"row\"><div class=\"k\">구현 방식</div><div class=\"v\">"
          << esc(n.how) << "</div></div>"
          << "<div class=\"row\"><div class=\"k\">논문의 예측</div><div class=\"v\">"
          << esc(n.paper_expectation) << "</div></div>";

        if (!n.observed.empty()) {
            o << "<div class=\"row\"><div class=\"k\">이번 실행</div><div class=\"v\"><ul class=\"obs\">";
            for (const auto& s : n.observed) o << "<li>" << esc(s) << "</li>";
            o << "</ul></div></div>";
        }
        if (!n.verdict.empty()) {
            // 논문 주장과 어긋나는 결과는 색을 달리해 발표 시 바로 눈에 띄게 한다.
            const bool warn = n.verdict.find("미달")   != std::string::npos ||
                              n.verdict.find("실패")   != std::string::npos ||
                              n.verdict.find("부족")   != std::string::npos ||
                              n.verdict.find("무너")   != std::string::npos ||
                              n.verdict.find("비정상") != std::string::npos ||
                              n.verdict.find("낮음")   != std::string::npos;
            o << "<div class=\"verdict" << (warn ? " warn" : "") << "\"><b>대조 결론 · </b>"
              << esc(n.verdict) << "</div>";
        }
        o << "</div></section>";
    }

    // ---- 시각 산출물 ----
    if (!in.panels.empty()) {
        o << "<h2>4. 단계별 시각 산출물</h2><div class=\"gal\">";
        for (const auto& p : in.panels) {
            const std::string uri = data_uri(p.image);
            if (uri.empty()) continue;
            o << "<figure><img src=\"" << uri << "\" alt=\"" << esc(p.caption) << "\">"
              << "<figcaption><b>" << esc(p.caption) << "</b><span>"
              << esc(p.note) << "</span></figcaption></figure>";
        }
        o << "</div>";
    }

    // ---- 총평 ----
    if (!in.summary.empty())
        o << "<h2>5. 총평</h2><p>" << esc(in.summary) << "</p>";

    o << "<footer>본 보고서는 파이프라인 실행 시 자동 생성되었습니다. "
         "인용된 논문 내용은 docs/ 아래 변환된 원문(paper.md)과 research.md에 근거합니다."
         "</footer></div></body></html>";
    return o.str();
}

}  // namespace cfp::report
