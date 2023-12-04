#include "textspotter/textspotter.hpp"

#include <opencv2/imgcodecs.hpp>

#include "textspotter/detect_read.hpp"
#include "textspotter/text_matching.hpp"
#include "textspotter/utility.hpp"

TextSpotter::TextSpotter(std::string_view path, bool enable_multi_thread)
    : model_path_(path), image_(nullptr), enable_multi_thread_(enable_multi_thread) {}

auto TextSpotter::LoadImage(std::string_view path) noexcept -> void {
  const auto image = cv::imread(path.data(), cv::IMREAD_COLOR);
  image_ = image.empty() ? nullptr : std::make_unique<cv::Mat>(image);
}

auto TextSpotter::LoadImage(const cv::Mat &image) noexcept -> void {
  image_ = image.empty() ? nullptr : std::make_unique<cv::Mat>(image.clone());
}

auto TextSpotter::GetImage() const noexcept -> cv::Mat { return image_ == nullptr ? cv::Mat() : image_->clone(); }

auto TextSpotter::DetectRead() noexcept -> std::vector<DetectReadResult> {
  if (image_ == nullptr) {
    return {};
  }
  if (enable_multi_thread_) {
    det_results_ = std::move(DetectReadTextMultiThread(*image_, model_path_, false));
  } else {
    det_results_ = std::move(DetectReadText(*image_, model_path_, false));
  }
  return det_results_;
}

auto TextSpotter::MatchText(std::string_view target) const noexcept -> cv::Point {
  if (image_ == nullptr) {
    return {-1, -1};
  }

  cv::Point pt;
  const auto tokens = SplitStr(std::string(target));
  if (tokens.size() == 1) {
    pt = MatchWord(det_results_, tokens[0]);
  } else {
    pt = MatchWordGroups(det_results_, tokens);
  }

  return pt;
}
