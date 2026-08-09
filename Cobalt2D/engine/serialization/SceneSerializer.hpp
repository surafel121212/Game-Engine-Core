#pragma once

#include "engine/ecs/Scene.hpp"

#include <string>

namespace cobalt {

class SceneSerializer {
public:
  static bool save(const Scene& scene, const std::string& path, std::string* error = nullptr);
  static bool load(Scene& scene, const std::string& path, std::string* error = nullptr);
};

}  // namespace cobalt