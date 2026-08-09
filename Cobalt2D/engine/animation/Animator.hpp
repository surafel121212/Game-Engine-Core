#pragma once

#include "engine/math/Math.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cobalt {

struct AnimationFrame {
  std::string texture;
  float duration{0.1f};
};

struct AnimationClip {
  std::string name;
  std::vector<AnimationFrame> frames;
  bool loop{true};
  float speed{1.0f};
};

enum class Interpolation { Constant, Linear, Smooth, EaseIn, EaseOut, EaseInOut };

struct Keyframe {
  float time{0.0f};
  float value{0.0f};
  Interpolation interpolation{Interpolation::Linear};
};

struct AnimationTrack {
  std::string name;
  std::vector<Keyframe> keys;
};

struct AnimationEvent {
  float time{0.0f};
  std::string name;
};

struct TimelineClip {
  std::string name;
  float duration{1.0f};
  bool loop{true};
  float speed{1.0f};
  std::vector<AnimationTrack> tracks;
  std::vector<AnimationEvent> events;
};

struct StateTransition {
  std::string from;
  std::string to;
  std::function<bool()> condition;
  float blendDuration{0.0f};
};

class Animator;

class AnimationStateMachine {
public:
  void addState(std::string state, std::string clip);
  void addTransition(StateTransition transition);
  bool setState(const std::string& state);
  void update(float deltaSeconds, Animator& animator);
  const std::string& state() const { return currentState_; }

private:
  std::unordered_map<std::string, std::string> states_;
  std::vector<StateTransition> transitions_;
  std::string currentState_;
};

class Animator {
public:
  void addClip(AnimationClip clip);
  void addTimeline(TimelineClip timeline);
  bool play(const std::string& name, bool restart = false);
  void pause() { paused_ = true; }
  void resume() { paused_ = false; }
  void stop();
  void update(float deltaSeconds);
  const AnimationClip* currentClip() const;
  const AnimationFrame* currentFrame() const;
  float evaluate(const std::string& timeline, const std::string& track, float time) const;
  void setEventCallback(std::function<void(const std::string&)> callback) {
    eventCallback_ = std::move(callback);
  }
  void setTimeline(const std::string& name);
  const TimelineClip* currentTimeline() const;
  const std::string& currentName() const { return current_; }
  bool finished() const { return finished_; }

private:
  static float applyInterpolation(float amount, Interpolation interpolation);
  std::unordered_map<std::string, AnimationClip> clips_;
  std::unordered_map<std::string, TimelineClip> timelines_;
  std::string current_;
  std::string currentTimeline_;
  std::size_t frameIndex_{0};
  float frameTime_{0.0f};
  float timelineTime_{0.0f};
  float previousTimelineTime_{0.0f};
  bool paused_{false};
  bool finished_{false};
  std::function<void(const std::string&)> eventCallback_;
};

}  // namespace cobalt