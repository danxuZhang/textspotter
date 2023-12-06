#pragma once

#include <memory>
#include <opencv2/core.hpp>
#include <optional>
#include <string>

#include "textspotter/result_type.hpp"

/**
 * @brief The TextSpotter class is designed for text detection and recognition in images.
 * It uses the EAST (Efficient and Accurate Scene Text Detector) model for text detection.
 * After loading an image, you can perform text detection, recognition and matching operations.
 */
class TextSpotter {
 public:
  /**
   * @brief Constructs a TextSpotter object with optional parameters.
   *
   * @param model_path The file path to the frozen EAST text detection model (default: "frozen_east_text_detection.pb").
   * @param enable_multi_thread Whether to enable multi-threading for detection (default: true).
   */
  explicit TextSpotter(std::string_view model_path = "frozen_east_text_detection.pb", bool enable_multi_thread = true);

  /**
   * @brief Destroys the TextSpotter object.
   */
  ~TextSpotter() = default;

  /**
   * @brief Loads an image from the specified file path.
   *
   * @param path The file path to the image.
   */
  auto LoadImage(std::string_view path) noexcept -> void;

  /**
   * @brief Loads an image from a provided OpenCV Mat object.
   *
   * @param image An OpenCV Mat representing the image.
   */
  auto LoadImage(const cv::Mat &image) noexcept -> void;

  /**
   * @brief Gets the loaded image.
   *
   * @return The loaded image as an OpenCV Mat.
   */
  auto GetImage() const noexcept -> cv::Mat;

  /**
   * @brief Detects and reads text in the loaded image.
   *
   * @return A vector of DetectReadResult objects representing detected and recognized text regions.
   */
  auto DetectRead() noexcept -> std::vector<DetectReadResult>;

  /**
   * @brief Matches a target text in the loaded image and returns its position.
   *
   * @param target The target text to match.
   * @return The position of the matched text as a cv::Point.
   */
  auto MatchText(std::string_view target) const noexcept -> cv::Point;

 private:
  bool enable_multi_thread_;                   // Whether multi-threading is enabled for detection.
  std::string model_path_;                     // The file path to the EAST model.
  std::unique_ptr<cv::Mat> image_;             // The loaded image.
  std::vector<DetectReadResult> det_results_;  // Detected and recognized text results.
};
