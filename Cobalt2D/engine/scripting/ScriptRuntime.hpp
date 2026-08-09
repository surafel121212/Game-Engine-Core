#pragma once

#include "engine/core/Types.hpp"
#include "engine/scripting/LuaScript.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace cobalt {

class ScriptRuntime {
public:
  bool attach(EntityId entity, const std::string& path);
  bool detach(EntityId entity);
  LuaScript* get(EntityId entity);
  const LuaScript* get(EntityId entity) const;
  bool reload(EntityId entity);
  void onCreate();
  void onAwake();
  void onStart();
  void onEnable(EntityId entity);
  void onDisable(EntityId entity);
  void update(float deltaSeconds);
  void fixedUpdate(float deltaSeconds);
  void lateUpdate(float deltaSeconds);
  void onDestroy(EntityId entity);
  void onCollision(EntityId entity, ScriptLifecycle lifecycle, const std::string& other);
  void clear();
  std::size_t size() const { return scripts_.size(); }

private:
  std::unordered_map<EntityId, std::unique_ptr<LuaScript>> scripts_;
};

}  // namespace cobalt