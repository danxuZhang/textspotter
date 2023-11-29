#include "textspotter/utility.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

ScopedTimer::ScopedTimer(std::string_view name) : name_(name), start_(std::chrono::high_resolution_clock::now()) {}

ScopedTimer::~ScopedTimer() {
  const auto end = std::chrono::high_resolution_clock::now();
  const auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start_);
  fmt::println("Duration of timer {}: {}", name_, time_span.count());
}

Timer::Timer() : start_(std::chrono::high_resolution_clock::now()) {}

auto Timer::Start() noexcept -> void { start_ = std::chrono::high_resolution_clock::now(); }

auto Timer::End() noexcept -> void { end_ = std::chrono::high_resolution_clock::now(); }

auto Timer::GetElapsedSeconds() const noexcept -> int {
  return std::chrono::duration_cast<std::chrono::duration<double>>(end_ - start_).count();
}

auto LoadImage(const char *image_path) -> cv::Mat { return LoadImage(std::string_view(image_path)); }

auto LoadImage(std::string_view image_path) -> cv::Mat {
  if (image_path.empty()) {
    throw std::invalid_argument("image path cannot be empty");
  }

  cv::Mat image = cv::imread(image_path.data(), cv::IMREAD_COLOR);
  if (image.empty()) {
    throw std::invalid_argument("image at path " + std::string(image_path) + "is empty");
  }

  cv::resize(image, image, cv::Size(1280, 720));

  return image;
}

auto Preprocess(const cv::Mat &image) noexcept -> cv::Mat {
  cv::Mat processed_image;
  if (image.channels() == 3) {
    cv::cvtColor(image, processed_image, cv::COLOR_BGR2GRAY);
  } else if (image.channels() == 4) {
    cv::cvtColor(image, processed_image, cv::COLOR_BGRA2GRAY);
  } else {
    processed_image = image;
  }

  cv::bitwise_not(processed_image, processed_image);

  cv::GaussianBlur(processed_image, processed_image, cv::Size(5, 5), 0);

  cv::adaptiveThreshold(processed_image, processed_image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 5, 3);

  cv::fastNlMeansDenoising(processed_image, processed_image);

  // cv::imshow("processed", processed_image);
  // cv::waitKey();
  // cv::destroyWindow("processed");
  return processed_image;
}

auto CalcLevenshteinDistance(std::string_view s1, std::string_view s2) noexcept -> int {
  if (s1.empty()) {
    return s2.size();
  }

  if (s2.empty()) {
    return s1.size();
  }

  int l1 = s1.size();
  int l2 = s2.size();

  std::vector<std::vector<int>> matrix(l1 + 1, std::vector<int>(l2 + 1));

  for (auto i = 0; i <= l1; ++i) {
    matrix[i][0] = i;
  }

  for (auto j = 0; j <= l2; ++j) {
    matrix[0][j] = j;
  }

  for (auto i = 1; i <= l1; ++i) {
    for (auto j = 1; j <= l2; ++j) {
      int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

      matrix[i][j] = std::min({matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost});
    }
  }

  return matrix[l1][l2];
}

auto SplitStr(const std::string &s) noexcept -> std::vector<std::string> {
  std::istringstream iss(s);
  std::vector<std::string> tokens;
  std::string token;

  while (iss >> token) {
    tokens.push_back(token);
  }

  return tokens;
}

auto ToLower(std::string_view s) noexcept -> std::string {
  std::string res(s);
  std::transform(s.begin(), s.end(), res.begin(), ::tolower);
  return res;
}

static inline std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(" \t\n\r\f\v");
  return (start == std::string::npos) ? "" : s.substr(start);
}

static inline std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(" \t\n\r\f\v");
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

static inline std::string trim(const std::string &s) { return ltrim(rtrim(s)); }

auto inline TrimStr(const std::string &s) noexcept -> std::string { return trim(s); }
