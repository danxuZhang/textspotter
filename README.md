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
