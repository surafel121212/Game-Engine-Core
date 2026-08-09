#pragma once

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

class Animator {
public:
  void addClip(AnimationClip clip);
  bool play(const std::string& name, bool restart = false);
  void pause() { paused_ = true; }
  void resume() { paused_ = false; }
  void stop();
  void update(float deltaSeconds);
  const AnimationClip* currentClip() const;
  const AnimationFrame* currentFrame() const;
  bool finished() const { return finished_; }

private:
  std::unordered_map<std::string, AnimationClip> clips_;
  std::string current_;
  std::size_t frameIndex_{0};
  float frameTime_{0.0f};
  bool paused_{false};
  bool finished_{false};
};

}  // namespace cobalt