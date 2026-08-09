#include "engine/particles/ParticleSystem.hpp"

#include <algorithm>

namespace cobalt {
namespace {
float randomUnit(std::uint32_t& seed) {
  seed = seed * 1664525u + 1013904223u;
  return static_cast<float>(seed & 0x00ffffffu) / static_cast<float>(0x01000000u);
}
float range(std::uint32_t& seed, float min, float max) {
  return min + (max - min) * randomUnit(seed);
}
}  // namespace

void ParticleSystem::emit(const ParticleEmitter& emitter, Vec2 origin) {
  if (particles_.size() >= emitter.maxParticles) return;
  particles_.push_back({
    origin,
    {range(seed_, emitter.velocityMin.x, emitter.velocityMax.x),
     range(seed_, emitter.velocityMin.y, emitter.velocityMax.y)},
    emitter.startColor,
    emitter.lifetime,
    0.0f,
    emitter.startSize,
    0.0f
  });
}

void ParticleSystem::burst(std::uint32_t count, const ParticleEmitter& emitter, Vec2 origin) {
  for (std::uint32_t index = 0; index < count; ++index) emit(emitter, origin);
}

void ParticleSystem::update(float deltaSeconds, const ParticleEmitter& emitter, Vec2 origin) {
  emissionRemainder_ += emitter.emissionRate * deltaSeconds;
  while (emissionRemainder_ >= 1.0f) {
    emit(emitter, origin);
    emissionRemainder_ -= 1.0f;
  }
  for (Particle& particle : particles_) {
    particle.age += deltaSeconds;
    particle.velocity += emitter.acceleration * deltaSeconds;
    particle.position += particle.velocity * deltaSeconds;
  }
  particles_.erase(
    std::remove_if(particles_.begin(), particles_.end(),
      [](const Particle& particle) { return particle.age >= particle.lifetime; }),
    particles_.end());
}

}  // namespace cobalt