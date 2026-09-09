#include "cfp/narration.hpp"

#include <sstream>

namespace cfp {

std::string paper_title(Paper p) {
    switch (p) {
        case Paper::Chowdhury2022:
            return "Contactless Fingerprint Recognition Using Deep Learning - A Systematic Review";
        case Paper::Oduah2021:
            return "Towards a high-precision contactless fingerprint scanner for biometric authentication";
        case Paper::Rachel2025:
            return "Robust contactless fingerprint authentication using dolphin optimization and SVM hybridization";
    }
    return "?";
}

std::string paper_cite(Paper p) {
    switch (p) {
        case Paper::Chowdhury2022:
            return "Chowdhury & Imtiaz, J. Cybersecur. Priv. 2022, 2(3):714-730";
        case Paper::Oduah2021:
            return "Oduah et al., Array 11 (2021) 100083";
        case Paper::Rachel2025:
            return "Rachel & Devarasan, Front. Big Data 8:1641714 (2025)";
    }
    return "?";
}

std::string render_text(const StageNarration& n) {
    std::ostringstream o;
    o << "\n[" << n.index << "] " << n.stage << "   (" << n.ms << " ms)\n";
    o << "    무엇을      : " << n.what << "\n";
    o << "    논문 근거   : " << paper_cite(n.ref.paper) << "  " << n.ref.locus << "\n";
    if (!n.ref.origin.empty())
        o << "                  └ 원저: " << n.ref.origin << "\n";
    o << "    논문 원문   : \"" << n.ref.quote << "\"\n";
    o << "    구현한 것   : " << n.implements << "\n";
    o << "    구현 방식   : " << n.how << "\n";
    o << "    논문의 예측 : " << n.paper_expectation << "\n";
    if (!n.observed.empty()) {
        o << "    이번 실행   :\n";
        for (const auto& s : n.observed) o << "                  - " << s << "\n";
    }
    if (!n.verdict.empty())
        o << "    대조 결론   : " << n.verdict << "\n";
    return o.str();
}

std::string render_markdown(const std::vector<StageNarration>& ns) {
    std::ostringstream o;
    o << "# 실행 단계별 논문 대조 리포트\n\n";
    o << "| # | 스택 | 논문 근거 | 소요(ms) |\n|---|---|---|---|\n";
    for (const auto& n : ns)
        o << "| " << n.index << " | " << n.stage << " | "
          << paper_cite(n.ref.paper) << " " << n.ref.locus << " | "
          << n.ms << " |\n";
    o << "\n";
    for (const auto& n : ns) {
        o << "## " << n.index << ". " << n.stage << "\n\n";
        o << "> **논문**: " << paper_title(n.ref.paper) << "  \n";
        o << "> **위치**: " << n.ref.locus;
        if (!n.ref.origin.empty()) o << "  (원저: " << n.ref.origin << ")";
        o << "  \n> **원문 근거**: \"" << n.ref.quote << "\"\n\n";
        o << "- **구현한 것**: " << n.implements << "\n";
        o << "- **구현 방식**: " << n.how << "\n";
        o << "- **논문의 예측**: " << n.paper_expectation << "\n";
        if (!n.observed.empty()) {
            o << "- **이번 실행 관측**:\n";
            for (const auto& s : n.observed) o << "  - " << s << "\n";
        }
        if (!n.verdict.empty()) o << "- **대조 결론**: " << n.verdict << "\n";
        o << "\n";
    }
    return o.str();
}

}  // namespace cfp
