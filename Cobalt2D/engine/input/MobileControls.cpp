#include "engine/input/MobileControls.hpp"

#include <algorithm>

namespace cobalt {

bool TouchButton::update(const Input& input) {
  const bool nextDown = input.touchCount() > 0 && bounds_.contains(input.touchPosition());
  if (nextDown && !down_ && onPressed_) onPressed_();
  if (!nextDown && down_ && onReleased_) onReleased_();
  down_ = nextDown;
  return down_;
}

bool VirtualJoystick::update(const Input& input, int pointerId) {
  if (input.touchCount() == 0) {
    active_ = false;
    axis_ = {};
    return false;
  }
  const Vec2 delta = input.touchPosition(pointerId) - center_;
  const float distance = delta.length();
  active_ = distance > 0.0f;
  axis_ = distance > radius_ ? delta.normalized() : delta / std::max(radius_, 0.001f);
  return active_;
}

}  // namespace cobalt