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
 * This function attempts to detect and read text from the image located at the specified path.
 * The extracted texts are stored in an array, and the size of the array is updated to reflect
 * the number of texts detected.
 *
 * @param image_path A pointer to a character string representing the path to the image file.
 * @param texts A pointer to an array of character pointers, where the detected texts will be stored.
 *              The array should be pre-allocated, and this function will fill in the text values.
 * @param size A pointer to an integer that will be updated with the size of the texts array.
 * @return Returns true if the text detection and reading are successful, false otherwise.
 * @note It is the responsibility of the caller to allocate and free the memory for the texts array.
 *       Ensure the image file exists and is accessible from the given path.
 */
DLL_EXPORT bool detect_read_text_from_file(const char *image_path, char **texts, int *size);

/**
 * Detects and reads text from an image in memory.
 *
 * This function attempts to detect and read text from an image data provided in memory.
 * The image data should be in a specific format as defined by the type parameter. The extracted
 * texts are stored in an array, and the size of the array is updated to reflect the number of
 * texts detected.
 *
 * @param data A pointer to the image data in memory.
 * @param height The height of the image in pixels.
 * @param width The width of the image in pixels.
 * @param type An integer representing the image data type/format.
 * @param texts A pointer to an array of character pointers, where the detected texts will be stored.
 *              The array should be pre-allocated, and this function will fill in the text values.
 * @param size A pointer to an integer that will be updated with the size of the texts array.
 * @return Returns true if the text detection and reading are successful, false otherwise.
 * @note The caller is responsible for ensuring that the image data is in the correct format and
 *       for managing the memory for the texts array.
 */
DLL_EXPORT bool detect_read_text(void *data, int height, int width, int type, char **texts, int *size);
}
