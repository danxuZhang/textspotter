enable_testing()

add_executable(utility_test
        utility/utility_test.cpp
        utility/load_image_test.cpp
        utility/levenshtein_distance_test.cpp
        utility/rect_center_test.cpp
        utility/case_conversion_test.cpp
)
target_link_libraries(utility_test GTest::gtest_main libtextspotter fmt::fmt)

include(GoogleTest)
gtest_discover_tests(utility_test)
