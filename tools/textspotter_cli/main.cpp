#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include <opencv2/opencv.hpp>

#include "textspotter/textspotter.hpp"
#include "textspotter/utility.hpp"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotterCLI");
  parser.add_argument("image").help("path to image file");
  parser.add_argument("--detect").help("detect text");
  parser.add_argument("--ocr").help("perform ocr");
  parser.add_argument("--dtm").help("east detection model path");

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

  auto single_start = std::chrono::high_resolution_clock::now();
  DetectText(image, model_path.c_str(), false);
  auto single_end = std::chrono::high_resolution_clock ::now();
  auto single_thread_elapsed = single_end - single_start;
  fmt::println("Single Thread: {} seconds", single_thread_elapsed.count());

  return 0;
}
