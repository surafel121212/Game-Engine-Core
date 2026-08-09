#pragma once

#include "engine/input/Input.hpp"

#include <functional>

namespace cobalt {

class TouchButton {
public:
  explicit TouchButton(Rect bounds = {}) : bounds_(bounds) {}
  void setBounds(Rect bounds) { bounds_ = bounds; }
  void setAction(std::string action) { action_ = std::move(action); }
  void setOnPressed(std::function<void()> callback) { onPressed_ = std::move(callback); }
  void setOnReleased(std::function<void()> callback) { onReleased_ = std::move(callback); }
  bool update(const Input& input);
  bool isDown() const { return down_; }

private:
  Rect bounds_;
  std::string action_;
  std::function<void()> onPressed_;
  std::function<void()> onReleased_;
  bool down_{false};
};

class VirtualJoystick {
public:
  VirtualJoystick(Vec2 center = {}, float radius = 80.0f) : center_(center), radius_(radius) {}
  void setCenter(Vec2 center) { center_ = center; }
  void setRadius(float radius) { radius_ = radius; }
  bool update(const Input& input, int pointerId = 0);
  Vec2 axis() const { return axis_; }
  bool active() const { return active_; }

private:
  Vec2 center_;
  float radius_;
  Vec2 axis_;
  bool active_{false};
};

}  // namespace cobalt