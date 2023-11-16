#include "textspotter/textspotter.hpp"

#include <fmt/core.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>

#include "textspotter/ocr.hpp"
#include "textspotter/text_detection.hpp"
#include "textspotter/utility.hpp"

auto DetectText(const cv::Mat &image, const char *model_path, bool display) noexcept -> void {
  cv::Mat target = image.clone();
  const EastTextDetector detector(model_path);
  const std::vector<DetectionResult> detection_results = detector.detect(image);

  for (const auto &det_res : detection_results) {
    const auto &[roi, dt_conf] = det_res;
    cv::rectangle(target, roi, cv::Scalar(0, 0, 255));
    const auto &ocr_results = RecognizeText(image, 0, roi);

    for (const auto &ocr_res : ocr_results) {
      const auto &[text, box, ocr_conf] = ocr_res;
      fmt::println("Text: {}", ocr_res.text_);
      if (display) {
        cv::rectangle(target, box, cv::Scalar(0, 255, 0));
      }
    }
  }

  if (display) {
    cv::imshow("TextSpotter", target);
    cv::waitKey();
    cv::destroyAllWindows();
  }
}

auto MatchText(const cv::Mat &image, std::string_view target) noexcept -> cv::Point {
  const auto ocr_result = RecognizeText(image, 0);

  std::unique_ptr<const std::string> best_match;
  cv::Rect match_box;

  for (const auto &i : ocr_result) {
    const auto &text = i.text_;
    const cv::Rect box = i.box_;
    float conf = i.conf_;

    if (text == target) {
      return GetRectCenter(box);
    }
    //    if (IsMatch(text, target)) {
    //      match_box = box;
    //      best_match = std::make_unique<const std::string>(text);
    //    }
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
  return MatchText(image, target);
}
