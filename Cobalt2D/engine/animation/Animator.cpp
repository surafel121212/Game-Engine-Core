#include "engine/animation/Animator.hpp"

namespace cobalt {

void Animator::addClip(AnimationClip clip) { clips_[clip.name] = std::move(clip); }

bool Animator::play(const std::string& name, bool restart) {
  if (clips_.find(name) == clips_.end()) return false;
  if (restart || current_ != name) {
    current_ = name;
    frameIndex_ = 0;
    frameTime_ = 0.0f;
    finished_ = false;
  }
  paused_ = false;
  return true;
}

void Animator::stop() {
  paused_ = true;
  frameIndex_ = 0;
  frameTime_ = 0.0f;
}

void Animator::update(float deltaSeconds) {
  if (paused_) return;
  const AnimationClip* clip = currentClip();
  if (!clip || clip->frames.empty()) return;
  frameTime_ += deltaSeconds * clip->speed;
  while (frameTime_ >= clip->frames[frameIndex_].duration) {
    frameTime_ -= clip->frames[frameIndex_].duration;
    ++frameIndex_;
    if (frameIndex_ < clip->frames.size()) continue;
    if (!clip->loop) {
      frameIndex_ = clip->frames.size() - 1;
      finished_ = true;
      paused_ = true;
      break;
    }
    frameIndex_ = 0;
  }
}

const AnimationClip* Animator::currentClip() const {
  const auto it = clips_.find(current_);
  return it == clips_.end() ? nullptr : &it->second;
}

const AnimationFrame* Animator::currentFrame() const {
  const AnimationClip* clip = currentClip();
  return clip && !clip->frames.empty() ? &clip->frames[frameIndex_] : nullptr;
}

}  // namespace cobalt