#pragma once

#include "engine/core/Types.hpp"
#include "engine/math/Math.hpp"

#include <functional>
#include <unordered_map>
#include <vector>

#if defined(COBALT_HAS_BOX2D)
#include <box2d/box2d.h>
#endif

namespace cobalt {

class PhysicsWorld {
public:
  explicit PhysicsWorld(Vec2 gravity = {0.0f, -9.81f});
  ~PhysicsWorld();
  PhysicsWorld(const PhysicsWorld&) = delete;
  PhysicsWorld& operator=(const PhysicsWorld&) = delete;

  bool initialize();
  void step(float fixedDelta);
  void setGravity(Vec2 gravity);
  Vec2 gravity() const { return gravity_; }
  void shutdown();
  using CollisionCallback = std::function<void(EntityId, EntityId)>;
  void setCollisionCallback(CollisionCallback callback) { callback_ = std::move(callback); }

private:
  Vec2 gravity_;
  CollisionCallback callback_;
  struct BodyState { EntityId id; Vec2 position; Vec2 velocity; };
  std::unordered_map<EntityId, BodyState> bodies_;
#if defined(COBALT_HAS_BOX2D)
  b2World* world_{nullptr};
#endif
};

}  // namespace cobalt