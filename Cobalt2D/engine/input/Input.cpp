#include "engine/input/Input.hpp"

namespace cobalt {

void Input::beginFrame() {
  pressed_.fill(false);
  released_.fill(false);
}

void Input::setKey(Key key, bool down) {
  const auto index = static_cast<std::size_t>(key);
  if (down && !down_[index]) pressed_[index] = true;
  if (!down && down_[index]) released_[index] = true;
  down_[index] = down;
}

void Input::setTouch(int pointerId, Vec2 position, bool down) {
  auto it = touches_.find(pointerId);
  if (it == touches_.end()) {
    if (down) touches_.emplace(pointerId, Touch{position, true});
    return;
  }
  it->second.position = position;
  it->second.down = down;
  if (!down) touches_.erase(it);
}

void Input::releaseTouch(int pointerId) { touches_.erase(pointerId); }
bool Input::isDown(Key key) const { return down_[static_cast<std::size_t>(key)]; }
bool Input::isPressed(Key key) const { return pressed_[static_cast<std::size_t>(key)]; }
bool Input::isReleased(Key key) const { return released_[static_cast<std::size_t>(key)]; }

bool Input::isActionDown(const std::string& action) const {
  const auto it = bindings_.find(action);
  return it != bindings_.end() && isDown(it->second);
}
bool Input::isActionPressed(const std::string& action) const {
  const auto it = bindings_.find(action);
  return it != bindings_.end() && isPressed(it->second);
}
void Input::bind(const std::string& action, Key key) { bindings_[action] = key; }

Vec2 Input::touchPosition(int pointerId) const {
  const auto it = touches_.find(pointerId);
  return it == touches_.end() ? Vec2{} : it->second.position;
}

}  // namespace cobalt