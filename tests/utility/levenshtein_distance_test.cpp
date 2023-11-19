#include <gtest/gtest.h>
#include "textspotter/utility.hpp"

TEST(LevenshteinDistanceTest, EmptyStrings) { EXPECT_EQ(CalcLevenshteinDistance("", ""), 0); }

TEST(LevenshteinDistanceTest, FirstStringEmpty) { EXPECT_EQ(CalcLevenshteinDistance("", "test"), 4); }

TEST(LevenshteinDistanceTest, SecondStringEmpty) { EXPECT_EQ(CalcLevenshteinDistance("test", ""), 4); }

TEST(LevenshteinDistanceTest, SameStrings) { EXPECT_EQ(CalcLevenshteinDistance("test", "test"), 0); }

TEST(LevenshteinDistanceTest, DifferentStrings) { EXPECT_EQ(CalcLevenshteinDistance("kitten", "sitting"), 3); }
