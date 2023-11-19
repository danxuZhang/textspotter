#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "textspotter/utility.hpp"

class LoadImageTest : public ::testing::Test {
 protected:
  std::string temp_image_path;

  void SetUp() override {
    // Create a temporary file
    temp_image_path = std::tmpnam(nullptr);  // This generates a unique file name
    temp_image_path += ".png";               // Add a png extension

    // Create a simple image and save it
    cv::Mat testImage = cv::Mat::zeros(cv::Size(100, 100), CV_8UC3);
    cv::imwrite(temp_image_path, testImage);
  }

  void TearDown() override {
    // Delete the temporary file
    std::remove(temp_image_path.c_str());
  }
};

TEST_F(LoadImageTest, LoadValidImage) {
  // Act
  cv::Mat result;
  ASSERT_NO_THROW(result = LoadImage(temp_image_path.c_str()));

  // Assert
  EXPECT_FALSE(result.empty());
}

TEST_F(LoadImageTest, ThrowExceptionForEmptyCharPointer) {
  // Arrange
  const char *emptyImagePath = "";

  // Act & Assert
  EXPECT_THROW(LoadImage(emptyImagePath), std::invalid_argument);
}

TEST_F(LoadImageTest, ThrowExceptionForEmptyStringView) {
  // Arrange
  std::string_view emptyImagePath;

  // Act & Assert
  EXPECT_THROW(LoadImage(emptyImagePath), std::invalid_argument);
}

TEST_F(LoadImageTest, ThrowExceptionForInvalidPathCharPointer) {
  // Arrange
  const char *invalidImagePath = "non_existent_image.png";

  // Act & Assert
  EXPECT_THROW(LoadImage(invalidImagePath), std::invalid_argument);
}

TEST_F(LoadImageTest, ThrowExceptionForInvalidPathStringView) {
  // Arrange
  std::string_view invalidImagePath = "non_existent_image.png";

  // Act & Assert
  EXPECT_THROW(LoadImage(invalidImagePath), std::invalid_argument);
}
