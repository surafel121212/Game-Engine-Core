#include "engine/scripting/LuaScript.hpp"

#include "engine/core/Log.hpp"

#if defined(COBALT_HAS_LUA)
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#endif

namespace cobalt {

LuaScript::~LuaScript() { shutdown(); }

bool LuaScript::initialize() {
#if defined(COBALT_HAS_LUA)
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
#if defined(COBALT_HAS_LUA)
  if (!state_ && !initialize()) return false;
  if (luaL_dofile(state_, path.c_str()) != LUA_OK) {
    lastError_ = lua_tostring(state_, -1);
    lua_pop(state_, 1);
    log::error(lastError_);
    return false;
  }
  return true;
#else
  (void)path;
  lastError_ = "Lua support was disabled at configure time";
  return false;
#endif
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
    lastError_ = lua_tostring(state_, -1);
    lua_pop(state_, 1);
    log::error(lastError_);
    return false;
  }
  return true;
#else
  (void)functionName;
  return false;
#endif
}

void LuaScript::update(float deltaSeconds) {
#if defined(COBALT_HAS_LUA)
  if (!state_) return;
  lua_getglobal(state_, "onUpdate");
  if (!lua_isfunction(state_, -1)) { lua_pop(state_, 1); return; }
  lua_pushnumber(state_, deltaSeconds);
  if (lua_pcall(state_, 1, 0, 0) != LUA_OK) {
    lastError_ = lua_tostring(state_, -1);
    lua_pop(state_, 1);
    log::error(lastError_);
  }
#else
  (void)deltaSeconds;
#endif
}

void LuaScript::shutdown() {
#if defined(COBALT_HAS_LUA)
  if (state_) lua_close(state_);
#endif
  state_ = nullptr;
}

}  // namespace cobalt