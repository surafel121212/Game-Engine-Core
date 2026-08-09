#include "engine/physics/PhysicsWorld.hpp"

#if defined(COBALT_HAS_BOX2D)
#include <box2d/box2d.h>
#endif

namespace cobalt {

PhysicsWorld::PhysicsWorld(Vec2 gravity) : gravity_(gravity) {}
PhysicsWorld::~PhysicsWorld() { shutdown(); }

bool PhysicsWorld::initialize() {
#if defined(COBALT_HAS_BOX2D)
  world_ = new b2World({gravity_.x, gravity_.y});
#endif
  return true;
}

void PhysicsWorld::step(float fixedDelta) {
#if defined(COBALT_HAS_BOX2D)
  if (world_) world_->Step(fixedDelta, 8, 3);
#else
  for (auto& [id, state] : bodies_) {
    state.velocity += gravity_ * fixedDelta;
    state.position += state.velocity * fixedDelta;
  }
#endif
}

void PhysicsWorld::setGravity(Vec2 gravity) {
  gravity_ = gravity;
#if defined(COBALT_HAS_BOX2D)
  if (world_) world_->SetGravity({gravity.x, gravity.y});
#endif
}

void PhysicsWorld::shutdown() {
#if defined(COBALT_HAS_BOX2D)
  delete world_;
  world_ = nullptr;
#endif
  bodies_.clear();
}

}  // namespace cobalt