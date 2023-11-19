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

auto DetectReadText(const cv::Mat &image, std::string_view model_path, bool display) noexcept
    -> std::vector<DetectReadResult> {
  cv::Mat target = image.clone();
  const EastTextDetector detector(model_path.data());
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

auto DetectReadTextMultiThread(const cv::Mat &image, std::string_view model_path, bool display) noexcept
    -> std::vector<DetectReadResult> {
  cv::Mat target = image.clone();
  const EastTextDetector detector(model_path.data());
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

auto MatchWord(const cv::Mat &image, std::string_view target, std::string_view model_path) noexcept -> cv::Point {
  auto is_match = [](std::string_view s1, std::string_view s2) -> bool {
    if (s1 == s2) {
      return true;
    }

    const int dist = CalcLevenshteinDistance(s1, s2);
    const size_t min_len = std::min(s1.length(), s2.length());
    return dist < 1 / 2 * min_len;
  };

  const auto &result = DetectReadTextMultiThread(image, model_path);
  for (const auto &r : result) {
    const auto &[text, box] = r;
    if (is_match(target, text)) {
      return GetRectCenter(box);
    }
  }

  return {-1, -1};
}
