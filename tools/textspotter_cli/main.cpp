#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include <opencv2/opencv.hpp>

#include "textspotter/textspotter.hpp"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotterCLI");
  parser.add_argument("image").help("path to image file");
  parser.add_argument("--detect").help("detect text");
  parser.add_argument("--ocr").help("perform ocr");

  try {
    parser.parse_args(argc, argv);
  } catch (const std::exception &e) {
    fmt::println(stderr, e.what());
    fmt::println(stderr, parser.help().str());
    exit(1);
  }

  const auto image_path = parser.get<std::string>("image");

  cv::Mat image = LoadImage(image_path);
  const auto &[texts, boxes, confs] = RecognizeText(image, 0);

  for (const auto &box : boxes) {
    cv::rectangle(image, box, cv::Scalar(0, 255, 0));
  }

  // cv::namedWindow("TextSpotterCLI", cv::WINDOW_AUTOSIZE);
  cv::imshow("TextSpotterCLI", image);
  cv::waitKey();

  cv::destroyAllWindows();
  return 0;
}
