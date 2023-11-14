#pragma once

#include <tesseract/baseapi.h>

#include <memory>
#include <opencv2/core.hpp>

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
  explicit TesseractApi(const char *language = "eng");
  ~TesseractApi();

  std::unique_ptr<tesseract::TessBaseAPI> api_;
};

/**
 * @brief Recognizes and extracts text from an image along with confidence scores and bounding rectangles.
 * @param image The image as a cv::Mat from which text is to be recognized.
 * @param conf_threshold The confidence threshold for the OCR process.
 * @return A tuple containing a vector of recognized strings, their corresponding bounding rectangles, and confidence
 * scores.
 */
auto RecognizeText(const cv::Mat &image, float conf_threshold) noexcept
  -> std::tuple<std::vector<std::string>, std::vector<cv::Rect>, std::vector<float>>;

