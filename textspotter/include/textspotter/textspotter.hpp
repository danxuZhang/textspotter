#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>
#include "result_type.hpp"

auto DetectReadText(const cv::Mat &image, const char *model_path = "frozen_east_text_detection.pb",
                    bool display = false) noexcept -> std::vector<DetectReadResult>;

auto DetectReadTextMultiThread(const cv::Mat &image, const char *model_path = "frozen_east_text_detection.pb",
                               bool display = false) noexcept -> std::vector<DetectReadResult>;
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
