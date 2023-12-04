#include <fmt/core.h>

#include <argparse/argparse.hpp>
#include <opencv2/opencv.hpp>

#include "textspotter/detect_read.hpp"
#include "textspotter/text_matching.hpp"
#include "textspotter/utility.hpp"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotter::Benchmark");
  parser.add_argument("image").help("path to image").required();
  parser.add_argument("--dtm").help("path to east detection model").required();

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

  std::vector<DetectReadResult> res1;
  Timer single_thread_timer;
  single_thread_timer.Start();
  res1 = DetectReadText(image, model_path, false);
  single_thread_timer.End();

  std::vector<DetectReadResult> res2;
  Timer multi_thread_timer;
  multi_thread_timer.Start();
  res2 = DetectReadTextMultiThread(image, model_path, false);
  multi_thread_timer.End();

  std::map<std::string, int> freq;
  for (const auto &r : res1) {
    freq[r.text_]++;
  }
  for (const auto &r : res2) {
    freq[r.text_]--;
  }

  fmt::println("Single-thread detect and read {} words in {} seconds", res1.size(),
               single_thread_timer.GetElapsedSeconds());
  fmt::println("Multi-thread detect and read {} words in {} seconds", res2.size(),
               multi_thread_timer.GetElapsedSeconds());

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

  return 0;
}
