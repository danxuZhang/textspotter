#include "textspotter/textspotter.hpp"

#include <fmt/core.h>
#include <future>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>

#include "textspotter/east_detector.hpp"
#include "textspotter/ocr.hpp"
#include "textspotter/result_type.hpp"
#include "textspotter/utility.hpp"

auto DetectReadText(const cv::Mat &image, const char *model_path, bool display) noexcept
    -> std::vector<DetectReadResult> {
  cv::Mat target = image.clone();
  const EastTextDetector detector(model_path);
  const std::vector<TextDetectionResult> detection_results = detector.detect(image);

  std::vector<DetectReadResult> res;
  for (const auto &det_res : detection_results) {
    const auto &[roi, dt_conf] = det_res;
    cv::rectangle(target, roi, cv::Scalar(0, 0, 255));
    const auto &ocr_results = RecognizeText(image, 0, roi);

    for (const auto &ocr_res : ocr_results) {
      const auto &[text, box, ocr_conf] = ocr_res;
      res.push_back({text, box});
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

  return res;
}

auto DetectReadTextMultiThread(const cv::Mat &image, const char *model_path, bool display) noexcept
    -> std::vector<DetectReadResult> {
  cv::Mat target = image.clone();
  const EastTextDetector detector(model_path);
  const std::vector<TextDetectionResult> detection_results = detector.detect(image);

  std::vector<std::future<std::vector<OcrResult>>> future_results;
  for (const auto &det_res : detection_results) {
    const auto &[roi, dt_conf] = det_res;
    future_results.push_back(std::async(std::launch::async, [&]() { return RecognizeText(image, 0, roi); }));
  }

  std::vector<DetectReadResult> results;
  for (auto &f : future_results) {
    auto ocr_results = f.get();
    for (const auto &res : ocr_results) {
      const auto &[text, box, conf] = res;
      results.push_back({text, box});
      if (display) {
        cv::rectangle(target, box, cv::Scalar(0, 255, 0));
      }
    }
  }

  if (display) {
    cv::imshow("TextSpotter", target);
    cv::waitKey();
    cv::destroyWindow("TextSpotter");
  }

  return results;
}

auto MatchText(const cv::Mat &image, std::string_view target) noexcept -> cv::Point { return {}; }

auto MatchText(const cv::Mat &image, const char *target) noexcept -> cv::Point {
  return MatchText(image, std::string_view(target));
}

auto MatchText(const char *image_path, const char *target) noexcept -> cv::Point {
  const cv::Mat image = LoadImage(image_path);
  return MatchText(image, target);
}
