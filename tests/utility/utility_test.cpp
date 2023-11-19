#include "textspotter/utility.hpp"
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

TEST(HelloTest, HelloTest) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

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

TEST(LevenshteinDistanceTest, EmptyStrings) { EXPECT_EQ(CalcLevenshteinDistance("", ""), 0); }

TEST(LevenshteinDistanceTest, FirstStringEmpty) { EXPECT_EQ(CalcLevenshteinDistance("", "test"), 4); }

TEST(LevenshteinDistanceTest, SecondStringEmpty) { EXPECT_EQ(CalcLevenshteinDistance("test", ""), 4); }

TEST(LevenshteinDistanceTest, SameStrings) { EXPECT_EQ(CalcLevenshteinDistance("test", "test"), 0); }

TEST(LevenshteinDistanceTest, DifferentStrings) { EXPECT_EQ(CalcLevenshteinDistance("kitten", "sitting"), 3); }

TEST(RectCenterTest, ZeroRectangle) {
  cv::Rect rect(0, 0, 0, 0);
  EXPECT_EQ(GetRectCenter(rect), cv::Point(0, 0));
}

TEST(RectCenterTest, StandardRectangle) {
  cv::Rect rect(10, 10, 20, 20);
  EXPECT_EQ(GetRectCenter(rect), cv::Point(20, 20));
}

TEST(ToLowerTest, EmptyString) { EXPECT_EQ(ToLower(""), ""); }

TEST(ToLowerTest, AllLowerCase) { EXPECT_EQ(ToLower("test"), "test"); }

TEST(ToLowerTest, MixedCase) { EXPECT_EQ(ToLower("TeSt"), "test"); }

TEST(ToLowerTest, AllUpperCase) { EXPECT_EQ(ToLower("TEST"), "test"); }