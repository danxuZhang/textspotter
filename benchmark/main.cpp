#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

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

auto RunBenchmark(const cv::Mat &image, const std::map<std::string, int> &text_count) {
  auto local_count = text_count;
  const cv::Point nil{-1, -1};

  auto start = std::chrono::high_resolution_clock::now();
  for (auto &p : local_count) {
    int x, y;
    match_text(p.first.c_str(), image.size().height, image.size().width, image.type(), image.data, &x, &y);
    if (x != -1 && y != -1) {
      p.second--;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  int total_match = 0;
  int total_text = 0;
  for (const auto &p : local_count) {
    const std::string &text = p.first;
    int total = text_count.at(text);
    int matched = p.second;
    if (matched == total) {
      total_match++;
    }
    total_text++;
    fmt::println("matched {} out of {} for text {}", matched, total, text);
  }

  fmt::println("In total, matched {} out of {} ({}\%) in {} seconds", total_match, total_text,
               (double)total_match / total_text * 100, elapsed.count());
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

  RunBenchmark(image, text_count);

  return 0;
}
