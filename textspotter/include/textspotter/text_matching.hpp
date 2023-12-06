#pragma once

#include <opencv2/core.hpp>
#include <string>

#include "textspotter/result_type.hpp"

/**
 * @brief Compares two strings for a match, optionally case-sensitive.
 *
 * @param s1 The first string for comparison.
 * @param s2 The second string for comparison.
 * @param case_sensitive Whether to perform a case-sensitive comparison (default: false).
 * @return True if the strings match (or partially match based on case sensitivity), false otherwise.
 */
auto IsMatch(std::string_view s1, std::string_view s2, bool case_sensitive = false) noexcept -> bool;

/**
 * @brief Matches a target word in the list of text detections and returns its position.
 *
 * @param detections A vector of DetectReadResult objects representing detected and recognized text regions.
 * @param target The target word to match.
 * @return The position of the matched word as a cv::Point.
 */
auto MatchWord(const std::vector<DetectReadResult> &detections, std::string_view target) noexcept -> cv::Point;

/**
 * @brief Matches a list of target words in the list of text detections and returns the position of the first match
 * found.
 *
 * @param detections A vector of DetectReadResult objects representing detected and recognized text regions.
 * @param target A vector of target words to match.
 * @return The position of the first matched word from the target list as a cv::Point.
 */
auto MatchWordGroups(const std::vector<DetectReadResult> &detections, const std::vector<std::string> &target) noexcept
    -> cv::Point;
