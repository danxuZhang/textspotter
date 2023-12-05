# Windows Setup with vcpkg

[vcpkg](https://vcpkg.io/en/) is a free C/C++ package manager for acquiring and managing libraries. Choose from over
1500 open source libraries to download and build in a single step or add your own private libraries to simplify your
build process. Maintained by the Microsoft C++ team and open source contributors.

## Setup vcpkg

[vcpkg Installation Guide](https://vcpkg.io/en/getting-started)

For simplicity and easier typing, we install vcpkg in `C:\vcpkg`.

1. Clone the vcpkg repo

    ``` ps1
    cd C:\
    git clone https://github.com/Microsoft/vcpkg.git
    ```

1. Run the bootstrap script to build vcpkg.

    ```ps1
    .\vcpkg\bootstrap-vcpkg.bat
    ```

1. Using vcpkg with MSBuild / Visual Studio (may require elevation)

    ```ps1
    .\vcpkg.exe integrate install
    ```

1. Set enviornment variable `VCPKG_ROOT` to be `C:\vcpkg`.

## Install tesseract with vcpkg

(This may take some time)

```ps1
.\vcpkg.exe install tesseract --triplet x64-windows
```

## Use vcpkg installed tesseract in command line

```ps1
cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build
```

## Use vcpkg installed tesseract in Visual Studio

Add `cmakeToolchain` setting to cmake preset json file.

``` json
"cmakeToolchain": "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
```

An example `CMakeSettings.json`:

```json
{
  "configurations": [
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "RelWithDebInfo",
      "buildRoot": "${projectDir}\\out\\build\\${name}",
      "installRoot": "${projectDir}\\out\\install\\${name}",
      "cmakeToolchain": "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake",
      "ctestCommandArgs": "",
      "inheritEnvironments": [
        "msvc_x64_x64"
      ]
    }
  ]
}
```
