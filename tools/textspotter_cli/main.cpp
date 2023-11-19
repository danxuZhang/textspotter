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

void find_text(const cv::Mat &image, const char *model_path) {
  std::vector<DetectReadResult> res;
  {
    ScopedTimer timer("TextSpotter");
    res = DetectReadTextMultiThread(image, model_path, true);
  }

  for (const auto &r : res) {
    const auto center = GetRectCenter(r.bounding_box_);
    fmt::println("({}, {}): {}", center.x, center.y, r.text_);
  }
}

void match_word(const cv::Mat &image, const char *model_path, std::string_view target) {
  const auto pt = MatchWord(image, target, model_path);
  fmt::println("({}, {}): {}", pt.x, pt.y, target);
  auto canvas = image.clone();
  cv::line(canvas, {0, pt.y}, {canvas.size().width, pt.y}, {0, 255, 0});
  cv::line(canvas, {pt.x, 0}, {pt.x, canvas.size().height}, {0, 255, 0});
  cv::imshow("TextSpotter", canvas);
  cv::waitKey();
  cv::destroyAllWindows();
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotterCLI");
  parser.add_argument("image").help("path to image file").required();
  parser.add_argument("--dtm").help("east detection model path");
  parser.add_argument("--benchmark").help("run benchmark").flag();
  parser.add_argument("--find").help("detect and match all words").flag();
  parser.add_argument("--match-word").help("detect and match single word").flag();
  parser.add_argument("--target").help("target to match");

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

  if (parser["--benchmark"] == true) {
    benchmark(image, model_path.data());
    return 0;
  }

  if (parser["--find"] == true) {
    find_text(image, model_path.data());
    return 0;
  }

  if (parser["--match-word"] == true) {
    if (!parser.present("--target")) {
      fmt::println(stderr, "no match target");
      exit(1);
    }
    const auto target = parser.get<std::string>("--target");
    match_word(image, model_path.data(), target);
    return 0;
  }

  fmt::println(parser.help().str());
  return 0;
}
