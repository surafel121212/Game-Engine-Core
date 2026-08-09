#pragma once

#include "engine/math/Math.hpp"

#include <array>
#include <string>
#include <unordered_map>

namespace cobalt {

enum class Key {
  Left, Right, Up, Down, Space, Escape, Count
};

class Input {
public:
  void beginFrame();
  void setKey(Key key, bool down);
  void setTouch(int pointerId, Vec2 position, bool down);
  void releaseTouch(int pointerId);
  bool isDown(Key key) const;
  bool isPressed(Key key) const;
  bool isReleased(Key key) const;
  bool isActionDown(const std::string& action) const;
  bool isActionPressed(const std::string& action) const;
  void bind(const std::string& action, Key key);
  Vec2 touchPosition(int pointerId = 0) const;
  std::size_t touchCount() const { return touches_.size(); }

private:
  std::array<bool, static_cast<std::size_t>(Key::Count)> down_{};
  std::array<bool, static_cast<std::size_t>(Key::Count)> pressed_{};
  std::array<bool, static_cast<std::size_t>(Key::Count)> released_{};
  struct Touch { Vec2 position; bool down; };
  std::unordered_map<int, Touch> touches_;
  std::unordered_map<std::string, Key> bindings_;
};

}  // namespace cobalt