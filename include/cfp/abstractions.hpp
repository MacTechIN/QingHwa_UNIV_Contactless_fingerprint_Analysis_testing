// =============================================================================
//  cfp/abstractions.hpp — 수직 절편(vertical slice)이 꽂히는 확장 지점
//
//  [설계 의도] dev_plan의 "Vertical Stackable" 원칙을 코드로 강제하는 파일.
//  절편 V3(고전 YCbCr)와 V5(U-Net ONNX)는 IFingerSegmenter라는 동일 계약을
//  구현하므로, 파이프라인 코드를 건드리지 않고 DI 등록만 바꿔 A/B 비교가 된다.
//  → 이 헤더의 인터페이스는 "추가"만 하고 "변경"하지 않는다.
// =============================================================================
#pragma once

#include <memory>
#include <opencv2/core.hpp>

#include "cfp/types.hpp"

namespace cfp {

// 획득 소스: 카메라(V1) / 파일 폴더(V8 배치평가) 가 같은 계약을 구현한다.
class IFrameSource {
public:
    virtual ~IFrameSource() = default;
    // 다음 프레임을 BGR 8UC3로 반환. 더 없으면 false.
    virtual bool next(cv::Mat& bgr_out) = 0;
};

// 품질 게이트 (V2)
class IQualityGate {
public:
    virtual ~IQualityGate() = default;
    virtual QualityReport evaluate(const cv::Mat& bgr) const = 0;
};

// 손가락 세그멘테이션 결과
struct SegmentResult {
    bool     ok = false;
    cv::Mat  mask;        // 8UC1, 손가락=255
    cv::Mat  roi_gray;    // 회전정규화·크롭된 그레이 ROI (파이프라인의 실입력)
    // roi_gray와 정확히 같은 기하로 변환된 손가락 마스크.
    // [중요] 세그멘테이션이 이미 알아낸 "손가락이 어디인가"를 하류 단계가 재추정하면
    // 배경이 유효영역으로 새어 들어간다(실제 사진 테스트에서 발견된 결함).
    // 그래서 마스크를 값으로 전파한다.
    cv::Mat  roi_mask;
    cv::RotatedRect box;  // 원본 좌표계에서의 ROI
    double   axis_angle_deg = 0.0;
    std::string reason;
};

// 세그멘테이션 (V3 고전 / V5 딥)
class IFingerSegmenter {
public:
    virtual ~IFingerSegmenter() = default;
    virtual SegmentResult segment(const cv::Mat& bgr) const = 0;
};

// 융선 향상 (V4)
struct EnhanceResult {
    cv::Mat normalized;    // 32FC1 평균/분산 정규화
    cv::Mat orientation;   // 32FC1 픽셀별 융선 방향 (rad, [0,pi))
    cv::Mat frequency;     // 32FC1 픽셀별 융선 주파수 (cycles/px)
    cv::Mat mask;          // 8UC1 유효 융선 영역
    // 32FC1 방향장 일관성 0..1. 미뉴셔 신뢰도 판정에 쓴다.
    // 1에 가까우면 융선이 국소적으로 완전히 나란하다는 뜻이고,
    // 낮은 곳에서 나온 미뉴셔는 대개 잡음/음영 경계가 만든 위양성이다.
    cv::Mat coherence;
    cv::Mat enhanced;      // 8UC1 Gabor 통과 결과
    cv::Mat binary;        // 8UC1 0/255 이진 융선
    cv::Mat skeleton;      // 8UC1 0/255 1픽셀 세선화
};

class IRidgeEnhancer {
public:
    virtual ~IRidgeEnhancer() = default;
    // roi_mask는 선택적 사전지식이다. 비어 있으면 구현체가 스스로 추정한다.
    virtual EnhanceResult enhance(const cv::Mat& roi_gray,
                                  const cv::Mat& roi_mask = cv::Mat()) const = 0;
};

// 미뉴셔 추출 (V6a 고전 CN / V6b MinuNet ONNX)
class IMinutiaExtractor {
public:
    virtual ~IMinutiaExtractor() = default;
    virtual std::vector<Minutia> extract(const EnhanceResult& enh) const = 0;
};

}  // namespace cfp
