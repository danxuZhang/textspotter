#include <gtest/gtest.h>
#include "textspotter/utility.hpp"

TEST(ToLowerTest, EmptyString) { EXPECT_EQ(ToLower(""), ""); }

TEST(ToLowerTest, AllLowerCase) { EXPECT_EQ(ToLower("test"), "test"); }

TEST(ToLowerTest, MixedCase) { EXPECT_EQ(ToLower("TeSt"), "test"); }

TEST(ToLowerTest, AllUpperCase) { EXPECT_EQ(ToLower("TEST"), "test"); }
