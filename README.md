<img src="logo/textspotter.png" alt="drawing" width="200"/>

# TextSpotter

![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5b0ccd8c880643209ed37d9fff0288d7)](https://app.codacy.com/gh/danxuZhang/textspotter/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

A Tesseract and EAST backed text detection and matching library.

## About TextSpotter

This project is a text detection and recognition system developed in C++.
It leverages the EAST (Efficient and Accurate Scene Text Detection) algorithm for detecting regions of interest (ROI)
containing text in images, followed by the use of Tesseract OCR for accurate Optical Character Recognition (OCR).
This combination provides a powerful tool for extracting written information from a variety of image sources.

### Motivation

This library is originally designed for interpreting text on device GUI screens via robot cameras for automated testing,
where the device tester needs to find or match certain text on the screen.
It addresses OCR errors, like confusing '0' with 'o', by using Tesseract and EAST for accurate text detection
and matching similar texts in images.

### About Tesseract

Tesseract is an open-source Optical Character Recognition (OCR) engine, renowned for its versatility and accuracy in
converting images of typed, handwritten, or printed text into machine-encoded text.
Initially developed by Hewlett-Packard in the late 1980s, Tesseract was later released as open-source software in 2005
and has since been
sponsored and maintained by Google.

Tesseract supports a wide range of languages and is compatible with various operating systems. Its ability to recognize
complex page layouts, various font types, and languages makes it a popular choice in the field of document image
analysis.
Tesseract can be used as a standalone OCR tool or integrated into other applications to enable text
recognition capabilities.
Over the years, it has evolved significantly, incorporating machine learning techniques for
improved accuracy, making it a go-to solution for OCR tasks in diverse applications ranging from document scanning to
automation processes.

### About EAST

The EAST (Efficient and Accurate Scene Text) detector is a popular and powerful tool used for detecting text in natural
scene images.
It stands out for its speed and accuracy, making it highly suitable for real-time applications.
The EAST detector utilizes a deep learning model, specifically a Convolutional Neural Network (CNN), to directly predict
words or
text lines in various orientations and sizes without the need for a separate text region proposal step.
This approach allows it to handle a wide range of text layouts and backgrounds, which is often challenging in complex
scene images.
Its ability to detect text quickly and accurately under various conditions has made it a widely used choice in the field
of computer vision, particularly in applications involving natural scene understanding and automated text analysis.

### Text Matching

The algorithm for matching similar text is based on edit distance or levenshtein distance, which is the minimum
number of single-character edits (insertions, deletions or substitutions) required to change one word into the other.

For example, the edit distance between *cat* and *bat* is 1, between *kitten* and *sitting* is 3.

## Key Features

1. **Text Detection:** Utilizes the EAST text detection model, known for its efficiency and accuracy in detecting text
   in natural scene images. This model quickly identifies and outputs the coordinates of text regions in a given image.

2. **Text Recognition:** Once the text regions are identified, Tesseract OCR is employed to read and interpret the text
   within these regions. Tesseract's advanced OCR capabilities allow for high accuracy in converting image-based text
   into machine-encoded text.

3. **User Input Search:** An additional functionality of this project is its ability to search for and locate specific
   text provided by the user in the image. This feature is particularly useful in scenarios where quick retrieval of
   certain text information from a large document or image is required.

## Setup

### Prerequisites

Building this library requires:

* A C++ compiler with C++17 support
* cmake >= 3.14
* OpenCV 4.8
* Tesseract 5.3
* Leptonica 1.83.1
* EAST text detector ([download model](https://www.dropbox.com/s/r2ingd0l3zt8hxs/frozen_east_text_detection.tar.gz?dl=1))

### Installation

If all dependencies are installed and setup properly, run:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

For more detailed setup guide including dependencies:

[Windows Setup Guide (vcpkg)](./doc/windows-vcpkg.md)

[Windows Setup Guide (build from source)](./doc/windows-build.md)

## Command Line Tool Usage

[Run with Visual Studio on Windows](./doc/windows-run.md)

A few command line tools are provided:

### Text Detection

Detect all texts in a photo.

```bash
# detect with single thread
./tools/DetectText --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png
# detect with multi thread
./tools/DetectText --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png --multi-thread
```

### Interactive Match Text

Detect and read all texts in a photo, interactively match user input text.

``` bash
# display matched location after each call
./tools/InteractiveMatch  --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png --display
# use multi-thread
./tools/InteractiveMatch  --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png --multi-thread
# no display
./tools/InteractiveMatch  --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png
```

Example output:

``` text
Detect and read 43 texts in 5 seconds

Start interactive matching: (type \quit to quit)   

> test
Found @ (203, 143) 
> hello world
Found @ (1012, 450) 
> \quit
```

### Benchmark

A utility tool for comparing performance of single-thread and multi-thread text detection and recognition performance.

``` bash
./tools/benchmark/Benchmark --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png 

Single-thread detect and read 45 words in 3 seconds
Multi-thread detect and read 45 words in 2 seconds
No mismatch between single and multi thread algo!
```

## API

### Data type

```c++
/**
 * @struct DetectReadResult
 * @brief Represents the result of a combined text detection and reading operation.
 *
 * @details This structure is used to store the result of operations where text is both detected and read,
 * storing the text and its location within the image.
 */
struct DetectReadResult {
  /**
   * @brief The text detected and read from the image.
   */
  std::string text_;

  /**
   * @brief The bounding box of the detected and read text within the image.
   */
  cv::Rect bounding_box_;
};
```

### Detect and recognize text (`detect_read.hpp`)

#### Detect and recognize text using single thread

```c++
/**
 * @function DetectReadText
 * @brief Detects and reads text from an image using a specified model.
 *
 * @param image The image (cv::Mat) from which text is to be detected and read.
 * @param model_path The path to the EAST model file. Defaults to "frozen_east_text_detection.pb".
 * @param display A flag indicating whether to display the detection results. Defaults to false.
 * @return A vector of DetectReadResult, each containing the detected and read text along with its bounding box.
 */
auto DetectReadText(const cv::Mat &image, std::string_view model_path = "frozen_east_text_detection.pb",
                    bool display = false) noexcept -> std::vector<DetectReadResult>;
```

#### Detect and recognize text using multiple thread(implemented with c++ async)

```c++
/**
 * @function DetectReadTextMultiThread
 * @brief Detects and reads text from an image using a specified model, employing multi-threading for improved performance.
 *
 * @param image The image (cv::Mat) from which text is to be detected and read.
 * @param model_path The path to the EAST model file. Defaults to "frozen_east_text_detection.pb".
 * @param display A flag indicating whether to display the detection results. Defaults to false.
 * @return A vector of DetectReadResult, each containing the detected and read text along with its bounding box.
 */
auto DetectReadTextMultiThread(const cv::Mat &image, std::string_view model_path = "frozen_east_text_detection.pb",
                               bool display = false) noexcept -> std::vector<DetectReadResult>;

```

### Match Text (`text_matching.hpp`)

#### Determine if two words matches

```c++
/**
 * @brief Compares two strings for a match, optionally case-sensitive.
 *
 * @param s1 The first string for comparison.
 * @param s2 The second string for comparison.
 * @param case_sensitive Whether to perform a case-sensitive comparison (default: false).
 * @return True if the strings match (or partially match based on case sensitivity), false otherwise.
 */
auto IsMatch(std::string_view s1, std::string_view s2, bool case_sensitive = false) noexcept -> bool;
```

#### Match a single target word and finds its center

```c++
/**
 * @brief Matches a target word in the list of text detections and returns its position.
 *
 * @param detections A vector of DetectReadResult objects representing detected and recognized text regions.
 * @param target The target word to match.
 * @return The position of the matched word as a cv::Point.
 */
auto MatchWord(const std::vector<DetectReadResult> &detections, std::string_view target) noexcept -> cv::Point;
```

#### Match a group of words and find their center

```c++
/**
 * @brief Matches a list of target words in the list of text detections and finds their center.
 *
 * @param detections A vector of DetectReadResult objects representing detected and recognized text regions.
 * @param target A vector of target words to match.
 * @return The position of the center of the target list as a cv::Point.
 */
auto MatchWordGroups(const std::vector<DetectReadResult> &detections, const std::vector<std::string> &target) noexcept
    -> cv::Point;
```

### TextSpotter (`textspotter.hpp`)

Combines detect, recognize and matching functions.

#### Load image

```c++
/**
 * @brief Loads an image from the specified file path.
 *
 * @param path The file path to the image.
 */
auto LoadImage(std::string_view path) noexcept -> void;

/**
 * @brief Loads an image from a provided OpenCV Mat object.
 *
 * @param image An OpenCV Mat representing the image.
 */
auto LoadImage(const cv::Mat &image) noexcept -> void;
```

#### Get Image

```c++
/**
 * @brief Gets the loaded image.
 *
 * @return The loaded image as an OpenCV Mat.
 */
auto GetImage() const noexcept -> cv::Mat;
```

#### Detect and read

``` c++
/**
 * @brief Detects and reads text in the loaded image.
 *
 * @return A vector of DetectReadResult objects representing detected and recognized text regions.
 */
auto DetectRead() noexcept -> std::vector<DetectReadResult>;
```

``` c++
/**
 * @brief Matches a target text in the loaded image and returns its position.
 *
 * @param target The target text to match.
 * @return The position of the matched text as a cv::Point.
 */
auto MatchText(std::string_view target) const noexcept -> cv::Point;
```

### Example Usage

```c++
// Create a TextSpotter object with default settings.
TextSpotter textSpotter;

// Load an image from a file.
textSpotter.LoadImage("image.jpg");

// Detect and read text in the loaded image.
std::vector<DetectReadResult> results = textSpotter.DetectRead();

// Match a target text in the image.
cv::Point matchPosition = textSpotter.MatchText("Target Text");
```
