#include "lib4c.hpp"

bool match_text_from_file(const char *image_path, const char *target, int *x, int *y) {
  cv::Mat image = LoadImage(image_path);
  auto pt = MatchText(image, target);
  *x = pt.x;
  *y = pt.y;
  return true;
}

bool match_text(const char *text, int height, int width, int type, void *data, int *x, int *y) {
  cv::Mat image{height, width, type, data};
  auto pt = MatchText(image, text);
  *x = pt.x;
  *y = pt.y;
  return true;
}
