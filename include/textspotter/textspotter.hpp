#pragma once

#include <tesseract/baseapi.h>
#include <algorithm>
#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

/**
 * @class TesseractApi
 * @brief This class encapsulates the Tesseract OCR API for recognizing text in images.
 *
 * @details It initializes the Tesseract API with the specified language.
 */
class TesseractApi {
 public:
  /**
   * @brief Constructs a new Tesseract API instance using the default or provided language.
   * @param language The language code for OCR. Defaults to English ("eng") if not specified.
   */
  TesseractApi(const char *language = "eng");
  ~TesseractApi();

  std::unique_ptr<tesseract::TessBaseAPI> api_;
};

/**
 * @brief Loads an image from a file path into an OpenCV matrix (cv::Mat).
 * @param image_path The path to the image file as a C-style string.
 * @return A cv::Mat object containing the loaded image.
 */
auto LoadImage(const char *image_path) -> cv::Mat;
/**
 * @brief Loads an image from a file path into an OpenCV matrix (cv::Mat).
 * @param image_path The path to the image file as a std::string_view.
 * @return A cv::Mat object containing the loaded image.
 */
auto LoadImage(std::string_view image_path) -> cv::Mat;

/**
 * @brief Preprocesses an image for OCR processing.
 * @param image The input cv::Mat image to preprocess.
 * @return A cv::Mat object containing the preprocessed image.
 */
auto Preprocess(const cv::Mat &image) noexcept -> cv::Mat;

/**
 * @brief Recognizes and extracts text from an image along with confidence scores and bounding rectangles.
 * @param image The image as a cv::Mat from which text is to be recognized.
 * @param conf_threshold The confidence threshold for the OCR process.
 * @return A tuple containing a vector of recognized strings, their corresponding bounding rectangles, and confidence
 * scores.
 */
auto RecognizeText(const cv::Mat &image, float conf_threshold) noexcept
    -> std::tuple<std::vector<std::string>, std::vector<cv::Rect>, std::vector<float>>;

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

/**
 * @brief Calculates the Levenshtein distance between two strings.
 * @param s1 The first string as a std::string_view.
 * @param s2 The second string as a std::string_view.
 * @return An integer representing the Levenshtein distance (edit distance) between the two strings.
 */
auto CalcLevenshteinDistance(std::string_view s1, std::string_view s2) noexcept -> int;

/**
 * @brief Calculates the center point of a given rectangle.
 * @param rect The cv::Rect structure from which to calculate the center.
 * @return A cv::Point representing the center of the rectangle.
 */
auto GetRectCenter(const cv::Rect &rect) noexcept -> cv::Point;

extern "C" {
/**
 * @brief Matches text from a file and returns the coordinates of the match.
 * @param image_path Path to the image file containing the text.
 * @param target The text to search for within the image.
 * @param x Pointer to an integer to store the x-coordinate of the match.
 * @param y Pointer to an integer to store the y-coordinate of the match.
 * @return True if the text was successfully matched and coordinates set; otherwise, false.
 */
_declspec(dllexport) bool match_text_from_file(const char *image_path, const char *target, int *x, int *y);
}
