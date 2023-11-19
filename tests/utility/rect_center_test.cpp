#include <gtest/gtest.h>
#include "textspotter/utility.hpp"

TEST(RectCenterTest, ZeroRectangle) {
  cv::Rect rect(0, 0, 0, 0);
  EXPECT_EQ(GetRectCenter(rect), cv::Point(0, 0));
}

TEST(RectCenterTest, StandardRectangle1) {
  cv::Rect rect(10, 10, 20, 20);
  EXPECT_EQ(GetRectCenter(rect), cv::Point(20, 20));
}

TEST(RectCenterTest, StandardRectangle2) {
  cv::Rect rect(20, 20, 20, 20);
  EXPECT_EQ(GetRectCenter(rect), cv::Point(30, 30));
}
