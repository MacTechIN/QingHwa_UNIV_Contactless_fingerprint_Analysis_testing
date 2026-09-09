#include "cfp/segmenter.hpp"

#include "cfp/cv_compat.hpp"

namespace cfp {

// =============================================================================
//  고전 손가락 세그멘테이션 (dev_plan V3 / research.md §3 "YCbCr 피부색 추정")
//
//  [핵심 로직 해설 — 왜 RGB가 아니라 YCbCr인가]
//  RGB는 밝기와 색상이 세 채널에 뒤섞여 있다. 조명이 밝아지면 R,G,B가 동시에
//  커지므로 "피부색 범위"를 RGB 박스로 잡으면 조명이 바뀔 때마다 무너진다.
//  YCbCr은 Y(휘도)와 Cb,Cr(색차)을 분리하므로, Y를 버리고 Cb/Cr만 보면
//  조명 밝기 변화에 대체로 불변인 피부색 판정이 가능하다.
//  이것이 research.md가 인용한 Wang et al.(2017) 접근의 핵심이다.
//
//  [한계 — 코드리뷰에서 다시 지적됨]
//  고정 임계 박스는 피부 톤(멜라닌 농도)에 따라 실패한다.
//  V5의 U-Net(IFingerSegmenter 동일 계약)으로 교체하는 것이 정공법이며,
//  이 구현은 모델 없이도 파이프라인이 끝까지 돌게 하는 폴백 레인이다.
// =============================================================================
SegmentResult YCbCrSegmenter::segment(const cv::Mat& bgr) const {
    SegmentResult out;
    if (bgr.empty() || bgr.channels() < 3) { out.reason = "need BGR input"; return out; }

    // --- 1) 피부색 마스크 ---
    cv::Mat ycrcb;
    cv::cvtColor(bgr, ycrcb, cv::COLOR_BGR2YCrCb);
    cv::Mat mask;
    cv::inRange(ycrcb,
                cv::Scalar(0,   cfg_.cr_min, cfg_.cb_min),
                cv::Scalar(255, cfg_.cr_max, cfg_.cb_max),
                mask);

    // --- 2) 모폴로지 정리: open으로 점잡음 제거 → close로 내부 구멍 메움 ---
    // 순서가 중요하다. close를 먼저 하면 잡음까지 뭉쳐서 커진다.
    const cv::Mat k = cv::getStructuringElement(cv::MORPH_ELLIPSE, {7, 7});
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,  k);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, k);

    // --- 3) 최대 연결성분만 남긴다(손은 하나) ---
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty()) { out.reason = "no skin region"; return out; }

    std::size_t best = 0;
    double best_area = 0.0;
    for (std::size_t i = 0; i < contours.size(); ++i) {
        const double a = cv::contourArea(contours[i]);
        if (a > best_area) { best_area = a; best = i; }
    }
    const double area_ratio = best_area / static_cast<double>(bgr.total());
    if (area_ratio < cfg_.min_area_ratio) {
        out.reason = "skin region too small";
        return out;
    }

    cv::Mat clean = cv::Mat::zeros(mask.size(), CV_8UC1);
    cv::drawContours(clean, contours, static_cast<int>(best), cv::Scalar(255), cv::FILLED);
    out.mask = clean;

    // --- 4) 주축 추정 ---
    // minAreaRect는 손가락처럼 길쭉한 영역의 장축 방향을 안정적으로 준다.
    // (PCA도 가능하나 마스크 경계 잡음에 더 민감하다)
    cv::RotatedRect rr = cv::minAreaRect(contours[best]);
    // OpenCV는 width/height 배정이 각도에 따라 뒤바뀐다. 항상 height가 장축이 되도록 정규화.
    if (rr.size.width > rr.size.height) {
        std::swap(rr.size.width, rr.size.height);
        rr.angle += 90.0f;
    }

    // -------------------------------------------------------------------------
    //  [핵심] 180도 모호성 해소 — 실제 사진 테스트로 발견한 버그
    //
    //  minAreaRect의 각도는 180도 모듈로다. 즉 -4.9도와 175.1도는 "같은 축"을 뜻한다.
    //  그런데 이 값을 그대로 회전각으로 쓰면 결과가 전혀 달라진다:
    //  175.1도로 회전시키면 손가락이 거의 거꾸로 서고, 그 상태에서 위쪽을 지두로
    //  잘라내면 실제로는 손가락 '밑동'을 자르게 된다.
    //
    //  실측: 같은 사진을 5도만 돌렸는데 주축각이 -0.0도에서 175.1도로 튀었고,
    //        그 결과 미뉴셔가 29개에서 11개로 붕괴했다.
    //
    //  해결 2단계:
    //   ① 각도를 (-90, 90]로 접어 손가락이 대략 세로로 서게 만든다.
    //   ② 그래도 위/아래가 뒤집혔을 수 있다. 손가락 끝은 밑동보다 '좁다'는
    //      기하학적 사실로 판별한다. 위쪽 1/4의 마스크 폭이 아래쪽 1/4보다
    //      넓으면 거꾸로 선 것이므로 180도를 더한다.
    // -------------------------------------------------------------------------
    while (rr.angle >   90.0f) rr.angle -= 180.0f;   // ① (-90, 90]로 접기
    while (rr.angle <= -90.0f) rr.angle += 180.0f;

    {
        // ② 위/아래 판별: 임시로 역회전한 마스크에서 양 끝의 폭을 잰다.
        const cv::Mat probe_rot = cv::getRotationMatrix2D(rr.center, rr.angle, 1.0);
        cv::Mat probe;
        cv::warpAffine(clean, probe, probe_rot, clean.size(), cv::INTER_NEAREST);

        const int y0 = static_cast<int>(rr.center.y - rr.size.height * 0.5f);
        const int hq = std::max(1, static_cast<int>(rr.size.height * 0.25f));
        cv::Rect top(0, y0,                          probe.cols, hq);
        cv::Rect bot(0, y0 + static_cast<int>(rr.size.height) - hq, probe.cols, hq);
        top &= cv::Rect(0, 0, probe.cols, probe.rows);
        bot &= cv::Rect(0, 0, probe.cols, probe.rows);

        if (top.area() > 0 && bot.area() > 0) {
            const double top_area = cv::countNonZero(probe(top));
            const double bot_area = cv::countNonZero(probe(bot));
            // 손가락 끝(둥근 쪽)이 면적이 작다. 위가 더 넓으면 뒤집힌 것.
            if (top_area > bot_area) rr.angle += 180.0f;
        }
    }

    out.box = rr;
    out.axis_angle_deg = rr.angle;

    // --- 5) 회전 정규화 ---
    // 손가락 장축이 영상의 세로축과 나란해지도록 전체를 역회전시킨다.
    // 이후 단계(방향장/Gabor)는 회전 불변이 아니므로, 여기서 자세를 고정해 두면
    // 후속 파라미터(융선 주기 등)를 일관되게 쓸 수 있다.
    cv::Mat gray;
    cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);

    const cv::Mat rot = cv::getRotationMatrix2D(rr.center, rr.angle, 1.0);
    cv::Mat rotated, rotated_mask;
    cv::warpAffine(gray,  rotated,      rot, gray.size(), cv::INTER_CUBIC, cv::BORDER_REPLICATE);
    cv::warpAffine(clean, rotated_mask, rot, gray.size(), cv::INTER_NEAREST);

    // --- 6) 지두(fingertip) 크롭 ---
    // 회전 후 손가락은 세로로 서 있다. 위쪽 tip_fraction 만큼이 손가락 끝.
    const float w = rr.size.width, h = rr.size.height;
    const float tip_h = h * static_cast<float>(cfg_.tip_fraction);
    cv::Rect roi(static_cast<int>(rr.center.x - w * 0.5f),
                 static_cast<int>(rr.center.y - h * 0.5f),
                 static_cast<int>(w),
                 static_cast<int>(tip_h));
    roi &= cv::Rect(0, 0, rotated.cols, rotated.rows);   // 영상 밖 클리핑
    if (roi.width < 16 || roi.height < 16) {
        out.reason = "roi degenerate";
        return out;
    }

    cv::Mat tip = rotated(roi).clone();
    cv::resize(tip, out.roi_gray, cv::Size(cfg_.out_w, cfg_.out_h), 0, 0, cv::INTER_CUBIC);

    // 마스크도 완전히 동일한 기하 변환을 거쳐야 픽셀이 일대일로 대응된다.
    // 보간은 NEAREST를 써야 이진성이 유지된다(CUBIC은 경계에 중간값을 만든다).
    cv::Mat tip_mask = rotated_mask(roi).clone();
    cv::resize(tip_mask, out.roi_mask, cv::Size(cfg_.out_w, cfg_.out_h), 0, 0,
               cv::INTER_NEAREST);

    out.ok = true;
    return out;
}

}  // namespace cfp
