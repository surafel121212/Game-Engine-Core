#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace cobalt {

struct PerformanceSnapshot {
  float fps{0.0f};
  float frameTimeMs{0.0f};
  float cpuTimeMs{0.0f};
  float gpuTimeMs{0.0f};
  std::uint32_t drawCalls{0};
  std::uint32_t triangles{0};
  std::uint64_t textureMemoryBytes{0};
  std::uint32_t entityCount{0};
  std::uint32_t physicsBodies{0};
  float luaTimeMs{0.0f};
};

class Profiler {
public:
  class Scope {
  public:
    Scope(Profiler& profiler, std::string name);
    ~Scope();
  private:
    Profiler& profiler_;
    std::string name_;
    std::chrono::steady_clock::time_point start_;
  };

  void beginFrame();
  void endFrame(float fps);
  void addSample(const std::string& name, float milliseconds);
  void setDrawCalls(std::uint32_t value) { snapshot_.drawCalls = value; }
  void setTriangles(std::uint32_t value) { snapshot_.triangles = value; }
  void setEntityCount(std::uint32_t value) { snapshot_.entityCount = value; }
  void setPhysicsBodies(std::uint32_t value) { snapshot_.physicsBodies = value; }
  void setLuaTimeMs(float value) { snapshot_.luaTimeMs = value; }
  const PerformanceSnapshot& snapshot() const { return snapshot_; }
  float sample(const std::string& name) const;

private:
  PerformanceSnapshot snapshot_;
  std::unordered_map<std::string, float> samples_;
  std::chrono::steady_clock::time_point frameStart_;
};

}  // namespace cobalt