// =============================================================================
//  cfp/narration.hpp — "논문 ↔ 구현" 추적 및 실행시 단계별 해설
//
//  [왜 이런 게 코드에 있는가]
//  이 앱은 제품이 아니라 "논문 검증용 테스트 하네스"다.
//  따라서 각 스택이 논문의 어느 주장을 구현한 것인지, 그리고 실제로 돌렸을 때
//  그 주장이 이 입력에서 어떻게 나타났는지를 화면에 같이 띄워야 가치가 있다.
//
//  구조:
//    PaperRef        — 어느 논문 어느 절을 구현했는가 (정적, 컴파일 타임 고정)
//    StageNarration  — 그 스택의 설명 + 이번 실행의 실측값 해설 (동적)
//
//  narration은 파이프라인 실행마다 새로 생성되며, CLI/GUI/발표자료가 같은
//  데이터를 재사용한다. 문서와 코드가 따로 노는 것을 막기 위한 장치다.
// =============================================================================
#pragma once

#include <string>
#include <vector>

#include "cfp/types.hpp"

namespace cfp {

// 논문 출처. 세 편 모두를 참조하므로 어느 논문인지 명시한다.
enum class Paper {
    Chowdhury2022,   // Contactless FP Recognition Using Deep Learning - A Systematic Review
    Oduah2021,       // Towards a high-precision contactless fingerprint scanner
    Rachel2025,      // DOA + SVM hybridization
};

struct PaperRef {
    Paper       paper;
    std::string locus;      // 절/표/그림 위치            예) "§4 Table 3"
    std::string quote;      // 논문 원문 근거 (요약 인용)
    std::string origin;     // 그 절이 인용한 원저         예) "Wang et al. 2017"
};

std::string paper_title(Paper p);
std::string paper_cite(Paper p);

// 한 스택(파이프라인 단계)의 설명 묶음.
struct StageNarration {
    int         index = 0;          // 1-based 실행 순서
    std::string stage;              // 스택 이름
    std::string what;               // 이 스택이 무엇을 하는가 (한 줄)
    PaperRef    ref;                // 논문 근거
    std::string implements;         // 논문의 무엇을 구현했는가
    std::string how;                // 어떤 방식으로 구현했는가
    std::string paper_expectation;  // 논문이 예측/주장하는 결과
    std::vector<std::string> observed;   // 이번 실행의 실측 관측 (동적 생성)
    std::string verdict;            // 논문 주장과 이번 실행의 대조 결론
    double      ms = 0.0;           // 소요 시간
};

// 사람이 읽는 형태로 렌더링. GUI/CLI/발표 슬라이드가 공유한다.
std::string render_text(const StageNarration& n);
std::string render_markdown(const std::vector<StageNarration>& ns);

}  // namespace cfp
