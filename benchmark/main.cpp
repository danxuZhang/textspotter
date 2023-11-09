#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

#include "benchmark.hpp"
#include "textspotter/textspotter.hpp"

namespace fs = std::filesystem;

auto ReadTargets(const std::string &file_path) -> std::map<std::string, int> {
  if (!fs::exists(file_path) || fs::is_directory(file_path)) {
    throw std::invalid_argument("file at path " + file_path + " does not exist");
  }

  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::invalid_argument("could not open the file " + file_path);
  }

  std::map<std::string, int> text_count;
  std::string line;

  while (getline(file, line)) {
    text_count[line]++;
  }

  file.close();
  return text_count;
}

auto LoadImage(const std::string &file_path) -> cv::Mat {
  const cv::Mat image = cv::imread(file_path, cv::IMREAD_COLOR);
  if (image.empty()) {
    throw std::invalid_argument("image at path " + file_path + " is empty");
  }
  return image;
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser parser("TextSpotter Benchmark");
  parser.add_argument("--image").help("path to image file");
  parser.add_argument("--text").help("path to text file containing expected result");

  try {
    parser.parse_args(argc, argv);
  } catch (const std::exception &err) {
    fmt::println(stderr, err.what());
    fmt::println(stderr, parser.help().str());
    exit(1);
  }

  const auto image_path = parser.get<std::string>("--image");
  const auto text_path = parser.get<std::string>("--text");

  cv::Mat image = LoadImage(image_path);
  std::map<std::string, int> text_count = ReadTargets(text_path);

  BenchmarkRecognizeAccuracy(image, text_count);

  return 0;
}
