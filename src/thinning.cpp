#include "cfp/thinning.hpp"

#include <opencv2/imgproc.hpp>

namespace cfp {

namespace {

// -----------------------------------------------------------------------------
//  한 서브패스에서 지울 픽셀을 표시한다.
//
//  [핵심 로직 해설]
//  P1을 중심으로 8-이웃을 시계 방향으로 P2..P9라 부른다.
//
//        P9 P2 P3
//        P8 P1 P4
//        P7 P6 P5
//
//  B(P1) = 이웃 중 1의 개수
//  A(P1) = P2->P3->...->P9->P2 순서로 돌 때 0에서 1로 바뀌는 횟수
//
//  삭제 조건(공통):
//    (a) 2 <= B <= 6      : 이웃이 너무 적으면 끝점(지우면 골격이 짧아진다),
//                           너무 많으면 내부점(지우면 구멍이 뚫린다)
//    (b) A == 1           : **연결성 보존 조건**. 이 값이 1이라는 것은 이웃의
//                           1들이 하나의 연속 덩어리라는 뜻이다. 2 이상이면
//                           P1이 두 갈래를 잇는 다리이므로 지우면 골격이 끊긴다.
//                           Zhang-Suen의 핵심이 바로 이 조건이다.
//
//  서브패스별 조건: 한쪽 패스는 남동쪽 경계를, 다른 쪽은 북서쪽 경계를 깎는다.
//  두 패스를 번갈아 적용해야 골격이 한쪽으로 치우치지 않고 도형 중앙에 남는다.
//  한 패스만 반복하면 골격이 대각선으로 밀린다.
//    pass 0 : P2*P4*P6 == 0  이고  P4*P6*P8 == 0
//    pass 1 : P2*P4*P8 == 0  이고  P2*P6*P8 == 0
// -----------------------------------------------------------------------------
int mark_pass(const cv::Mat& img, cv::Mat& marker, int pass) {
    marker.setTo(0);
    int marked = 0;

    for (int i = 1; i < img.rows - 1; ++i) {
        const uchar* prev = img.ptr<uchar>(i - 1);
        const uchar* cur  = img.ptr<uchar>(i);
        const uchar* next = img.ptr<uchar>(i + 1);
        uchar* mk = marker.ptr<uchar>(i);

        for (int j = 1; j < img.cols - 1; ++j) {
            if (!cur[j]) continue;                       // 배경은 대상 아님

            const int p2 = prev[j]     ? 1 : 0;          // 북
            const int p3 = prev[j + 1] ? 1 : 0;          // 북동
            const int p4 = cur[j + 1]  ? 1 : 0;          // 동
            const int p5 = next[j + 1] ? 1 : 0;          // 남동
            const int p6 = next[j]     ? 1 : 0;          // 남
            const int p7 = next[j - 1] ? 1 : 0;          // 남서
            const int p8 = cur[j - 1]  ? 1 : 0;          // 서
            const int p9 = prev[j - 1] ? 1 : 0;          // 북서

            const int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            if (B < 2 || B > 6) continue;                // 조건 (a)

            // 조건 (b): 0->1 전이 횟수. 순환이므로 마지막에 P9->P2를 더한다.
            const int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                          (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                          (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                          (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            if (A != 1) continue;

            const bool ok = (pass == 0)
                ? ((p2 * p4 * p6) == 0 && (p4 * p6 * p8) == 0)
                : ((p2 * p4 * p8) == 0 && (p2 * p6 * p8) == 0);
            if (!ok) continue;

            mk[j] = 1;
            ++marked;
        }
    }
    return marked;
}

}  // namespace

// =============================================================================
//  본체
//
//  [핵심 로직 해설 — 왜 "표시 후 일괄 삭제"인가]
//  픽셀을 검사하면서 곧바로 지우면, 같은 패스 안에서 뒤에 검사되는 픽셀이
//  이미 바뀐 이웃을 보게 된다. 그러면 결과가 주사 순서에 의존하게 되고
//  Zhang-Suen이 보장하는 연결성 보존이 깨진다.
//  그래서 한 패스에서는 지울 픽셀을 marker에 표시만 하고, 패스가 끝난 뒤
//  한꺼번에 지운다(병렬 알고리즘의 정의 그대로).
// =============================================================================
void zhang_suen_thinning(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) { dst.release(); return; }
    CV_Assert(src.type() == CV_8UC1);

    // 내부 표현은 0/1로 정규화한다. 입력이 0/255든 0/1이든 동일하게 동작한다.
    cv::Mat img;
    cv::threshold(src, img, 0, 1, cv::THRESH_BINARY);

    // 경계 픽셀은 8-이웃이 완전하지 않다. 1픽셀 테두리를 0으로 채워
    // 경계 검사를 생략할 수 있게 만든다(속도와 정확성 모두에 유리).
    cv::copyMakeBorder(img, img, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(0));

    cv::Mat marker(img.size(), CV_8UC1);

    // 수렴할 때까지 반복. 상한을 두어 병리적 입력에서 무한 루프를 막는다.
    const int kMaxIter = 200;
    for (int it = 0; it < kMaxIter; ++it) {
        int changed = 0;
        for (int pass = 0; pass < 2; ++pass) {
            if (mark_pass(img, marker, pass) > 0) {
                img.setTo(0, marker);                    // 일괄 삭제
                changed = 1;
            }
        }
        if (!changed) break;                             // 더 지울 것이 없으면 종료
    }

    // 테두리 제거 후 0/255로 되돌린다.
    cv::Mat out = img(cv::Rect(1, 1, src.cols, src.rows)) * 255;
    out.copyTo(dst);
}

}  // namespace cfp
