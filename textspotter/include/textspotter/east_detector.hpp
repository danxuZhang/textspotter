#pragma once

#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>

#include "textspotter/result_type.hpp"

/**
 * @class EastTextDetector
 * @brief A class for detecting text in images using the EAST (Efficient and Accurate Scene Text Detection) model.
 *
 * This class provides methods to load and use the EAST model for text detection in images.
 */

class EastTextDetector {
 public:
  /**
   * @brief Constructor that initializes the EastTextDetector with a given model path and optional parameters.
   * @param model_path Path to the EAST model file.
   * @param conf_threshold Confidence threshold for the detection, defaults to 0.5.
   * @param nms_threshold Non-maximum suppression threshold, defaults to 0.4.
   * @param width Width to resize the image to before feeding it into the model, defaults to 640.
   * @param height Height to resize the image to before feeding it into the model, defaults to 320.
   * @param detect_scale Scale factor for the detection, defaults to 1.0.
   * @param detect_mean Scalar with mean values to be subtracted from each image channel, defaults to {123.68, 116.78,
   * 103.94}.
   * @param swap_rb Flag to swap the Red and Blue channels of the image, defaults to true.
   */
  explicit EastTextDetector(const char *model_path, float conf_threshold = 0.5, float nms_threshold = 0.4,
                            int width = 640, int height = 320, double detect_scale = 1.0,
                            const cv::Scalar &detect_mean = cv::Scalar{123.68, 116.78, 103.94}, bool swap_rb = true);

  /**
   * @brief Constructor that initializes the EastTextDetector with a unique pointer to an EAST detection model.
   * @param detector Unique pointer to a pre-configured EAST text detection model.
   */
  explicit EastTextDetector(std::unique_ptr<cv::dnn::TextDetectionModel_EAST> detector);

  /**
   * @brief Default destructor.
   */
  ~EastTextDetector() = default;

  /**
   * @brief Detects text in a given image.
   * @param image Image in which to detect text.
   * @return A vector of TextDetectionResult objects, each representing a detected text instance.
   * @throws This method is noexcept and does not throw exceptions.
   */
  auto detect(const cv::Mat &image) const noexcept -> std::vector<TextDetectionResult>;

 private:
  /**
   * @brief Unique pointer to the EAST text detection model.
   */
  std::unique_ptr<cv::dnn::TextDetectionModel_EAST> detector_;
};
