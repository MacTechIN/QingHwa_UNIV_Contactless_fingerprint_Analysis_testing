#include "cfp/pipeline.hpp"

#include <chrono>
#include <utility>

namespace cfp {

namespace {
// RAII 타이머 — 스코프를 벗어나는 순간 소요시간을 기록한다.
// 예외가 나가도 소멸자가 불리므로 계측이 누락되지 않는다.
class ScopedTimer {
public:
    ScopedTimer(std::vector<StageTiming>& sink, std::string name)
        : sink_(sink), name_(std::move(name)),
          t0_(std::chrono::steady_clock::now()) {}
    ~ScopedTimer() {
        const auto dt = std::chrono::steady_clock::now() - t0_;
        sink_.push_back({name_,
            std::chrono::duration<double, std::milli>(dt).count()});
    }
private:
    std::vector<StageTiming>& sink_;
    std::string name_;
    std::chrono::steady_clock::time_point t0_;
};
}  // namespace

Pipeline::Pipeline(std::shared_ptr<IQualityGate>      quality,
                   std::shared_ptr<IFingerSegmenter>  segmenter,
                   std::shared_ptr<IRidgeEnhancer>    enhancer,
                   std::shared_ptr<IMinutiaExtractor> extractor)
    : quality_(std::move(quality)),
      segmenter_(std::move(segmenter)),
      enhancer_(std::move(enhancer)),
      extractor_(std::move(extractor)) {}

// =============================================================================
//  [핵심 로직 해설 — 실패를 예외가 아니라 값으로 다루는 이유]
//  "손가락이 안 보인다", "초점이 안 맞는다"는 버그가 아니라 정상적인 운영 상태다.
//  이를 예외로 던지면 실시간 프리뷰 루프(초당 30회)가 예외 비용을 계속 물고,
//  UI는 매 프레임 try/catch를 둘러야 한다.
//  그래서 각 단계는 ok 플래그 + failure_stage 문자열로 실패를 보고하고,
//  호출자는 어느 단계에서 멈췄는지 그대로 화면에 띄울 수 있다.
// =============================================================================
PipelineOutput Pipeline::run(const cv::Mat& bgr, bool enforce_quality_gate) const {
    PipelineOutput out;

    {
        ScopedTimer t(out.timings, "quality");
        out.quality = quality_->evaluate(bgr);
    }
    if (enforce_quality_gate && !out.quality.pass) {
        out.failure_stage = "quality:" + out.quality.reason;
        return out;
    }

    {
        ScopedTimer t(out.timings, "segment");
        out.segment = segmenter_->segment(bgr);
    }
    if (!out.segment.ok) {
        out.failure_stage = "segment:" + out.segment.reason;
        return out;
    }

    {
        ScopedTimer t(out.timings, "enhance");
        out.enhance = enhancer_->enhance(out.segment.roi_gray, out.segment.roi_mask);
    }
    if (out.enhance.skeleton.empty()) {
        out.failure_stage = "enhance:no skeleton";
        return out;
    }

    {
        ScopedTimer t(out.timings, "minutiae");
        out.minutiae = extractor_->extract(out.enhance);
    }
    out.ok = !out.minutiae.empty();
    if (!out.ok) out.failure_stage = "minutiae:none found";
    return out;
}

Template Pipeline::to_template(const PipelineOutput& o,
                               std::string subject_id, std::string capture_id) {
    Template t;
    t.subject_id = std::move(subject_id);
    t.capture_id = std::move(capture_id);
    t.minutiae   = o.minutiae;
    if (!o.segment.roi_gray.empty()) {
        t.width  = o.segment.roi_gray.cols;
        t.height = o.segment.roi_gray.rows;
    }
    return t;
}

}  // namespace cfp
