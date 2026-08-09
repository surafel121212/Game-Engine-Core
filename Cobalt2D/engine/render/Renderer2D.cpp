#include "engine/render/Renderer2D.hpp"

#include <algorithm>

#if defined(COBALT_PLATFORM_ANDROID)
#include <GLES3/gl3.h>
#endif

namespace cobalt {

Vec2 Camera2D::worldToScreen(Vec2 world, Vec2 viewport) const {
  const Vec2 relative = (world - position_) * zoom_;
  return {relative.x + viewport.x * 0.5f, viewport.y * 0.5f - relative.y};
}

Vec2 Camera2D::screenToWorld(Vec2 screen, Vec2 viewport) const {
  return {(screen.x - viewport.x * 0.5f) / zoom_ + position_.x,
          (viewport.y * 0.5f - screen.y) / zoom_ + position_.y};
}

bool Renderer2D::initialize(int width, int height) {
  width_ = width;
  height_ = height;
  return true;
}
void Renderer2D::resize(int width, int height) { width_ = width; height_ = height; }

void Renderer2D::beginFrame(Color clearColor) {
  commands_.clear();
  drawCallCount_ = 0;
#if defined(COBALT_PLATFORM_ANDROID)
  glViewport(0, 0, width_, height_);
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
#else
  (void)clearColor;
#endif
}

void Renderer2D::submit(SpriteDrawCommand command) {
  commands_.push_back(std::move(command));
}

void Renderer2D::endFrame() {
  std::stable_sort(commands_.begin(), commands_.end(),
    [](const auto& lhs, const auto& rhs) {
      return lhs.layer < rhs.layer || (lhs.layer == rhs.layer && lhs.order < rhs.order);
    });
  // A production backend groups adjacent commands by texture/material.
  // The command buffer remains backend-neutral for headless tests and Android GL.
  drawCallCount_ = commands_.empty() ? 0u : 1u;
}
void Renderer2D::shutdown() { commands_.clear(); }

}  // namespace cobalt