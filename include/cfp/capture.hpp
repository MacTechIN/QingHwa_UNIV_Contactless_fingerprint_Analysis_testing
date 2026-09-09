// =============================================================================
//  cfp/capture.hpp — 카메라 캡처 (dev_plan V1)
//  Windows: Media Foundation 구현이 IFrameSource 계약을 만족한다.
// =============================================================================
#pragma once

#include <string>
#include <vector>

#include "cfp/abstractions.hpp"

namespace cfp {

struct CameraInfo {
    int         index = -1;
    std::string name;
};

#ifdef _WIN32

std::vector<CameraInfo> enumerate_cameras();

class MediaFoundationCamera final : public IFrameSource {
public:
    MediaFoundationCamera();
    ~MediaFoundationCamera() override;
    MediaFoundationCamera(const MediaFoundationCamera&) = delete;
    MediaFoundationCamera& operator=(const MediaFoundationCamera&) = delete;

    bool open(int device_index = 0, int w = 1280, int h = 720);
    bool next(cv::Mat& bgr_out) override;

    // research.md §2.1: 조도 실험이 성립하려면 자동 노출/AWB를 반드시 꺼야 한다.
    // 미지원 카메라에서는 false를 반환한다(예외 아님) → UI가 경고 배지로 알린다.
    bool lock_auto_controls();
    bool set_manual(long exposure_log2, long focus = -1, bool lock_wb = true);

    bool        is_open() const;
    int         width()   const;
    int         height()  const;
    std::string last_error() const;

private:
    class Impl;
    Impl* impl_;
};

#endif  // _WIN32

}  // namespace cfp
