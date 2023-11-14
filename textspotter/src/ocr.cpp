#include "textspotter/ocr.hpp"

#include "textspotter/utility.hpp"

TesseractApi::TesseractApi(const char *language)
  : api_(std::make_unique<tesseract::TessBaseAPI>()) {
  if (api_->Init(nullptr, language) == -1) {
    throw std::runtime_error{"cannot initialize tesseract api"};
  }
  api_->SetVariable("debug_file", "tesseract.log");
  // api_->SetVariable("lstm_choice_mode", "2");
}

TesseractApi::~TesseractApi() { api_->End(); }


auto RecognizeText(const cv::Mat &image, const float conf_threshold) noexcept
  -> std::tuple<std::vector<std::string>, std::vector<cv::Rect>, std::vector<float>> {
  if (image.empty()) {
    return {};
  }

  const auto tesseract = TesseractApi();

  const auto processed_image = Preprocess(image);

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
