#include "engine/animation/Animator.hpp"
#include "engine/input/Input.hpp"
#include "engine/particles/ParticleSystem.hpp"
#include "engine/tilemap/Tilemap.hpp"

#include <cassert>

int main() {
  using namespace cobalt;
  Input input;
  input.bind("Jump", Key::Space);
  input.setKey(Key::Space, true);
  assert(input.isActionPressed("Jump"));
  input.beginFrame();
  assert(input.isActionDown("Jump"));

  Animator animator;
  animator.addClip({"blink", {{"a", 0.1f}, {"b", 0.1f}}, true, 1.0f});
  assert(animator.play("blink"));
  animator.update(0.11f);
  assert(animator.currentFrame()->texture == "b");

  Tilemap tilemap{4, 4, 16, 16};
  tilemap.set(1, 1, {1});
  assert(tilemap.get(1, 1).id == 1);
  assert(tilemap.worldToCell({17.0f, 1.0f}).x == 1.0f);

  ParticleSystem particles;
  ParticleEmitter emitter;
  emitter.emissionRate = 100.0f;
  particles.update(0.1f, emitter, {});
  assert(!particles.particles().empty());
  return 0;
}