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
# Option to build tests
option(build_test "Build Tests" OFF)

if (use_pkgconfig)
    # Check if pkg-config is available and use it
    find_package(PkgConfig REQUIRED)
    pkg_search_module(Tesseract REQUIRED tesseract)
    pkg_search_module(LEPTONICA REQUIRED lept)
else ()
    # Use find_package
    find_package(Leptonica REQUIRED)
    find_package(Tesseract REQUIRED)
endif ()

find_package(OpenCV REQUIRED)

include(FetchContent)

find_package(argparse QUIET)
find_package(fmt QUIET)

# Check if argparse was found, if not fetch it
if (NOT argparse_FOUND)
    include(FetchContent)

    FetchContent_Declare(
            argparse
            GIT_REPOSITORY https://github.com/p-ranav/argparse
            GIT_TAG v3.0
    )
    FetchContent_MakeAvailable(argparse)
endif ()

# Check if fmt was found, if not fetch it
if (NOT fmt_FOUND)
    include(FetchContent)

    FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG 10.1.1
    )
    FetchContent_MakeAvailable(fmt)
endif ()

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${Leptonica_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
)

if (build_test)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests/)
endif ()

add_subdirectory(textspotter/)
add_subdirectory(tools/interactive_match/)
add_subdirectory(tools/benchmark/)
add_subdirectory(tools/detect_text/)
