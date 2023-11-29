#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include <opencv2/opencv.hpp>

#include "textspotter/text_matching.hpp"
#include "textspotter/textspotter.hpp"
#include "textspotter/utility.hpp"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotter::InteractiveMatch");
  parser.add_argument("image").help("path to image").required();
  parser.add_argument("--dtm").help("path to east detection model").required();
  parser.add_argument("--display").help("display image after each matching").flag();

  try {
    parser.parse_args(argc, argv);
  } catch (const std::exception &e) {
    fmt::println(stderr, e.what());
    fmt::println(stderr, parser.help().str());
    exit(1);
  }

  const auto image_path = parser.get<std::string>("image");
  const auto model_path = parser.get<std::string>("--dtm");
  const auto display = parser["--display"] == true;

  const auto image = LoadImage(image_path);

  Timer timer;
  timer.Start();
  const auto detect_read_result = DetectReadTextMultiThread(image, model_path.c_str());
  timer.End();

  fmt::println("Detect and read {} texts in {} seconds", detect_read_result.size(), timer.GetElapsedSeconds());
  fmt::println("Start interactive matching: (type \\quit to quit)");

  cv::Point pt;
  std::string target;
  while (true) {
    fmt::print("> ");
    std::getline(std::cin, target);
    if (target == "\\quit") {
      break;
    }

    target = TrimStr(target);
    const auto tokens = SplitStr(target);
    if (tokens.size() == 1) {
      pt = MatchWord(detect_read_result, tokens[0]);
    } else {
      pt = MatchWordGroups(detect_read_result, tokens);
    }

    fmt::println("Found @ ({}, {})", pt.x, pt.y);

    if (display) {
      const auto canvas = image.clone();
      cv::line(canvas, {0, pt.y}, {canvas.size().width, pt.y}, {0, 255, 0});
      cv::line(canvas, {pt.x, 0}, {pt.x, canvas.size().height}, {0, 255, 0});
      cv::imshow("TextSpotter", canvas);
      cv::waitKey();
      cv::destroyWindow("TextSpotter");
    }
  }

  return 0;
}
