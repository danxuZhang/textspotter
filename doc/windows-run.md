# Running Command Line Tools

## Running from terminal

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

## Running from Visual Studio

1. Open project in Visual Studio, select `x64-Release` build type, and select a target from the dropdown
   list, `InteractiveMatch.exe` for example.
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

4. Click the start button on the top bar to run.
