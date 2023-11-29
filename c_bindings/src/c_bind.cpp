#include "c_bind.hpp"
#include "textspotter/text_matching.hpp"
#include "textspotter/textspotter.hpp"
#include "textspotter/utility.hpp"

auto detect_read_helper = [](const cv::Mat &image, char **texts, int *size) -> bool {
  const auto res = DetectReadTextMultiThread(image);
  *size = res.size();

  texts = (char **)malloc(res.size() * sizeof(char *));
  if (texts == nullptr) {
    return false;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    const auto text = res[i].text_;
    texts[i] = (char *)malloc(text.size() * sizeof(char));
    strcpy(texts[i], text.c_str());
  }

  return true;
};

DLL_EXPORT bool detect_read_text_from_file(const char *image_path, char **texts, int *size) {
  const auto image = LoadImage(image_path);
  return detect_read_helper(image, texts, size);
}

DLL_EXPORT bool detect_read_text(void *data, int height, int width, int type, char **texts, int *size) {
  const cv::Mat image(height, width, type, data);
  return detect_read_helper(image, texts, size);
}

DLL_EXPORT bool match_word(int size, char **texts, int *xs, int *ys, int *widths, int *heights, char *target,
                           int *x_out, int *y_out) {
  std::vector<DetectReadResult> detections;
  for (int i = 0; i < size; ++i) {
    detections.push_back({std::string(texts[i]), cv::Rect(xs[i], ys[i], widths[i], heights[i])});
  }

  const auto pt = MatchWord(detections, {target});
  *x_out = pt.x;
  *y_out = pt.y;
  if (pt == cv::Point{-1, -1}) {
    return false;
  }
  return true;
}

DLL_EXPORT bool match_text(int size, char **texts, int *xs, int *ys, int *widths, int *heights, char *target,
                           int *x_out, int *y_out) {
  std::vector<DetectReadResult> detections;
  for (int i = 0; i < size; ++i) {
    detections.push_back({std::string(texts[i]), cv::Rect(xs[i], ys[i], widths[i], heights[i])});
  }

  std::vector<std::string> tokens;
  std::istringstream iss(target);
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }

  const auto pt = MatchWordGroups(detections, tokens);
  *x_out = pt.x;
  *y_out = pt.y;
  if (pt == cv::Point{-1, -1}) {
    return false;
  }
  return true;
}
