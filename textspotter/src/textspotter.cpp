#include "textspotter/textspotter.hpp"
#include <cctype>


auto MatchText(const cv::Mat &image, std::string_view target) noexcept -> cv::Point {
  const auto &[texts, boxes, confs] = RecognizeText(image, 0);

  std::unique_ptr<const std::string> best_match;
  cv::Rect match_box;

  for (auto i = 0; i < texts.size(); ++i) {
    const auto &text = texts[i];
    const cv::Rect box = boxes[i];
    float conf = confs[i];

    if (text == target) {
      return GetRectCenter(box);
    }
    if (IsMatch(text, target)) {
      match_box = box;
      best_match = std::make_unique<const std::string>(text);
    }
  }

  if (best_match == nullptr) {
    return {-1, -1};
  }

  return GetRectCenter(match_box);
}

auto MatchText(const cv::Mat &image, const char *target) noexcept -> cv::Point {
  return MatchText(image, std::string_view(target));
}

auto MatchText(const char *image_path, const char *target) noexcept -> cv::Point {
  const cv::Mat image = LoadImage(image_path);
  return MatchText(image, {target});
}
