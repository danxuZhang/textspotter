#include "benchmark.hpp"

auto BenchmarkRecognizeAccuracy(const cv::Mat &image, std::map<std::string, int> text_count) noexcept -> void {
  auto start = std::chrono::high_resolution_clock::now();

  auto &[texts, boxes, confs] = RecognizeText(image, 0);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  fmt::println("Elapsed time for Recognition: {} sec", elapsed.count());

  start = std::chrono::high_resolution_clock::now();
  SafeCounter total;
  SafeCounter matched;
  for (const auto &p : text_count) {
    for (const auto &text : texts) {
      // fmt::println("{} vs {}", p.first, text);
      if (IsMatch(text, p.first)) {
        matched.Increment();
      }
    }
    total.Increment();
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  fmt::println("Elapsed time for validation: {} sec", elapsed.count());
  fmt::println("Recognized {} out of {} ({}\%)", matched.Get(), total.Get(), (double)matched.Get() / total.Get());
}

auto BenchmarkRecognizeSpeed(const cv::Mat &image, std::map<std::string, int> text_count) noexcept -> void {}
