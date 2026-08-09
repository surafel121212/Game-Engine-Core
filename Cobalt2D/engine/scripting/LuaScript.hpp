#pragma once

#include <string>

struct lua_State;

namespace cobalt {

class LuaScript {
public:
  LuaScript() = default;
  ~LuaScript();
  LuaScript(const LuaScript&) = delete;
  LuaScript& operator=(const LuaScript&) = delete;

  bool initialize();
  bool loadFile(const std::string& path);
  bool call(const char* functionName);
  void update(float deltaSeconds);
  void shutdown();
  const std::string& lastError() const { return lastError_; }

private:
  lua_State* state_{nullptr};
  std::string lastError_;
};

}  // namespace cobalt