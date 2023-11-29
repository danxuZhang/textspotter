#pragma once

#include <opencv2/core.hpp>
#include <string>

/**
 * @struct TextDetectionResult
 * @brief Represents the result of a text detection operation.
 *
 * @details This structure holds the details of a detected text instance including its bounding box
 * and a confidence score.
 */
struct TextDetectionResult {
  /**
   * @brief The bounding box of the detected text within the image.
   */
  cv::Rect bounding_box_;

  /**
   * @brief The confidence score of the detected text.
   *
   * @details This value represents how confident the detection algorithm is that the detected
   * region contains text. Higher values indicate greater confidence.
   */
  float conf_;
};

/**
 * @struct OcrResult
 * @brief Represents the result of an OCR (Optical Character Recognition) operation.
 *
 * @details This structure holds the recognized text, its bounding box in the image, and the confidence score of the
 * recognition.
 */
struct OcrResult {
  /**
   * @brief The text recognized by the OCR process.
   */
  std::string text_;

  /**
   * @brief The bounding box of the recognized text within the image.
   */
  cv::Rect bounding_box_;

  /**
   * @brief The confidence score of the recognized text.
   *
   * @details This value represents the confidence of the OCR algorithm in its recognition accuracy.
   * Higher scores indicate more accurate recognition.
   */
  float conf_;
};

/**
 * @struct DetectReadResult
 * @brief Represents the result of a combined text detection and reading operation.
 *
 * @details This structure is used to store the result of operations where text is both detected and read,
 * storing the text and its location within the image.
 */
struct DetectReadResult {
  /**
   * @brief The text detected and read from the image.
   */
  std::string text_;

  /**
   * @brief The bounding box of the detected and read text within the image.
   */
  cv::Rect bounding_box_;
};
