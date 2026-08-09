#pragma once

#include <chrono>
#include <cstdint>

namespace cobalt {

class Time {
public:
  void beginFrame();
  void endFrame();
  void setPaused(bool paused) { paused_ = paused; }
  bool isPaused() const { return paused_; }
  float deltaSeconds() const { return deltaSeconds_; }
  float unscaledDeltaSeconds() const { return unscaledDeltaSeconds_; }
  float fixedStep() const { return fixedStep_; }
  double totalSeconds() const { return totalSeconds_; }
  std::uint64_t frameCount() const { return frameCount_; }
  float fps() const { return fps_; }

private:
  using Clock = std::chrono::steady_clock;
  Clock::time_point previous_{Clock::now()};
  float deltaSeconds_{0.0f};
  float unscaledDeltaSeconds_{0.0f};
  float fixedStep_{1.0f / 60.0f};
  double totalSeconds_{0.0};
  float fps_{0.0f};
  float fpsAccumulator_{0.0f};
  std::uint32_t fpsFrames_{0};
  std::uint64_t frameCount_{0};
  bool paused_{false};
};

}  // namespace cobalt