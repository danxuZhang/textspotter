cmake_minimum_required(VERSION 3.15...3.26)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
add_subdirectory(third-party/pybind11)

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

include_directories(include)

set(SOURCE_FILES src/textspotter_bindings.cpp src/textspotter.cpp)

python_add_library(_core MODULE  ${SOURCE_FILES} WITH_SOABI)
target_link_libraries(_core PRIVATE pybind11::headers)
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _core DESTINATION textspotter)