#include "engine/core/Engine.hpp"

#include <cassert>

int main() {
  using namespace cobalt;
  Engine engine;
  assert(engine.initialize());
  engine.render();
  assert(!engine.renderer().commands().empty());
  return 0;
}
