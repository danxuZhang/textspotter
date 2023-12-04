# Windows Setup Guide

## Prerequisite

1. Visual Studio with a C++17 compatible compiler (MSVC 19.15 or later)
2. CMake >= 3.14
3. Software Network SW client

## Build Dependencies

1. Download the source code of latest Leptonica and Tesseract release. Here we use [leptonica 1.83.1](https://github.com/DanBloomberg/leptonica/releases/tag/1.83.1) an [tesseract 5.3.3](https://github.com/tesseract-ocr/tesseract/releases/tag/5.3.3).

2. Extract the source to disk (`~\Documents\third_party\` for example).

### Install SW

1. Download the latest release of SW client from [here](https://software-network.org/client/).
2. Add path to SW client to system path.
3. Run `sw.exe setup` as administrator.

### Build Leptonica

1. Open terminal, navigate to leptonica source direcotry

    ```ps1
    cd ~\Documents\third_party\leptonica-1.83.1
    ```

2. Configure CMake project

    ```ps1
    cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
    ```

3. Build leptonica

    ``` ps1
    cmake --build cmake-build-release --config Release
    cmake --build cmake-build-debug --config Debug
    ```

4. Install leptonica

    By default, installation prefix is `C:\Program Fils (x86)\`, thus running powershell as administrator is required.

    ```ps1
    cmake --install cmake-build-release
    cmake --install cmake-build-debug
    ```

### Build Tesseract

1. Open powershell and navigate to tesseract source directory

    ```ps1
    cd ~\Documents\third_party\tesseract-5.3.3
    ```

2. Configure tesseract with cmake

    ```ps1
    cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release -DSW_BUILD=ON -DOPENMP_BUILD=ON
    cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -DSW_BUILD=ON -DOPENMP_BUILD=ON
    ```

    Note:

    * If cmake complains about cannot find leptonica, add `-DLeptonica_DIR=/path/to/leptonica/cmake` to the configuratio command

3. Build tesseract

    ```ps1
    cmake --build cmake-build-release --config release
    cmake --build cmake-build-debug --config debug
    ```

4. Install tesseract

    Similar to leptonica, installing to default prefix requires running powershell as administrator.

    ```ps1
    cmake --install cmake-build-release
    cmake --install cmake-build-debug
    ```

5. Prepare tesseract pre-trained model data

    1. Download pre-trained English model from [GitHub repo](https://github.com/tesseract-ocr/tessdata_best).
    2. Copy the downloaded model `eng.traineddata` to `C:\Program Fils (x86)\tesseract\share\tessdata`
    3. Modify environment variable to set `TESSDATA_PREFIX` to be the above mentioned director.

## Building TextSpotter from Powershell

1. Clone the repository and navigate to the directory

2. Crate a build directory

    ```ps1
    mkdir build && cd build
    ```

3. Configure textspotter with cmake

    ```ps1
    cmake .. 
    ```

If cmake cannot find package like OpenCV or Tesseract, add their respective path to cmake arguments like this:

```ps1
cmake .. -DOpnCV_DIR="C:\opencv\build\x64\vc16\lib" -DTesseract_DIR="C:\Program Files(X86)\tesseract\lib\cmake"
```

4. Build textspotter

    ```ps1
    cmake --build .
    ```

## Building TextSpotter from Visual Studio

1. Clone the repo and open with Visual Studio

2. Right click `CMakeLists.txt` and select `CMake Settings`.

3. Add configurations `x64-Release`.

4. Similar to powershell build, if package is not found, go to `CMake Settings` > `Command arguments` > `CMake command arguments` and add package dir manually.

5. Click `Build` > `Build All` on the top menu bar to build all target.

## Running Command Line Tools

### Running from terminal

Navigate to the build directory, go to `tools/` and can find respective applications in their folders.

Example usage:

```ps1
# run interative match to match text in the photo interactively
# to display matching result after each step, add --display flag
.\interactive_match\InteractiveMatch.exe --dtm /path/to/frozen_east_text_detection.pb /path/to/image --display
# find all texts on the photo

.\detect_text\DetectText.exe --dtm /path/to/frozen_east_text_detection.pb /path/to/image

# run benchmark to compare single and multi thread performance
.\benchmark\Benchmark.exe --dtm /path/to/frozen_east_text_detection.pb /path/to/image
```

### Running from Visual Studio

1. Open project in Visual Studio, select `x64-Release` build type, and select a target from the dropdown list, `InteractiveMatch.exe` for example.
2. Go to `Debug` > `Debug and Launch Settings for xxx` on the top menu bar.
3. Add arguments to the `launch.vs.json` file. For example:

    ```json
    {
        "version": "xxx",
        "configurations": [
            {
                "type": "default",
                "project": "CMakeLists.txt",
                "projectTarget": "InteractiveMatch.exe (tools\\interactive_match\\InteractiveMatch.exe)",
                "name": "Interactive Match",
                "args": [
                    "--dtm \\path\\to\\frozen_east_text_detection.pb \\path\\to\\image --display"
                ]
            }
        ]
    }
    ```

4. Click the arrow on the top bar to run.
