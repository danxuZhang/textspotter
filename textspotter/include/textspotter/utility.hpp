#pragma once

#include <fmt/core.h>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <string>

class ScopedTimer {
 public:
  explicit ScopedTimer(std::string_view name = "");
  ~ScopedTimer();

 private:
  std::string_view name_;
  std::chrono::high_resolution_clock::time_point start_;
};

class Timer {
 public:
  Timer();
  ~Timer() = default;
  auto Start() noexcept -> void;
  auto End() noexcept -> void;

  auto GetElapsedSeconds() const noexcept -> int;

 private:
  std::chrono::high_resolution_clock::time_point start_;
  std::chrono::high_resolution_clock::time_point end_;
};

/**
 * @brief Loads an image from a file path into an OpenCV matrix (cv::Mat).
 * @param image_path The path to the image file as a C-style string.
 * @return A cv::Mat object containing the loaded image.
 */
auto LoadImage(const char *image_path) -> cv::Mat;

/**
 * @brief Loads an image from a file path into an OpenCV matrix (cv::Mat).
 * @param image_path The path to the image file as a std::string_view.
 * @return A cv::Mat object containing the loaded image.
 */
auto LoadImage(std::string_view image_path) -> cv::Mat;

/**
 * @brief Preprocesses an image for OCR processing.
 * @param image The input cv::Mat image to preprocess.
 * @return A cv::Mat object containing the preprocessed image.
 */
auto Preprocess(const cv::Mat &image) noexcept -> cv::Mat;

/**
 * @brief Calculates the Levenshtein distance between two strings.
 * @param s1 The first string as a std::string_view.
 * @param s2 The second string as a std::string_view.
 * @return An integer representing the Levenshtein distance (edit distance) between the two strings.
 */
auto CalcLevenshteinDistance(std::string_view s1, std::string_view s2) noexcept -> int;

/**
 * @brief Calculates the center point of a given rectangle.
 * @param rect The cv::Rect structure from which to calculate the center.
 * @return A cv::Point representing the center of the rectangle.
 */
inline auto GetRectCenter(const cv::Rect &rect) noexcept -> cv::Point {
  return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

auto ToLower(std::string_view s) noexcept -> std::string;

auto SplitStr(const std::string &s) noexcept -> std::vector<std::string>;

auto TrimStr(const std::string &s) noexcept -> std::string;
