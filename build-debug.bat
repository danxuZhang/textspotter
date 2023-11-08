@echo off
SETLOCAL EnableDelayedExpansion

REM Step 1: Create a directory called 'build' and enter it
if not exist "build" (
    mkdir build
)
cd build

REM Step 2: Execute cmake with the specified build type
cmake -DCMAKE_BUILD_TYPE=Debug ..

REM Step 3: Build using cmake
cmake --build . --config Debug

REM Step 4: Copy the textspotter.dll to the specified location
if exist "Debug\textspotter.dll" (
    if not exist "..\python\textspotter\lib" (
        mkdir "..\python\textspotter\lib"
    )
    copy /Y "Debug\textspotter.dll" "..\python\textspotter\lib"
) else (
    echo Error: textspotter.dll not found in the Debug directory.
)

echo Batch process complete.
pause
ENDLOCAL