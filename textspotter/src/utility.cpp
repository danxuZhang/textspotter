#include "textspotter/utility.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

auto LoadImage(const char *image_path) -> cv::Mat { return LoadImage(std::string_view(image_path)); }

auto LoadImage(std::string_view image_path) -> cv::Mat {
  if (image_path.empty()) {
    throw std::invalid_argument("image path cannot be empty");
  }

  cv::Mat image = cv::imread(image_path.data(), cv::IMREAD_COLOR);
  if (image.empty()) {
    throw std::invalid_argument("image at path " + std::string(image_path) + "is empty");
  }

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

  cv::GaussianBlur(processed_image, processed_image, cv::Size(3, 3), 0);

  cv::adaptiveThreshold(processed_image, processed_image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 2);

  cv::fastNlMeansDenoising(processed_image, processed_image);

  cv::imshow("Processed", processed_image);
  cv::waitKey();
  cv::destroyAllWindows();
  return processed_image;
}

inline auto IsMatch(std::string_view s1, std::string_view s2) noexcept -> bool {
  std::string s1_lower(s1);
  std::string s2_lower(s2);
  for (auto &c : s1_lower) {
    c = std::tolower(c);
  }
  for (auto &c : s2_lower) {
    c = std::tolower(c);
  }

  if (s1_lower == s2_lower) {
    return true;
  }

  int min_length = std::min(s1_lower.length(), s2_lower.length());
  int distance = CalcLevenshteinDistance(s1_lower, s2_lower);
  return distance < (1 / 2 * min_length);
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

auto GetRectCenter(const cv::Rect &rect) noexcept -> cv::Point {
  return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}
