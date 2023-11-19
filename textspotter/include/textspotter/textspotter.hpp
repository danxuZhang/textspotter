#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>
#include "result_type.hpp"

auto DetectReadText(const cv::Mat &image, std::string_view model_path = "frozen_east_text_detection.pb",
                    bool display = false) noexcept -> std::vector<DetectReadResult>;

auto DetectReadTextMultiThread(const cv::Mat &image, std::string_view model_path = "frozen_east_text_detection.pb",
                               bool display = false) noexcept -> std::vector<DetectReadResult>;

auto MatchWord(const cv::Mat &image, std::string_view target,
               std::string_view model_path = "frozen_east_text_detection.pb") noexcept -> cv::Point;
