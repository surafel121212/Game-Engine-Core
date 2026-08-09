#include "engine/scripting/LuaScript.hpp"

#include "engine/core/Log.hpp"

#include <fstream>
#include <regex>
#include <sstream>

#if defined(COBALT_HAS_LUA)
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#endif

namespace cobalt {

LuaScript::~LuaScript() { shutdown(); }

namespace {
ScriptPropertyValue parseValue(const std::string& raw) {
  const std::string value = std::regex_replace(raw, std::regex("^\\s+|\\s+$"), "");
  if (value == "true") return true;
  if (value == "false") return false;
  if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
    return value.substr(1, value.size() - 2);
  }
  try {
    if (value.find('.') != std::string::npos) return std::stof(value);
    return static_cast<std::int64_t>(std::stoll(value));
  } catch (...) {
    return value;
  }
}

const char* callbackName(ScriptLifecycle lifecycle) {
  switch (lifecycle) {
    case ScriptLifecycle::Create: return "onCreate";
    case ScriptLifecycle::Awake: return "onAwake";
    case ScriptLifecycle::Start: return "onStart";
    case ScriptLifecycle::Enable: return "onEnable";
    case ScriptLifecycle::Disable: return "onDisable";
    case ScriptLifecycle::Update: return "onUpdate";
    case ScriptLifecycle::FixedUpdate: return "onFixedUpdate";
    case ScriptLifecycle::LateUpdate: return "onLateUpdate";
    case ScriptLifecycle::Destroy: return "onDestroy";
    case ScriptLifecycle::CollisionEnter: return "onCollisionEnter";
    case ScriptLifecycle::CollisionStay: return "onCollisionStay";
    case ScriptLifecycle::CollisionExit: return "onCollisionExit";
    case ScriptLifecycle::TriggerEnter: return "onTriggerEnter";
    case ScriptLifecycle::TriggerStay: return "onTriggerStay";
    case ScriptLifecycle::TriggerExit: return "onTriggerExit";
  }
  return "";
}
}  // namespace

bool LuaScript::initialize() {
#if defined(COBALT_HAS_LUA)
  if (state_) return true;
  state_ = luaL_newstate();
  if (!state_) {
    lastError_ = "Lua state allocation failed";
    return false;
  }
  luaL_openlibs(state_);
  return true;
#else
  lastError_ = "Lua support was disabled at configure time";
  return false;
#endif
}

bool LuaScript::loadFile(const std::string& path) {
  std::ifstream input(path);
  if (!input) {
    reportError("Unable to open script: " + path);
    return false;
  }
  std::stringstream buffer;
  buffer << input.rdbuf();
  sourcePath_ = path;
  source_ = buffer.str();
  properties_ = inspectSource(source_);
#if defined(COBALT_HAS_LUA)
  if (!state_ && !initialize()) return false;
  if (luaL_dofile(state_, path.c_str()) != LUA_OK) {
    const char* error = lua_tostring(state_, -1);
    reportError(error ? error : "Unknown Lua error");
    lua_pop(state_, 1);
    return false;
  }
  return true;
#else
  (void)path;
  reportError("Lua support was disabled at configure time");
  return false;
#endif
}

bool LuaScript::reload() {
  if (sourcePath_.empty()) {
    reportError("Cannot reload a script that has not been loaded");
    return false;
  }
  shutdown();
  return loadFile(sourcePath_);
}

bool LuaScript::call(const char* functionName) {
#if defined(COBALT_HAS_LUA)
  if (!state_) return false;
  lua_getglobal(state_, functionName);
  if (!lua_isfunction(state_, -1)) {
    lua_pop(state_, 1);
    return true;
  }
  if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
    const char* error = lua_tostring(state_, -1);
    reportError(error ? error : "Unknown Lua error");
    lua_pop(state_, 1);
    return false;
  }
  return true;
#else
  (void)functionName;
  return false;
#endif
}

bool LuaScript::callWithNumber(const char* functionName, float value) {
#if defined(COBALT_HAS_LUA)
  if (!state_) return false;
  lua_getglobal(state_, functionName);
  if (!lua_isfunction(state_, -1)) { lua_pop(state_, 1); return true; }
  lua_pushnumber(state_, value);
  if (lua_pcall(state_, 1, 0, 0) != LUA_OK) {
    const char* error = lua_tostring(state_, -1);
    reportError(error ? error : "Unknown Lua error");
    lua_pop(state_, 1);
    return false;
  }
  return true;
#else
  (void)functionName;
  (void)value;
  return false;
#endif
}

