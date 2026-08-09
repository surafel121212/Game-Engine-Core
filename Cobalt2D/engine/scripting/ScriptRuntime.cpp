#include "engine/scripting/ScriptRuntime.hpp"

namespace cobalt {

bool ScriptRuntime::attach(EntityId entity, const std::string& path) {
  auto script = std::make_unique<LuaScript>();
  if (!script->loadFile(path)) return false;
  script->dispatch(ScriptLifecycle::Create);
  script->dispatch(ScriptLifecycle::Awake);
  scripts_[entity] = std::move(script);
  return true;
}

bool ScriptRuntime::detach(EntityId entity) {
  const auto it = scripts_.find(entity);
  if (it == scripts_.end()) return false;
  it->second->dispatch(ScriptLifecycle::Destroy);
  scripts_.erase(it);
  return true;
}

LuaScript* ScriptRuntime::get(EntityId entity) {
  const auto it = scripts_.find(entity);
  return it == scripts_.end() ? nullptr : it->second.get();
}

const LuaScript* ScriptRuntime::get(EntityId entity) const {
  const auto it = scripts_.find(entity);
  return it == scripts_.end() ? nullptr : it->second.get();
}

bool ScriptRuntime::reload(EntityId entity) {
  auto* script = get(entity);
  return script && script->reload();
}

void ScriptRuntime::onCreate() { for (auto& [id, script] : scripts_) script->dispatch(ScriptLifecycle::Create); }
void ScriptRuntime::onAwake() { for (auto& [id, script] : scripts_) script->dispatch(ScriptLifecycle::Awake); }
void ScriptRuntime::onStart() { for (auto& [id, script] : scripts_) script->dispatch(ScriptLifecycle::Start); }
void ScriptRuntime::onEnable(EntityId entity) { if (auto* script = get(entity)) script->dispatch(ScriptLifecycle::Enable); }
void ScriptRuntime::onDisable(EntityId entity) { if (auto* script = get(entity)) script->dispatch(ScriptLifecycle::Disable); }
void ScriptRuntime::update(float deltaSeconds) { for (auto& [id, script] : scripts_) script->update(deltaSeconds); }
void ScriptRuntime::fixedUpdate(float deltaSeconds) { for (auto& [id, script] : scripts_) script->fixedUpdate(deltaSeconds); }
void ScriptRuntime::lateUpdate(float deltaSeconds) { for (auto& [id, script] : scripts_) script->lateUpdate(deltaSeconds); }
void ScriptRuntime::onDestroy(EntityId entity) { if (auto* script = get(entity)) script->dispatch(ScriptLifecycle::Destroy); }
void ScriptRuntime::onCollision(EntityId entity, ScriptLifecycle lifecycle, const std::string& other) {
  if (auto* script = get(entity)) script->dispatch(lifecycle, 0.0f, other);
}
void ScriptRuntime::clear() { scripts_.clear(); }

}  // namespace cobalt