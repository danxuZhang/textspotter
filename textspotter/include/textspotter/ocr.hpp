#pragma once

#include <tesseract/baseapi.h>

#include <memory>
#include <opencv2/core.hpp>
#include <optional>

#include "textspotter/result_type.hpp"

/**
 * @function RecognizeText
 * @brief Recognizes and extracts text from an image using OCR (Optical Character Recognition).
 *
 * @details This function utilizes the Tesseract OCR engine to recognize and extract text from
 * the given image. It returns the recognized text along with their confidence scores and bounding rectangles.
 *
 * @param image The image (cv::Mat) from which text is to be recognized.
 * @param conf_threshold The confidence threshold for the OCR process. Text with confidence below this threshold will be
 * discarded.
 * @param roi Optional region of interest within the image where text recognition should be performed. Defaults to
 * std::nullopt (whole image).
 * @return A vector of OcrResult, each containing recognized text, its bounding rectangle, and confidence score.
 * @throws This function is noexcept and does not throw exceptions.
 */
auto RecognizeText(const cv::Mat &image, float conf_threshold, std::optional<cv::Rect> roi = std::nullopt) noexcept
    -> std::vector<OcrResult>;

/**
 * @class TesseractApi
 * @brief Encapsulates the Tesseract OCR API for text recognition.
 *
 * @details This class provides an interface to the Tesseract OCR engine. It initializes the Tesseract
 * engine with the specified language and provides a mechanism to perform OCR on images.
 */
class TesseractApi {
 public:
  /**
   * @brief Constructs a new Tesseract API instance.
   *
   * @details This constructor initializes the Tesseract OCR engine with the specified language.
   * If no language is specified, it defaults to English ("eng").
   *
   * @param language The language code for OCR. Defaults to "eng" (English) if not specified.
   */
  explicit TesseractApi(const char *language = "eng");

  /**
   * @brief Destroys the TesseractApi instance.
   *
   * @details This destructor ensures proper cleanup of resources allocated by the Tesseract OCR engine.
   */
  ~TesseractApi();

  /**
   * @brief Grants the RecognizeText function access to the private members of TesseractApi.
   *
   * @details This friend declaration allows the RecognizeText function to access the private
   * members of the TesseractApi class, such as the Tesseract engine instance, for text recognition purposes.
   */
  friend auto RecognizeText(const cv::Mat &image, float conf_threshold, std::optional<cv::Rect> roi) noexcept
      -> std::vector<OcrResult>;

 private:
  /**
   * @brief A unique pointer to the Tesseract OCR engine instance.
   *
   * @details This pointer holds and manages the Tesseract OCR engine instance used for text recognition.
   */
  std::unique_ptr<tesseract::TessBaseAPI> api_;
};
