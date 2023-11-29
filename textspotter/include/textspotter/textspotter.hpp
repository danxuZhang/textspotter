#pragma once

#include <memory>
#include <opencv2/core.hpp>
#include <optional>
#include <string>

#include "textspotter/result_type.hpp"

class TextSpotter {
 public:
  explicit TextSpotter(std::string_view model_path = "frozen_east_text_detection.pb");
  ~TextSpotter() = default;

  auto LoadImage(std::string_view path) noexcept -> void;
  auto LoadImage(const cv::Mat &image) noexcept -> void;
  auto GetImage() const noexcept -> cv::Mat;

  auto DetectRead() noexcept -> std::vector<DetectReadResult>;
  auto MatchText(std::string_view target) const noexcept -> cv::Point;

 private:
  std::string model_path_;
  std::unique_ptr<cv::Mat> image_;
  std::vector<DetectReadResult> det_results_;
};
