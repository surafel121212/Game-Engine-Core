#include "engine/math/Math.hpp"

#include <cassert>
#include <cmath>

int main() {
  using namespace cobalt;
  const Vec2 vector{3.0f, 4.0f};
  assert(std::abs(vector.length() - 5.0f) < 0.0001f);
  const Transform2D parent{{10.0f, 0.0f}, 0.0f, {2.0f, 2.0f}};
  const Transform2D child{{2.0f, 3.0f}, 0.0f, {1.0f, 1.0f}};
  const Transform2D world = child.combined(parent);
  assert(world.position.x == 14.0f);
  assert(world.position.y == 6.0f);
  return 0;
}