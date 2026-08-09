#pragma once

#include "engine/ecs/Components.hpp"
#include "engine/ecs/Entity.hpp"

#include <memory>
#include <string>
#include <vector>

namespace cobalt {

class Scene {
public:
  explicit Scene(std::string name = "Untitled");

  Entity& createObject(const std::string& name);
  bool destroyObject(EntityId id);
  Entity* find(EntityId id);
  const Entity* find(EntityId id) const;
  std::vector<Entity*> roots();
  std::vector<const Entity*> objects() const;
  void clear();

  const std::string& name() const { return name_; }
  void setName(std::string name) { name_ = std::move(name); }
  EntityId rootId() const { return rootId_; }

private:
  std::string name_;
  EntityId nextId_{1};
  EntityId rootId_{0};
  std::vector<std::unique_ptr<Entity>> objects_;
};

}  // namespace cobalt