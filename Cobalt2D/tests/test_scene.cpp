#include "engine/ecs/Components.hpp"
#include "engine/ecs/Scene.hpp"
#include "engine/serialization/SceneSerializer.hpp"

#include <cassert>
#include <filesystem>

int main() {
  using namespace cobalt;
  Scene scene{"TestScene"};
  Entity& player = scene.createObject("Player");
  player.add<TransformComponent>().local.position = {4.0f, 2.0f};
  Entity& camera = scene.createObject("Camera");
  camera.setParent(player.id());
  assert(scene.find(player.id()) == &player);
  assert(camera.parent() == player.id());
  const auto path = std::filesystem::temp_directory_path() / "cobalt-scene.scene";
  std::string error;
  assert(SceneSerializer::save(scene, path.string(), &error));
  Scene loaded;
  assert(SceneSerializer::load(loaded, path.string(), &error));
  assert(loaded.name() == "TestScene");
  assert(loaded.objects().size() == 2);
  std::filesystem::remove(path);
  return 0;
}