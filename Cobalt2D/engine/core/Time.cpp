#include "engine/core/Time.hpp"

#include <algorithm>

namespace cobalt {

void Time::beginFrame() {
  const auto now = Clock::now();
  const std::chrono::duration<float> elapsed = now - previous_;
  previous_ = now;
  unscaledDeltaSeconds_ = std::clamp(elapsed.count(), 0.0f, 0.25f);
  deltaSeconds_ = paused_ ? 0.0f : unscaledDeltaSeconds_;
  totalSeconds_ += deltaSeconds_;
  ++frameCount_;
  fpsAccumulator_ += unscaledDeltaSeconds_;
  ++fpsFrames_;
  if (fpsAccumulator_ >= 0.5f) {
    fps_ = static_cast<float>(fpsFrames_) / fpsAccumulator_;
    fpsAccumulator_ = 0.0f;
    fpsFrames_ = 0;
  }
}

void Time::endFrame() {}

}  // namespace cobalt