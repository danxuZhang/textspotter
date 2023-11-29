#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>

#include "result_type.hpp"

/**
 * @function DetectReadText
 * @brief Detects and reads text from an image using a specified model.
 *
 * @details This function processes an image to detect and read text using the specified EAST (Efficient and
 * Accurate Scene Text Detection) model file. The function can optionally display the detection results.
 *
 * @param image The image (cv::Mat) from which text is to be detected and read.
 * @param model_path The path to the EAST model file. Defaults to "frozen_east_text_detection.pb".
 * @param display A flag indicating whether to display the detection results. Defaults to false.
 * @return A vector of DetectReadResult, each containing the detected and read text along with its bounding box.
 * @throws This function is noexcept and does not throw exceptions.
 */
auto DetectReadText(const cv::Mat &image, std::string_view model_path = "frozen_east_text_detection.pb",
                    bool display = false) noexcept -> std::vector<DetectReadResult>;

/**
 * @function DetectReadTextMultiThread
 * @brief Detects and reads text from an image using a specified model, employing multi-threading for improved
 * performance.
 *
 * @details Similar to DetectReadText, this function uses the EAST model to detect and read text from an image.
 * It is optimized for performance by using multiple threads. The function can optionally display the detection results.
 *
 * @param image The image (cv::Mat) from which text is to be detected and read.
 * @param model_path The path to the EAST model file. Defaults to "frozen_east_text_detection.pb".
 * @param display A flag indicating whether to display the detection results. Defaults to false.
 * @return A vector of DetectReadResult, each containing the detected and read text along with its bounding box.
 * @throws This function is noexcept and does not throw exceptions.
 */
auto DetectReadTextMultiThread(const cv::Mat &image, std::string_view model_path = "frozen_east_text_detection.pb",
                               bool display = false) noexcept -> std::vector<DetectReadResult>;
