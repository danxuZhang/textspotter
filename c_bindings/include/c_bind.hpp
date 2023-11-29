#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
/**
 * Detects and reads text from an image file.
 *
 * @param image_path The path to the image file.
 * @param texts Pointer to an array of strings where the detected texts will be stored.
 * @param size Pointer to an integer where the size (number of detected texts) will be stored.
 *
 * @return true if the text detection and reading are successful, false otherwise.
 */
DLL_EXPORT bool detect_read_text_from_file(const char *image_path, char **texts, int *size);

/**
 * Detects and reads text from image data in memory.
 *
 * @param data Pointer to the image data in memory.
 * @param height The height of the image in pixels.
 * @param width The width of the image in pixels.
 * @param type The type of the image data, typically representing the color depth and channels.
 * @param texts Pointer to an array of strings where the detected texts will be stored.
 * @param size Pointer to an integer where the size (number of detected texts) will be stored.
 *
 * @return true if the text detection and reading are successful, false otherwise.
 */
DLL_EXPORT bool detect_read_text(void *data, int height, int width, int type, char **texts, int *size);

/**
 * Detects and reads text from image data in memory.
 *
 * @param data Pointer to the image data in memory.
 * @param height The height of the image in pixels.
 * @param width The width of the image in pixels.
 * @param type The type of the image data, typically representing the color depth and channels.
 * @param texts Pointer to an array of strings where the detected texts will be stored.
 * @param size Pointer to an integer where the size (number of detected texts) will be stored.
 *
 * @return true if the text detection and reading are successful, false otherwise.
 */
DLL_EXPORT bool detect_read_text(void *data, int height, int width, int type, char **texts, int *size);

/**
 * Matches a specific text or phrase within the detected texts and provides its coordinates.
 *
 * @param size The number of detected texts.
 * @param texts Array of strings containing detected texts.
 * @param xs Array of x-coordinates for each text.
 * @param ys Array of y-coordinates for each text.
 * @param widths Array of widths for each text's bounding box.
 * @param heights Array of heights for each text's bounding box.
 * @param target The text or phrase to match.
 * @param x_out Pointer to an integer where the x-coordinate of the matched text will be stored.
 * @param y_out Pointer to an integer where the y-coordinate of the matched text will be stored.
 *
 * @return true if the text or phrase is found, false otherwise.
 */
DLL_EXPORT bool match_text(int size, char **texts, int *xs, int *ys, int *widths, int *heights, char *target,
                           int *x_out, int *y_out);
}
