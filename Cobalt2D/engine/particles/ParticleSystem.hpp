#pragma once

#include "engine/math/Math.hpp"

#include <cstdint>
#include <vector>

namespace cobalt {

struct Particle {
  Vec2 position{};
  Vec2 velocity{};
  Color color{};
  float lifetime{1.0f};
  float age{0.0f};
  float size{1.0f};
  float rotation{0.0f};
};

struct ParticleEmitter {
  float emissionRate{20.0f};
  float lifetime{1.0f};
  Vec2 velocityMin{-1.0f, 1.0f};
  Vec2 velocityMax{1.0f, 3.0f};
  Vec2 acceleration{0.0f, -9.8f};
  Color startColor{1.0f, 1.0f, 1.0f, 1.0f};
  float startSize{1.0f};
  std::uint32_t maxParticles{512};
};

class ParticleSystem {
public:
  void update(float deltaSeconds, const ParticleEmitter& emitter, Vec2 origin);
  void burst(std::uint32_t count, const ParticleEmitter& emitter, Vec2 origin);
  const std::vector<Particle>& particles() const { return particles_; }
  void clear() { particles_.clear(); }

private:
  void emit(const ParticleEmitter& emitter, Vec2 origin);
  std::vector<Particle> particles_;
  float emissionRemainder_{0.0f};
  std::uint32_t seed_{0xC0BA17u};
};

}  // namespace cobalt