// =============================================================================
//  cfp/pipeline.hpp — 절편 조립기 (dev_plan V0-µ7)
//
//  각 단계의 중간 산출물과 소요시간을 모두 보관한다.
//  UI(썸네일 스트립)와 배치평가가 같은 객체를 재사용하기 위함.
// =============================================================================
#pragma once

#include <memory>
#include <vector>

#include "cfp/abstractions.hpp"

namespace cfp {

struct PipelineOutput {
    bool                     ok = false;
    QualityReport            quality;
    SegmentResult            segment;
    EnhanceResult            enhance;
    std::vector<Minutia>     minutiae;
    std::vector<StageTiming> timings;
    std::string              failure_stage;
};

class Pipeline {
public:
    Pipeline(std::shared_ptr<IQualityGate>      quality,
             std::shared_ptr<IFingerSegmenter>  segmenter,
             std::shared_ptr<IRidgeEnhancer>    enhancer,
             std::shared_ptr<IMinutiaExtractor> extractor);

    PipelineOutput run(const cv::Mat& bgr, bool enforce_quality_gate = true) const;

    // 파이프라인 산출물 → 저장용 템플릿
    static Template to_template(const PipelineOutput& out,
                                std::string subject_id,
                                std::string capture_id);

private:
    std::shared_ptr<IQualityGate>      quality_;
    std::shared_ptr<IFingerSegmenter>  segmenter_;
    std::shared_ptr<IRidgeEnhancer>    enhancer_;
    std::shared_ptr<IMinutiaExtractor> extractor_;
};

}  // namespace cfp
