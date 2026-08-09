#include "engine/core/Profiler.hpp"

namespace cobalt {

Profiler::Scope::Scope(Profiler& profiler, std::string name)
    : profiler_(profiler), name_(std::move(name)), start_(std::chrono::steady_clock::now()) {}

Profiler::Scope::~Scope() {
  const auto elapsed = std::chrono::steady_clock::now() - start_;
  profiler_.addSample(name_, std::chrono::duration<float, std::milli>(elapsed).count());
}

void Profiler::beginFrame() { frameStart_ = std::chrono::steady_clock::now(); }

void Profiler::endFrame(float fps) {
  const auto elapsed = std::chrono::steady_clock::now() - frameStart_;
  snapshot_.fps = fps;
  snapshot_.frameTimeMs = std::chrono::duration<float, std::milli>(elapsed).count();
  snapshot_.cpuTimeMs = snapshot_.frameTimeMs;
}

void Profiler::addSample(const std::string& name, float milliseconds) {
  samples_[name] = milliseconds;
}

float Profiler::sample(const std::string& name) const {
  const auto it = samples_.find(name);
  return it == samples_.end() ? 0.0f : it->second;
}

}  // namespace cobalt