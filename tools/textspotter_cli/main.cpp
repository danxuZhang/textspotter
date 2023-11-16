#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include <opencv2/opencv.hpp>

#include "textspotter/textspotter.hpp"
#include "textspotter/utility.hpp"

void benchmark(const cv::Mat &image, const char *model_path) {
  std::vector<DetectReadResult> res1;
  {
    ScopedTimer timer("SingleThread");
    res1 = DetectReadText(image, model_path, false);
  }
  fmt::println("Detect and read {} words", res1.size());

  std::vector<DetectReadResult> res2;
  {
    ScopedTimer timer("MultiThread");
    res2 = DetectReadTextMultiThread(image, model_path, false);
  }
  fmt::println("Detect and read {} words", res2.size());

  std::map<std::string, int> freq;
  for (const auto &r : res1) {
    freq[r.text_]++;
  }
  for (const auto &r : res2) {
    freq[r.text_]--;
  }

  bool has_mismatched = false;
  for (const auto &p : freq) {
    if (p.second != 0) {
      has_mismatched = true;
      fmt::println(stderr, "Mismatched in word:{}", p.first);
    }
  }

  if (!has_mismatched) {
    fmt::println("No mismatch between single and multi thread algo!");
  }
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotterCLI");
  parser.add_argument("image").help("path to image file");
  parser.add_argument("--dtm").help("east detection model path");
  parser.add_argument("--benchmark").help("run benchmark");

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

  std::vector<DetectReadResult> res;
  {
    ScopedTimer timer("TextSpotter");
    res = DetectReadTextMultiThread(image, model_path.c_str(), true);
  }

  for (const auto &r : res) {
    const auto center = GetRectCenter(r.bounding_box_);
    fmt::println("({}, {}): {}", center.x, center.y, r.text_);
  }

  return 0;
}
