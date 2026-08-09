#pragma once

#include "engine/core/Time.hpp"
#include "engine/core/Profiler.hpp"
#include "engine/ecs/Scene.hpp"
#include "engine/input/Input.hpp"
#include "engine/physics/PhysicsWorld.hpp"
#include "engine/render/Renderer2D.hpp"
#include "engine/scripting/ScriptRuntime.hpp"

#include <functional>
#include <memory>

namespace cobalt {

class Engine {
public:
  struct Config {
    int width{1280};
    int height{720};
    bool fixedTimestep{true};
    float fixedStep{1.0f / 60.0f};
    Vec2 gravity{0.0f, -20.0f};
  };

  Engine();
  explicit Engine(Config config);
  ~Engine();
  bool initialize();
  void update();
  void render();
  void run(std::function<bool()> shouldContinue = {});
  void shutdown();
  void pause() { time_.setPaused(true); }
  void resume() { time_.setPaused(false); }
  void onSurfaceCreated(int width, int height);
  void onSurfaceDestroyed();

  Time& time() { return time_; }
  Input& input() { return input_; }
  Renderer2D& renderer() { return renderer_; }
  PhysicsWorld& physics() { return physics_; }
  Profiler& profiler() { return profiler_; }
  ScriptRuntime& scripts() { return scripts_; }
  Scene& scene() { return *scene_; }
  void setScene(std::unique_ptr<Scene> scene);
  bool isInitialized() const { return initialized_; }

private:
  Config config_;
  Time time_;
  Profiler profiler_;
  Input input_;
  Renderer2D renderer_;
  PhysicsWorld physics_;
  ScriptRuntime scripts_;
  std::unique_ptr<Scene> scene_;
  bool initialized_{false};
  bool surfaceReady_{false};
  float fixedAccumulator_{0.0f};
};

}  // namespace cobalt