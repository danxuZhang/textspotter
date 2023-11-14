#pragma once

#include <opencv2/opencv.hpp>
#include <string>

#include "textspotter/utility.hpp"
#include "textspotter/text_detection.hpp"
#include "textspotter/ocr.hpp"

/**
 * @brief Matches a specified text string within an image and returns the location.
 * @param image The image as a cv::Mat in which to match the text.
 * @param target A string_view representing the text to match in the image.
 * @return A cv::Point representing the location of the matched text in the image.
 */
auto MatchText(const cv::Mat &image, std::string_view target) noexcept -> cv::Point;

/**
 * @brief Matches a specified text string within an image and returns the location.
 * @param image The image as a cv::Mat in which to match the text.
 * @param target A C-style string representing the text to match in the image.
 * @return A cv::Point representing the location of the matched text in the image.
 */
auto MatchText(const cv::Mat &image, const char *target) noexcept -> cv::Point;

/**
 * @brief Matches a specified text string within an image, loaded from a file path, and returns the location.
 * @param image_path The path to the image file as a C-style string.
 * @param target A C-style string representing the text to match in the image.
 * @return A cv::Point representing the location of the matched text in the image.
 */
auto MatchText(const char *image_path, const char *target) noexcept -> cv::Point;