bool LuaScript::callWithString(const char* functionName, const std::string& value) {
#if defined(COBALT_HAS_LUA)
  if (!state_) return false;
  lua_getglobal(state_, functionName);
  if (!lua_isfunction(state_, -1)) { lua_pop(state_, 1); return true; }
  lua_pushlstring(state_, value.data(), value.size());
  if (lua_pcall(state_, 1, 0, 0) != LUA_OK) {
    const char* error = lua_tostring(state_, -1);
    reportError(error ? error : "Unknown Lua error");
    lua_pop(state_, 1);
    return false;
  }
  return true;
#else
  (void)functionName;
  (void)value;
  return false;
#endif
}

bool LuaScript::dispatch(ScriptLifecycle lifecycle, float deltaSeconds, const std::string& other) {
  if (paused_) return true;
  const char* callback = callbackName(lifecycle);
  if (lifecycle == ScriptLifecycle::Update || lifecycle == ScriptLifecycle::FixedUpdate ||
      lifecycle == ScriptLifecycle::LateUpdate) {
    return callWithNumber(callback, deltaSeconds);
  }
  if (lifecycle == ScriptLifecycle::CollisionEnter || lifecycle == ScriptLifecycle::CollisionStay ||
      lifecycle == ScriptLifecycle::CollisionExit || lifecycle == ScriptLifecycle::TriggerEnter ||
      lifecycle == ScriptLifecycle::TriggerStay || lifecycle == ScriptLifecycle::TriggerExit) {
    return callWithString(callback, other);
  }
  return call(callback);
}

bool LuaScript::setProperty(const std::string& name, const ScriptPropertyValue& value) {
  for (ScriptProperty& property : properties_) {
    if (property.name != name) continue;
    property.value = value;
#if defined(COBALT_HAS_LUA)
    if (!state_) return true;
    if (const auto* integer = std::get_if<std::int64_t>(&value)) lua_pushinteger(state_, *integer);
    else if (const auto* number = std::get_if<float>(&value)) lua_pushnumber(state_, *number);
    else if (const auto* boolean = std::get_if<bool>(&value)) lua_pushboolean(state_, *boolean);
    else if (const auto* string = std::get_if<std::string>(&value)) lua_pushlstring(state_, string->data(), string->size());
    else return true;
    lua_setglobal(state_, name.c_str());
#endif
    return true;
  }
  return false;
}

std::vector<ScriptProperty> LuaScript::inspectSource(const std::string& source) {
  std::vector<ScriptProperty> result;
  bool exportNext = false;
  const std::regex exportMarker(R"(^\s*---@export(?:\s+(\w+))?\s*$)");
  const std::regex declaration(R"(^\s*([A-Za-z_]\w*)\s*=\s*(.+?)\s*$)");
  std::istringstream lines(source);
  std::string line;
  while (std::getline(lines, line)) {
    std::smatch marker;
    if (std::regex_match(line, marker, exportMarker)) {
      exportNext = true;
      continue;
    }
    if (!exportNext) continue;
    std::smatch declarationMatch;
    if (!std::regex_match(line, declarationMatch, declaration)) continue;
    const std::string name = declarationMatch[1].str();
    const std::string rawValue = declarationMatch[2].str();
    ScriptProperty property;
    property.name = name;
    property.value = parseValue(rawValue);
    property.exported = true;
    if (std::holds_alternative<std::int64_t>(property.value)) property.type = ScriptPropertyType::Integer;
    else if (std::holds_alternative<float>(property.value)) property.type = ScriptPropertyType::Float;
    else if (std::holds_alternative<bool>(property.value)) property.type = ScriptPropertyType::Boolean;
    else property.type = ScriptPropertyType::String;
    result.push_back(std::move(property));
    exportNext = false;
  }
  return result;
}

void LuaScript::setBreakpoint(int line, bool enabled) {
  if (enabled) breakpoints_.insert(line);
  else breakpoints_.erase(line);
}

void LuaScript::reportError(const std::string& message) {
  lastError_ = message;
  ScriptDiagnostic diagnostic;
  diagnostic.severity = ScriptDiagnostic::Severity::Error;
  diagnostic.file = sourcePath_;
  diagnostic.message = message;
  const std::regex linePattern(R"(:([0-9]+):)");
  std::smatch match;
  if (std::regex_search(message, match, linePattern)) diagnostic.line = std::stoi(match[1].str());
  diagnostics_.push_back(diagnostic);
  log::error(message);
}

void LuaScript::shutdown() {
#if defined(COBALT_HAS_LUA)
  if (state_) lua_close(state_);
#endif
  state_ = nullptr;
}

}  // namespace cobalt