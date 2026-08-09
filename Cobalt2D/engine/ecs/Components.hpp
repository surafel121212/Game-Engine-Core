#pragma once

#include "engine/core/Types.hpp"
#include "engine/math/Math.hpp"

#include <string>

namespace cobalt {

struct TransformComponent {
  Transform2D local{};
  EntityId parent{0};
};

struct SpriteRendererComponent {
  std::string texture;
  Color tint{};
  Vec2 size{1.0f, 1.0f};
  int sortingLayer{0};
  int order{0};
  bool flipX{false};
  bool flipY{false};
};

enum class BodyType { Static, Kinematic, Dynamic };

struct RigidBody2DComponent {
  BodyType bodyType{BodyType::Dynamic};
  float mass{1.0f};
  float gravityScale{1.0f};
  float linearDamping{0.0f};
  bool fixedRotation{false};
};

struct BoxCollider2DComponent {
  Vec2 size{1.0f, 1.0f};
  Vec2 offset{};
  float friction{0.3f};
  float restitution{0.0f};
  bool isTrigger{false};
};

struct Camera2DComponent {
  float zoom{1.0f};
  float smoothing{0.0f};
  Rect bounds{{-10000.0f, -10000.0f}, {10000.0f, 10000.0f}};
};

struct ScriptComponent {
  std::string sourceFile;
  bool reloadOnChange{true};
};

}  // namespace cobalt