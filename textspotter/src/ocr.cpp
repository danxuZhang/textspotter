#include "textspotter/ocr.hpp"

#include "textspotter/utility.hpp"

TesseractApi::TesseractApi(const char *language) : api_(std::make_unique<tesseract::TessBaseAPI>()) {
  if (api_->Init(nullptr, language) == -1) {
    throw std::runtime_error{"cannot initialize tesseract api"};
  }
  api_->SetVariable("debug_file", "tesseract.log");
  // api_->SetVariable("lstm_choice_mode", "2");
}

TesseractApi::~TesseractApi() { api_->End(); }

auto RecognizeText(const cv::Mat &image, float conf_threshold, std::optional<cv::Rect> roi) noexcept
    -> std::vector<OcrResult> {
  if (image.empty()) {
    return {};
  }

  const auto tesseract = TesseractApi();

  // const auto processed_image = Preprocess(image);

  tesseract.api_->SetImage(image.data, image.size().width, image.size().height, image.channels(), image.step1());
  if (roi != std::nullopt) {
    tesseract.api_->SetRectangle(roi->x, roi->y, roi->width, roi->height);
  }
  tesseract.api_->Recognize(nullptr);

  std::vector<OcrResult> result;

  const auto it = tesseract.api_->GetIterator();
  constexpr tesseract::PageIteratorLevel level = tesseract::RIL_WORD;

  if (!it) {
    return {};
  }

  do {
    float conf = it->Confidence(level);

    if (conf < conf_threshold) {
      continue;
    }

    const char *word = it->GetUTF8Text(level);
    if (word == nullptr) {
      continue;
    }
    const std::string word_str(word);
    if (word_str.empty()) {
      continue;
    }

    int x1, y1, x2, y2;
    it->BoundingBox(level, &x1, &y1, &x2, &y2);
    const cv::Rect box(cv::Point(x1, y1), cv::Point(x2, y2));
    result.push_back({word, box, conf});
  } while (it->Next(level));

  return result;
}
