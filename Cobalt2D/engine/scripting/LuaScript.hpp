#pragma once

#include "engine/math/Math.hpp"

#include <cstdint>
#include <set>
#include <string>
#include <variant>
#include <vector>

struct lua_State;

namespace cobalt {

enum class ScriptLifecycle {
  Create,
  Awake,
  Start,
  Enable,
  Disable,
  Update,
  FixedUpdate,
  LateUpdate,
  Destroy,
  CollisionEnter,
  CollisionStay,
  CollisionExit,
  TriggerEnter,
  TriggerStay,
  TriggerExit,
};

enum class ScriptPropertyType {
  Integer,
  Float,
  Boolean,
  String,
  Vector2,
  Vector3,
  Color,
  Enum,
  GameObjectReference,
  ComponentReference,
  AssetReference,
};

using ScriptPropertyValue = std::variant<std::int64_t, float, bool, std::string, Vec2, Vec3, Color>;

struct ScriptProperty {
  std::string name;
  ScriptPropertyType type{ScriptPropertyType::String};
  ScriptPropertyValue value{std::string{}};
  std::vector<std::string> enumOptions;
  bool exported{false};
};

struct ScriptDiagnostic {
  enum class Severity { Info, Warning, Error };
  Severity severity{Severity::Error};
  std::string file;
  int line{0};
  std::string message;
};

class LuaScript {
public:
  LuaScript() = default;
  ~LuaScript();
  LuaScript(const LuaScript&) = delete;
  LuaScript& operator=(const LuaScript&) = delete;

  bool initialize();
  bool loadFile(const std::string& path);
  bool reload();
  bool call(const char* functionName);
  bool dispatch(ScriptLifecycle lifecycle, float deltaSeconds = 0.0f,
                const std::string& other = {});
  void update(float deltaSeconds) { dispatch(ScriptLifecycle::Update, deltaSeconds); }
  void fixedUpdate(float deltaSeconds) { dispatch(ScriptLifecycle::FixedUpdate, deltaSeconds); }
  void lateUpdate(float deltaSeconds) { dispatch(ScriptLifecycle::LateUpdate, deltaSeconds); }
  bool setProperty(const std::string& name, const ScriptPropertyValue& value);
  const std::vector<ScriptProperty>& properties() const { return properties_; }
  const std::vector<ScriptDiagnostic>& diagnostics() const { return diagnostics_; }
  static std::vector<ScriptProperty> inspectSource(const std::string& source);
  void setPaused(bool paused) { paused_ = paused; }
  bool isPaused() const { return paused_; }
  void setBreakpoint(int line, bool enabled);
  bool hasBreakpoint(int line) const { return breakpoints_.find(line) != breakpoints_.end(); }
  const std::string& sourcePath() const { return sourcePath_; }
  void shutdown();
  const std::string& lastError() const { return lastError_; }

private:
  bool callWithNumber(const char* functionName, float value);
  bool callWithString(const char* functionName, const std::string& value);
  void reportError(const std::string& message);

  lua_State* state_{nullptr};
  std::string sourcePath_;
  std::string source_;
  std::string lastError_;
  std::vector<ScriptProperty> properties_;
  std::vector<ScriptDiagnostic> diagnostics_;
  std::set<int> breakpoints_;
  bool paused_{false};
};

}  // namespace cobalt