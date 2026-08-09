#pragma once

#include "engine/math/Math.hpp"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace cobalt {

struct SpriteDrawCommand {
  std::string texture;
  Transform2D transform{};
  Vec2 size{1.0f, 1.0f};
  Color tint{};
  int layer{0};
  int order{0};
};

class Camera2D {
public:
  void setPosition(Vec2 position) { position_ = position; }
  void setZoom(float zoom) { zoom_ = std::max(0.01f, zoom); }
  Vec2 position() const { return position_; }
  float zoom() const { return zoom_; }
  Vec2 worldToScreen(Vec2 world, Vec2 viewport) const;
  Vec2 screenToWorld(Vec2 screen, Vec2 viewport) const;

private:
  Vec2 position_{};
  float zoom_{1.0f};
};

class Renderer2D {
public:
  bool initialize(int width, int height);
  void resize(int width, int height);
  void beginFrame(Color clearColor = {0.04f, 0.05f, 0.08f, 1.0f});
  void submit(SpriteDrawCommand command);
  void endFrame();
  void shutdown();
  const std::vector<SpriteDrawCommand>& commands() const { return commands_; }
  Camera2D& camera() { return camera_; }
  int width() const { return width_; }
  int height() const { return height_; }
  std::uint32_t drawCallCount() const { return drawCallCount_; }

private:
  int width_{0};
  int height_{0};
  std::uint32_t drawCallCount_{0};
  Camera2D camera_;
  std::vector<SpriteDrawCommand> commands_;
};

}  // namespace cobalt