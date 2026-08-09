#include "engine/ecs/Scene.hpp"

#include <algorithm>

namespace cobalt {

Scene::Scene(std::string name) : name_(std::move(name)) {}

Entity& Scene::createObject(const std::string& name) {
  auto entity = std::make_unique<Entity>(nextId_++, name);
  Entity& reference = *entity;
  objects_.push_back(std::move(entity));
  return reference;
}

bool Scene::destroyObject(EntityId id) {
  const auto it = std::find_if(objects_.begin(), objects_.end(),
    [id](const auto& entity) { return entity->id() == id; });
  if (it == objects_.end()) return false;
  objects_.erase(it);
  return true;
}

Entity* Scene::find(EntityId id) {
  const auto it = std::find_if(objects_.begin(), objects_.end(),
    [id](const auto& entity) { return entity->id() == id; });
  return it == objects_.end() ? nullptr : it->get();
}

const Entity* Scene::find(EntityId id) const {
  const auto it = std::find_if(objects_.begin(), objects_.end(),
    [id](const auto& entity) { return entity->id() == id; });
  return it == objects_.end() ? nullptr : it->get();
}

std::vector<Entity*> Scene::roots() {
  std::vector<Entity*> result;
  for (auto& object : objects_) {
    if (object->parent() == 0) result.push_back(object.get());
  }
  return result;
}

std::vector<const Entity*> Scene::objects() const {
  std::vector<const Entity*> result;
  result.reserve(objects_.size());
  for (const auto& object : objects_) result.push_back(object.get());
  return result;
}

void Scene::clear() {
  objects_.clear();
  nextId_ = 1;
}

}  // namespace cobalt