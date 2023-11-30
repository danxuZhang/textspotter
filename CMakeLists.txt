cmake_minimum_required(VERSION 3.14)

project(
        textspotter
        VERSION 0.1
        LANGUAGES CXX)
set(THIS textspotter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to use pkg-config
option(use_pkgconfig "Use pkg-config to find packages" OFF)

if (use_pkgconfig)
    # Check if pkg-config is available and use it
    find_package(PkgConfig REQUIRED)
    pkg_search_module(Tesseract REQUIRED tesseract)
    pkg_search_module(LEPTONICA REQUIRED lept)
else()
    # Use find_package
    find_package(Leptonica REQUIRED)
    find_package(Tesseract REQUIRED)
    if (WIN32)
        link_directories("C:\\Program Files (x86)\\tesseract\\lib")
    endif()
endif()

find_package(OpenCV REQUIRED)

include(FetchContent)

# Fetch cxxopts
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse
        GIT_TAG v3.0
)
FetchContent_MakeAvailable(argparse)

# Fetch fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.1
)
FetchContent_MakeAvailable(fmt)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${Leptonica_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(textspotter/)
add_subdirectory(tools/interactive_match/)
add_subdirectory(tools/benchmark/)
add_subdirectory(tools/detect_text/)
add_subdirectory(tests/)
