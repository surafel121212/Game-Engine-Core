#pragma once

#include "engine/core/Types.hpp"
#include "engine/math/Math.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace cobalt {

class Entity {
public:
  Entity() = default;
  Entity(EntityId id, std::string name) : id_(id), name_(std::move(name)) {}

  EntityId id() const { return id_; }
  const std::string& name() const { return name_; }
  void setName(std::string name) { name_ = std::move(name); }
  bool enabled() const { return enabled_; }
  void setEnabled(bool enabled) { enabled_ = enabled; }
  EntityId parent() const { return parent_; }
  void setParent(EntityId parent) { parent_ = parent; }

  template <typename Component, typename... Args>
  Component& add(Args&&... args) {
    auto component = std::make_shared<Component>(std::forward<Args>(args)...);
    Component& reference = *component;
    components_[std::type_index(typeid(Component))] = std::move(component);
    return reference;
  }

  template <typename Component>
  Component* get() {
    const auto it = components_.find(std::type_index(typeid(Component)));
    return it == components_.end() ? nullptr : static_cast<Component*>(it->second.get());
  }

  template <typename Component>
  const Component* get() const {
    const auto it = components_.find(std::type_index(typeid(Component)));
    return it == components_.end() ? nullptr : static_cast<const Component*>(it->second.get());
  }

  template <typename Component>
  bool has() const { return get<Component>() != nullptr; }

  template <typename Component>
  void remove() { components_.erase(std::type_index(typeid(Component))); }

private:
  struct ComponentBase { virtual ~ComponentBase() = default; };
  template <typename Component>
  struct ComponentHolder : ComponentBase { Component value; };

  // The holder is intentionally not exposed; add/get are the public typed API.
  EntityId id_{0};
  std::string name_;
  EntityId parent_{0};
  bool enabled_{true};
  std::unordered_map<std::type_index, std::shared_ptr<void>> components_;
};

}  // namespace cobalt