#include "textspotter/east_detector.hpp"

EastTextDetector::EastTextDetector(const char *model_path, float conf_threshold, float nms_threshold, int width,
                                   int height, double detect_scale, const cv::Scalar &detect_mean, bool swap_rb)
    : detector_(std::make_unique<cv::dnn::TextDetectionModel_EAST>(model_path)) {
  detector_->setConfidenceThreshold(conf_threshold);
  detector_->setNMSThreshold(nms_threshold);
  detector_->setInputParams(detect_scale, cv::Size{width, height}, detect_mean, swap_rb);
}

EastTextDetector::EastTextDetector(std::unique_ptr<cv::dnn::TextDetectionModel_EAST> detector)
    : detector_(std::move(detector)) {}

auto EastTextDetector::detect(const cv::Mat &image) const noexcept -> std::vector<TextDetectionResult> {
  if (image.empty()) {
    return {};
  }

  std::vector<std::vector<cv::Point>> detections;
  std::vector<float> confidences;
  detector_->detect(image, detections, confidences);

  std::vector<TextDetectionResult> result;

  for (size_t i = 0; i < detections.size(); ++i) {
    const auto &detection = detections[i];
    cv::Point top_left = detection[1];
    cv::Point bot_right = detection[3];
    result.push_back({{top_left, bot_right}, confidences[i]});
  }

  return result;
}