// =============================================================================
//  cfp/cv_compat.hpp — OpenCV 4.x / 5.x 호환 계층
//
//  [배경] OpenCV 5.x는 모듈을 재편하면서 imgproc에 있던 일부 2D 기하 함수를
//  새로 만든 geometry 모듈로 옮겼다. 그래서 4.x에서 잘 빌드되던 코드가
//  5.x 프리빌트에서 다음처럼 실패한다:
//
//      error C2039: 'minAreaRect': 'cv'의 멤버가 아닙니다.
//      error C3861: 'getRotationMatrix2D': 식별자를 찾을 수 없습니다.
//
//  이동한 함수(우리가 쓰는 것만):
//      contourArea, minAreaRect, getRotationMatrix2D   imgproc -> geometry
//  나머지(cvtColor, warpAffine, findContours, drawContours, morphologyEx,
//  resize, threshold, filter2D, boxFilter, Sobel, CLAHE 등)는 그대로 imgproc에 있다.
//
//  버전 분기를 이 헤더 한 곳에 모아 두어, 각 .cpp가 조건부 include를
//  중복해서 갖지 않도록 한다.
// =============================================================================
#pragma once

#include <opencv2/core.hpp>       // CV_VERSION_MAJOR 정의를 먼저 확보
#include <opencv2/imgproc.hpp>

#if defined(CV_VERSION_MAJOR) && CV_VERSION_MAJOR >= 5
#  include <opencv2/geometry.hpp>
#endif
