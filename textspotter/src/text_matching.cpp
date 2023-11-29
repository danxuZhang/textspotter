#include "textspotter/text_matching.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "textspotter/utility.hpp"

auto IsMatch(std::string_view s1, std::string_view s2, bool case_sensitive) noexcept -> bool {
  std::string str1(s1);
  std::string str2(s2);
  if (!case_sensitive) {
    str1 = ToLower(s1);
    str2 = ToLower(s2);
  }

  const auto min_length = std::min(str1.length(), str2.length());
  const auto edit_dist = CalcLevenshteinDistance(str1, str2);
  if (edit_dist > min_length) {
    return false;
  }

  return edit_dist < min_length / 2;
}

auto MatchWord(const std::vector<DetectReadResult> &detections, std::string_view target) noexcept -> cv::Point {
  for (const auto res : detections) {
    if (IsMatch(res.text_, target)) {
      return GetRectCenter(res.bounding_box_);
    }
  }

  return {-1, -1};
}

// Helper function to generate all combinations (Cartesian product)
void GenerateCombinations(const std::map<std::string, std::vector<cv::Rect>> &mp,
                          std::vector<std::vector<cv::Rect>> &combinations, std::vector<cv::Rect> &current,
                          std::vector<std::string>::const_iterator iter, const std::vector<std::string> &target) {
  if (iter == target.end()) {
    combinations.push_back(current);
    return;
  }
  for (const auto &p : mp) {
    if (!IsMatch(p.first, *iter)) {
      continue;
    }
    for (const auto &candidate : p.second) {
      current.push_back(candidate);
      GenerateCombinations(mp, combinations, current, std::next(iter), target);
      current.pop_back();
    }
  }
}

cv::Point CalculateCenter(const std::vector<cv::Rect> &sequence) {
  int x = 0, y = 0;
  for (const auto &rect : sequence) {
    x += rect.x + rect.width / 2;
    y += rect.y + rect.height / 2;
  }
  x /= sequence.size();
  y /= sequence.size();
  return cv::Point(x, y);
}

auto MatchWordGroups(const std::vector<DetectReadResult> &detections, const std::vector<std::string> &target) noexcept
    -> cv::Point {
  std::map<std::string, std::vector<cv::Rect>> mp;
  for (const auto &res : detections) {
    mp[res.text_].push_back(res.bounding_box_);
  }

  std::vector<std::vector<cv::Rect>> possible_sequences;
  std::vector<cv::Rect> sequence;

  GenerateCombinations(mp, possible_sequences, sequence, target.begin(), target);

  // Find the best matching sequence
  double minDistance = std::numeric_limits<double>::max();
  std::vector<cv::Rect> best_sequence;
  for (const auto &seq : possible_sequences) {
    double distance = 0;
    // Calculate the 'closeness' of the sequence
    for (size_t i = 0; i < seq.size(); ++i) {
      for (size_t j = i + 1; j < seq.size(); ++j) {
        cv::Point center_i = cv::Point(seq[i].x + seq[i].width / 2, seq[i].y + seq[i].height / 2);
        cv::Point center_j = cv::Point(seq[j].x + seq[j].width / 2, seq[j].y + seq[j].height / 2);
        distance += cv::norm(center_i - center_j);
      }
    }
    if (distance < minDistance) {
      minDistance = distance;
      best_sequence = seq;
    }
  }

  if (best_sequence.empty()) {
    return {-1, -1};
  }
  return CalculateCenter(best_sequence);
}
