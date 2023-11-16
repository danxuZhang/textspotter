#pragma once

#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>

#include "textspotter/result_type.hpp"

class EastTextDetector {
 public:
  explicit EastTextDetector(const char *model_path, float conf_threshold = 0.5, float nms_threshold = 0.4,
                            int width = 640, int height = 320, double detect_scale = 1.0,
                            const cv::Scalar &detect_mean = cv::Scalar{123.68, 116.78, 103.94}, bool swap_rb = true);

  explicit EastTextDetector(std::unique_ptr<cv::dnn::TextDetectionModel_EAST> detector);

  ~EastTextDetector() = default;

  auto detect(const cv::Mat &image) const noexcept -> std::vector<TextDetectionResult>;

 private:
  std::unique_ptr<cv::dnn::TextDetectionModel_EAST> detector_;
};