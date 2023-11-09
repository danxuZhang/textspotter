#include "textspotter/textspotter.hpp"
#include <cctype>

TesseractApi::TesseractApi(const char *language) : api_(std::make_unique<tesseract::TessBaseAPI>()) {
  if (api_->Init(NULL, language) == -1) {
    throw std::runtime_error{"cannot initialize tesseract api"};
  }
  api_->SetVariable("debug_file", "tesseract.log");
}

TesseractApi::~TesseractApi() { api_->End(); }

auto LoadImage(const char *image_path) -> cv::Mat { return LoadImage(std::string_view(image_path)); }

auto LoadImage(std::string_view image_path) -> cv::Mat {
  if (image_path.empty()) {
    throw std::invalid_argument("image path cannot be empty");
  }

  cv::Mat image = cv::imread(image_path.data(), cv::IMREAD_COLOR);
  if (image.empty()) {
    throw std::invalid_argument("image at path " + std::string(image_path) + "is empty");
  }

  return image;
}

auto Preprocess(const cv::Mat &image) noexcept -> cv::Mat {
  cv::Mat gray_image;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
  } else {
    gray_image = image.clone();
  }

  cv::Mat inverted_image;
  cv::bitwise_not(gray_image, inverted_image);

  cv::Mat blurred_image;
  cv::GaussianBlur(inverted_image, blurred_image, cv::Size(5, 5), 0);

  cv::Mat threshold_image;
  cv::adaptiveThreshold(blurred_image, threshold_image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);

  // cv::imshow("Processed", threshold_image);
  // cv::waitKey();
  // cv::destroyAllWindows();
  return threshold_image;
}

auto RecognizeText(const cv::Mat &image, float conf_threshold) noexcept
    -> std::tuple<std::vector<std::string>, std::vector<cv::Rect>, std::vector<float>> {
  if (image.empty()) {
    return {};
  }

  auto tesseract = TesseractApi();

  cv::Mat processed_image = Preprocess(image);

  tesseract.api_->SetImage((uchar *)processed_image.data, processed_image.size().width, processed_image.size().height,
                           processed_image.channels(), processed_image.step1());
  tesseract.api_->Recognize(0);

  std::vector<cv::Rect> boxes;
  std::vector<std::string> texts;
  std::vector<float> confs;

  auto it = tesseract.api_->GetIterator();
  const tesseract::PageIteratorLevel level = tesseract::RIL_WORD;

  if (!it) {
    return {};
  }

  do {
    float conf = it->Confidence(level);

    if (conf < conf_threshold) {
      continue;
    }

    confs.push_back(conf);

    const char *word = it->GetUTF8Text(level);
    texts.emplace_back(word);

    int x1, y1, x2, y2;
    it->BoundingBox(level, &x1, &y1, &x2, &y2);
    boxes.emplace_back(cv::Point{x1, y1}, cv::Point{x2, y2});
  } while (it->Next(level));

  return {texts, boxes, confs};
}

auto MatchText(const cv::Mat &image, std::string_view target) noexcept -> cv::Point {
  auto &[texts, boxes, confs] = RecognizeText(image, 0);

  int min_distance = INT_MAX;
  std::unique_ptr<const std::string> best_match;
  cv::Rect match_box;

  for (auto i = 0; i < texts.size(); ++i) {
    const auto &text = texts[i];
    const cv::Rect box = boxes[i];
    float conf = confs[i];

    int distance = CalcLevenshteinDistance(text, target);
    if (text == target) {
      return GetRectCenter(box);
    }
    if (IsMatch(text, target)) {
      min_distance = distance;
      match_box = box;
      best_match = std::make_unique<const std::string>(text);
    }
  }

  if (best_match == nullptr) {
    return {-1, -1};
  }

  return GetRectCenter(match_box);
}

auto MatchText(const cv::Mat &image, const char *target) noexcept -> cv::Point {
  return MatchText(image, std::string_view(target));
}

auto MatchText(const char *image_path, const char *target) noexcept -> cv::Point {
  cv::Mat image = LoadImage(image_path);
  return MatchText(image, {target});
}

auto IsMatch(std::string_view s1, std::string_view s2) noexcept -> bool {
  if (s1 == s2) {
    return true;
  }

  int min_length = std::min(s1.length(), s2.length());
  int distance = CalcLevenshteinDistance(s1, s2);
  return distance < 1 / 4 * min_length;
}

auto CalcLevenshteinDistance(std::string_view s1, std::string_view s2) noexcept -> int {
  if (s1.empty()) {
    return s2.size();
  }

  if (s2.empty()) {
    return s1.size();
  }

  int l1 = s1.size();
  int l2 = s2.size();

  std::vector<std::vector<int>> matrix(l1 + 1, std::vector<int>(l2 + 1));

  for (auto i = 0; i <= l1; ++i) {
    matrix[i][0] = i;
  }

  for (auto j = 0; j <= l2; ++j) {
    matrix[0][j] = j;
  }

  for (auto i = 1; i <= l1; ++i) {
    for (auto j = 1; j <= l2; ++j) {
      int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

      matrix[i][j] = std::min({matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost});
    }
  }

  return matrix[l1][l2];
}

auto GetRectCenter(const cv::Rect &rect) noexcept -> cv::Point {
  return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}
