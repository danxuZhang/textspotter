#pragma once

#include <fmt/core.h>

#include <chrono>
#include <opencv2/core/core.hpp>
#include <string>

/**
 * @class ScopedTimer
 * @brief A utility class for timing the duration of scopes.
 *
 * @details This class records the time when it is instantiated and calculates the elapsed time when it goes out of
 * scope.
 */
class ScopedTimer {
 public:
  /**
   * @brief Constructs a new ScopedTimer with an optional name.
   * @param name An optional name for the timer scope. Defaults to an empty string.
   */
  explicit ScopedTimer(std::string_view name = "");

  /**
   * @brief Destroys the ScopedTimer and logs the elapsed time since construction.
   */
  ~ScopedTimer();

 private:
  std::string_view name_;
  std::chrono::high_resolution_clock::time_point start_;
};

/**
 * @class Timer
 * @brief A utility class for manual start and stop timing.
 *
 * @details This class provides methods to manually start and stop a timer and retrieve the elapsed time.
 */
class Timer {
 public:
  /**
   * @brief Constructs a new Timer.
   */
  Timer();

  /**
   * @brief Default destructor.
   */
  ~Timer() = default;

  /**
   * @brief Starts the timer.
   */
  auto Start() noexcept -> void;

  /**
   * @brief Stops the timer.
   */
  auto End() noexcept -> void;

  /**
   * @brief Gets the elapsed time in seconds between the start and end of the timer.
   * @return Elapsed time in seconds.
   */
  auto GetElapsedSeconds() const noexcept -> int;

 private:
  std::chrono::high_resolution_clock::time_point start_;
  std::chrono::high_resolution_clock::time_point end_;
};

/**
 * @function LoadImage
 * @brief Loads an image from a given file path.
 * @param image_path Path to the image file.
 * @return Loaded image as cv::Mat.
 */
auto LoadImage(const char *image_path) -> cv::Mat;

/**
 * @overload auto LoadImage(std::string_view image_path) -> cv::Mat
 */
auto LoadImage(std::string_view image_path) -> cv::Mat;

/**
 * @function Preprocess
 * @brief Preprocesses an image for further processing.
 * @param image The image to preprocess.
 * @return Preprocessed image as cv::Mat.
 */
auto Preprocess(const cv::Mat &image) noexcept -> cv::Mat;

/**
 * @function CalcLevenshteinDistance
 * @brief Calculates the Levenshtein distance between two strings.
 * @param s1 The first string.
 * @param s2 The second string.
 * @return The Levenshtein distance as an integer.
 */
auto CalcLevenshteinDistance(std::string_view s1, std::string_view s2) noexcept -> int;

/**
 * @function ToLower
 * @brief Converts a string to lowercase.
 * @param s The string to convert.
 * @return Lowercase version of the string.
 */
auto ToLower(std::string_view s) noexcept -> std::string;

/**
 * @function SplitStr
 * @brief Splits a string into a vector of strings based on whitespace.
 * @param s The string to split.
 * @return A vector of strings.
 */
auto SplitStr(const std::string &s) noexcept -> std::vector<std::string>;

/**
 * @function TrimStr
 * @brief Trims whitespace from the beginning and end of a string.
 * @param s The string to trim.
 * @return Trimmed string.
 */
auto TrimStr(const std::string &s) noexcept -> std::string;

/**
 * @function GetRectCenter
 * @brief Calculates the center point of a rectangle.
 * @param rect The rectangle to calculate the center for.
 * @return The center point of the rectangle.
 */
inline auto GetRectCenter(const cv::Rect &rect) noexcept -> cv::Point {
  return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}
