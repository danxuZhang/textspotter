#include <fmt/core.h>

#include <argparse/argparse.hpp>
#include <opencv2/opencv.hpp>

#include "textspotter/detect_read.hpp"
#include "textspotter/text_matching.hpp"
#include "textspotter/utility.hpp"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotter::TextDetection");
  parser.add_argument("image").help("path to image").required();
  parser.add_argument("--dtm").help("path to east detection model").required();
  parser.add_argument("--multi-thread").help("enable multi-thread").flag();

  try {
    parser.parse_args(argc, argv);
  } catch (const std::exception &e) {
    fmt::println(stderr, e.what());
    fmt::println(stderr, parser.help().str());
    exit(1);
  }

  const auto image_path = parser.get<std::string>("image");
  const auto model_path = parser.get<std::string>("--dtm");

  cv::Mat image = LoadImage(image_path);

  if (parser["--multi-thread"] == true) {
    fmt::println("Multi-thread enabled");
    const auto res = DetectReadTextMultiThread(image, model_path, true);
    for (const auto &r : res) {
      fmt::println("{} @ ({}, {})", r.text_, r.bounding_box_.x, r.bounding_box_.y);
    }
  } else {
    const auto res = DetectReadText(image, model_path, true);
    for (const auto &r : res) {
      fmt::println("{} @ ({}, {})", r.text_, r.bounding_box_.x, r.bounding_box_.y);
    }
  }

  return 0;
}
