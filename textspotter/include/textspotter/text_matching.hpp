#pragma once

#include <opencv2/core.hpp>
#include <string>

#include "textspotter/result_type.hpp"

auto IsMatch(std::string_view s1, std::string_view s2, bool case_sensitive = false) noexcept -> bool;

auto MatchWord(const std::vector<DetectReadResult> &detections, std::string_view target) noexcept -> cv::Point;

auto MatchWordGroups(const std::vector<DetectReadResult> &detections, const std::vector<std::string> &target,
                     std::vector<cv::Rect> &boxes) noexcept -> cv::Point;
