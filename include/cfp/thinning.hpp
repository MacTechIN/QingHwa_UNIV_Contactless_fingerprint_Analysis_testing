// =============================================================================
//  cfp/thinning.hpp — Zhang-Suen 세선화
//
//  [왜 직접 구현하는가]
//  이 함수 하나를 위해 OpenCV의 opencv_contrib 전체(ximgproc)를 의존성으로
//  끌어오면, vcpkg에서 contrib -> dnn -> protobuf -> abseil 순으로 연쇄 의존이
//  발생해 빌드 시간이 수십 분 늘고 실패 지점도 그만큼 늘어난다.
//  (실제로 Windows 빌드가 abseil에서 실패했다)
//
//  Zhang-Suen은 60줄이면 구현되는 고전 알고리즘이고, 이 프로젝트는 이미
//  방향장·Gabor·Crossing Number를 직접 구현하고 있다. 세선화만 외부에 맡길 이유가 없다.
//
//  원논문: T. Y. Zhang and C. Y. Suen,
//          "A fast parallel algorithm for thinning digital patterns",
//          Communications of the ACM, 27(3):236-239, 1984.
// =============================================================================
#pragma once

#include <opencv2/core.hpp>

namespace cfp {

// src: 8UC1 이진 영상(0 또는 255). dst: 1픽셀 두께 골격(0 또는 255).
// in-place 호출(src와 dst가 같은 Mat) 가능.
void zhang_suen_thinning(const cv::Mat& src, cv::Mat& dst);

}  // namespace cfp
