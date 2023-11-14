#pragma once
#include <fmt/core.h>
#include <chrono>
#include <map>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <thread>

#include "textspotter/textspotter.hpp"

class SafeCounter {
 private:
  int value_;
  mutable std::mutex mtx_;

 public:
  SafeCounter() : value_(0) {}  // Constructor initializes the counter to 0.

  // Increment the counter.
  void Increment() {
    std::lock_guard<std::mutex> lock(mtx_);
    ++value_;
  }

  // Decrement the counter.
  void Decrement() {
    std::lock_guard<std::mutex> lock(mtx_);
    --value_;
  }

  // Get the current value of the counter.
  int Get() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return value_;
  }
};

auto BenchmarkRecognizeAccuracy(const cv::Mat &image, std::map<std::string, int> text_count) noexcept -> void;

auto BenchmarkRecognizeSpeed(const cv::Mat &image, std::map<std::string, int> text_count) noexcept -> void;
