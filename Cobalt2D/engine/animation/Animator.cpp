#include "engine/animation/Animator.hpp"

#include <algorithm>
#include <cmath>

namespace cobalt {

void Animator::addClip(AnimationClip clip) { clips_[clip.name] = std::move(clip); }
void Animator::addTimeline(TimelineClip timeline) { timelines_[timeline.name] = std::move(timeline); }

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
  const TimelineClip* timeline = currentTimeline();
  if (!timeline || paused_) return;
  previousTimelineTime_ = timelineTime_;
  timelineTime_ += deltaSeconds * timeline->speed;
  for (const AnimationEvent& event : timeline->events) {
    const bool crossed = event.time > previousTimelineTime_ && event.time <= timelineTime_;
    if (crossed && eventCallback_) eventCallback_(event.name);
  }
  if (timelineTime_ >= timeline->duration) {
    if (timeline->loop) {
      timelineTime_ = std::fmod(timelineTime_, timeline->duration);
      previousTimelineTime_ = 0.0f;
    } else {
      timelineTime_ = timeline->duration;
    }
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

float Animator::applyInterpolation(float amount, Interpolation interpolation) {
  amount = std::clamp(amount, 0.0f, 1.0f);
  switch (interpolation) {
    case Interpolation::Constant: return 0.0f;
    case Interpolation::Smooth: return amount * amount * (3.0f - 2.0f * amount);
    case Interpolation::EaseIn: return amount * amount;
    case Interpolation::EaseOut: return 1.0f - (1.0f - amount) * (1.0f - amount);
    case Interpolation::EaseInOut:
      return amount < 0.5f ? 2.0f * amount * amount
                           : 1.0f - std::pow(-2.0f * amount + 2.0f, 2.0f) / 2.0f;
    case Interpolation::Linear: return amount;
  }
  return amount;
}

float Animator::evaluate(const std::string& timeline, const std::string& track, float time) const {
  const auto timelineIt = timelines_.find(timeline);
  if (timelineIt == timelines_.end()) return 0.0f;
  const auto trackIt = std::find_if(timelineIt->second.tracks.begin(), timelineIt->second.tracks.end(),
    [&track](const AnimationTrack& item) { return item.name == track; });
  if (trackIt == timelineIt->second.tracks.end() || trackIt->keys.empty()) return 0.0f;
  const auto& keys = trackIt->keys;
  if (time <= keys.front().time) return keys.front().value;
  if (time >= keys.back().time) return keys.back().value;
  for (std::size_t index = 1; index < keys.size(); ++index) {
    if (time > keys[index].time) continue;
    const Keyframe& before = keys[index - 1];
    const Keyframe& after = keys[index];
    const float amount = applyInterpolation((time - before.time) / (after.time - before.time),
                                             before.interpolation);
    return before.value + (after.value - before.value) * amount;
  }
  return keys.back().value;
}

void Animator::setTimeline(const std::string& name) {
  if (timelines_.find(name) == timelines_.end()) return;
  currentTimeline_ = name;
  timelineTime_ = 0.0f;
  previousTimelineTime_ = 0.0f;
}

const TimelineClip* Animator::currentTimeline() const {
  const auto it = timelines_.find(currentTimeline_);
  return it == timelines_.end() ? nullptr : &it->second;
}

void AnimationStateMachine::addState(std::string state, std::string clip) {
  states_[std::move(state)] = std::move(clip);
}

void AnimationStateMachine::addTransition(StateTransition transition) {
  transitions_.push_back(std::move(transition));
}

bool AnimationStateMachine::setState(const std::string& state) {
  if (states_.find(state) == states_.end()) return false;
  currentState_ = state;
  return true;
}

void AnimationStateMachine::update(float deltaSeconds, Animator& animator) {
  if (currentState_.empty()) return;
  const auto stateIt = states_.find(currentState_);
  if (stateIt != states_.end() && animator.currentName() != stateIt->second) {
    animator.play(stateIt->second);
  }
  animator.update(deltaSeconds);
  for (const StateTransition& transition : transitions_) {
    if (transition.from == currentState_ && transition.condition && transition.condition()) {
      setState(transition.to);
      return;
    }
  }
}

}  // namespace cobalt