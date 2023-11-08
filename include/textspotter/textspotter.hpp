#pragma once

#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>
#include <string>

class TesseractApi {
 public:
  TesseractApi(const char *language = "eng");
  TesseractApi(std::string_view language);
  ~TesseractApi();

  std::unique_ptr<tesseract::TessBaseAPI> api_;
};

auto LoadImage(const char *image_path) -> cv::Mat;
auto LoadImage(std::string_view image_path) -> cv::Mat;

auto Preprocess(const cv::Mat &image) noexcept -> cv::Mat;

auto RecognizeText(const cv::Mat &image, float conf_threshold) noexcept
    -> std::tuple<std::vector<std::string>, std::vector<cv::Rect>, std::vector<float>>;

auto MatchText(const cv::Mat &image, std::string_view target) noexcept -> cv::Point;
auto MatchText(const cv::Mat &image, const char *target) noexcept -> cv::Point;
auto MatchText(const char *image_path, const char *target) noexcept -> cv::Point;

auto CalcLevenshteinDistance(std::string_view s1, std::string_view s2) noexcept -> int;

auto GetRectCenter(const cv::Rect &rect) noexcept -> cv::Point;

extern "C" {
_declspec(dllexport) bool match_text_from_file(const char *image_path, const char *target, int *x, int *y);
_declspec(dllexport) bool match_text(const cv::Mat image, const char *target, int *x, int *y);
}
