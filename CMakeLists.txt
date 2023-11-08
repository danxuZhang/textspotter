cmake_minimum_required(VERSION 3.15...3.26)

project(
    textspotter
  VERSION 0.1
  LANGUAGES CXX)
set(THIS textspotter)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Leptonica REQUIRED)
find_package(Tesseract REQUIRED)
IF (WIN32)
    link_directories("C:\\Program Files (x86)\\tesseract\\lib")
ENDIF()

include(FetchContent)

# Fetch cxxopts
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        v3.1.1
)
FetchContent_MakeAvailable(cxxopts)

# Fetch fmt
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG       10.1.1
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

include_directories("include/")

set(SOURCE_FILES src/textspotter.cpp)


add_library(${THIS} SHARED ${SOURCE_FILES})

target_include_directories(${THIS} INTERFACE "include/")
target_link_libraries(${THIS} PRIVATE ${OpenCV_LIBS} ${Tesseract_LIBRARIES} leptonica)
