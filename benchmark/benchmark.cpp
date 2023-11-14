#include "benchmark.hpp"

auto BenchmarkRecognizeAccuracy(const cv::Mat &image, std::map<std::string, int> text_count) noexcept -> void {
  auto start = std::chrono::high_resolution_clock::now();

  const auto &[texts, boxes, confs] = RecognizeText(image, 0);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  fmt::println("Elapsed time for Recognition: {} sec", elapsed.count());

  start = std::chrono::high_resolution_clock::now();

  SafeCounter total;
  SafeCounter matched;
  std::vector<std::string> unmatched;
  for (const auto &p : text_count) {
    bool has_matched{false};
    for (const auto &text : texts) {
      // fmt::println("{} vs {}", p.first, text);
      if (IsMatch(text, p.first)) {
        matched.Increment();
        has_matched = true;
        break;
      }
    }
    if (!has_matched) {
      unmatched.push_back(p.first);
      fmt::println("unmatched: {}", p.first);
    }
    total.Increment();
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;

  for (const auto &s : texts) {
    fmt::println("matched: {}", s);
  }

  fmt::println("Elapsed time for validation: {} sec", elapsed.count());
  fmt::println("Recognized {} out of {} ({}\%)", matched.Get(), total.Get(), (double)matched.Get() / total.Get() * 100);
}

auto BenchmarkRecognizeSpeed(const cv::Mat &image, std::map<std::string, int> text_count) noexcept -> void {}
