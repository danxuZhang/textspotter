#pragma once

#include <opencv2/core.hpp>
#include <string>

struct TextDetectionResult {
  cv::Rect bounding_box_;
  float conf_;
};

struct OcrResult {
  std::string text_;
  cv::Rect bounding_box_;
  float conf_;
};

struct DetectReadResult {
  std::string text_;
  cv::Rect bounding_box_;
};
