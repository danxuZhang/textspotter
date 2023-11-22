#include "c_bind.hpp"
#include "textspotter/textspotter.hpp"
#include "textspotter/utility.hpp"

auto detect_read_helper = [](const cv::Mat &image, char **texts, int *size) -> void {
  const auto res = DetectReadTextMultiThread(image);
  *size = res.size();

  texts = (char **)malloc(res.size() * sizeof(char *));
  for (size_t i = 0; i < res.size(); ++i) {
    const auto text = res[i].text_;
    texts[i] = (char *)malloc(text.size() * sizeof(char));
    strcpy(texts[i], text.c_str());
  }
};

DLL_EXPORT bool detect_read_text_from_file(const char *image_path, char **texts, int *size) {
  const auto image = LoadImage(image_path);
  detect_read_helper(image, texts, size);
  return true;
}

DLL_EXPORT bool detect_read_text(void *data, int height, int width, int type, char **texts, int *size) {
  const cv::Mat image(height, width, type, data);
  detect_read_helper(image, texts, size);
  return true;
}
