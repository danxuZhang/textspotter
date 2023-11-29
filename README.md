<img src="logo/textspotter.png" alt="drawing" width="200"/>

# TextSpotter

A Tesseract and EAST backed text detection and matching library.

## About

This project is a robust text detection and recognition system developed in C++.
It leverages the EAST (Efficient and Accurate Scene Text Detection) algorithm for detecting regions of interest (ROI)
containing text in images, followed by the use of Tesseract OCR for accurate Optical Character Recognition (OCR).
This combination provides a powerful tool for extracting written information from a variety of image sources.

### Key Features

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

* A C++17 compatible compiler (GCC 8, Clang 5, MSVC 19.15 or later)
* cmake >= 3.14
* OpenCV 4.8
* Tesseract 5.3
* Leptonica 1.83.1
* EAST text detector

### Installation

For macOS and Linux:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j`nproc`
```

For Windows, a separate guide using Visual Studio can be found here.

## Command Line Usage

A few command line tools are provided:

### Text Detection

Detect all texts in a photo.

```bash
cd build/
./tools/DetectText --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png
```

### Interactive Match Text

Detect and read all texts in a photo, interactively match user input text.

``` bash
# display matched location after each call
./tools/InteractiveMatch  --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png --display
# no display
./tools/InteractiveMatch  --dtm /path/to/frozen_east_text_detection.pb /path/to/image.png
```

Example output:

``` text
Detect and read 43 texts in 25 seconds

Start interactive matching: (type \quit to quit)   

> test
Found @ (203, 143) 
> hello world
Found @ (1012, 450) 
> \quit
```
