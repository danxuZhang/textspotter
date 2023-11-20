#include "c_bind.hpp"
#include "textspotter/textspotter.hpp"
#include "textspotter/utility.hpp"

bool match_text_from_file(const char *image_path, const char *target, int *x, int *y) {
  cv::Mat image = LoadImage(image_path);
  auto pt = MatchWord(image, target);
  *x = pt.x;
  *y = pt.y;
  return true;
}

bool match_text(const char *text, int height, int width, int type, void *data, int *x, int *y) {
  cv::Mat image{height, width, type, data};
  auto pt = MatchWord(image, text);
  *x = pt.x;
  *y = pt.y;
  return true;
}
