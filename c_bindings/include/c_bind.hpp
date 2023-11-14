#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
/**
 * @brief Matches text from a file and returns the coordinates of the match.
 * @param image_path Path to the image file containing the text.
 * @param target The text to search for within the image.
 * @param x Pointer to an integer to store the x-coordinate of the match.
 * @param y Pointer to an integer to store the y-coordinate of the match.
 * @return True if the text was successfully matched and coordinates set; otherwise, false.
 */
DLL_EXPORT bool match_text_from_file(const char *image_path, const char *target, int *x, int *y);
/**
 * @brief Matches text within the given image data in memory and returns the coordinates of the match.
 *
 * This function searches for the specified target text within an image that is provided
 * as raw data in memory. The image data is expected to be in a specific format, defined by
 * the width, height, and type parameters. If the text is found, the function returns true
 * and sets the x and y coordinates to the starting point of the matched text within the image.
 *
 * @param target The text to search for within the image data.
 * @param height The height of the image in pixels.
 * @param width The width of the image in pixels.
 * @param type An integer indicating the image data type and format (e.g., grayscale, RGB).
 * @param data A pointer to the image data in memory. The format of this data must match
 *             the type specified.
 * @param x Pointer to an integer to store the x-coordinate of the match.
 * @param y Pointer to an integer to store the y-coordinate of the match.
 *
 * @return True if the text was successfully matched and coordinates set; otherwise, false.
 */
DLL_EXPORT bool match_text(const char *target, int height, int width, int type, void *data, int *x, int *y);
}
