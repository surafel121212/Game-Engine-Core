#include "engine/core/Engine.hpp"

#include "engine/core/Log.hpp"

#include <thread>

namespace cobalt {

Engine::Engine() : Engine(Config{}) {}

Engine::Engine(Config config)
    : config_(config), physics_(config.gravity), scene_(std::make_unique<Scene>("Main")) {}
Engine::~Engine() { shutdown(); }

bool Engine::initialize() {
  if (initialized_) return true;
  if (!renderer_.initialize(config_.width, config_.height) || !physics_.initialize()) {
    log::error("Engine initialization failed");
    return false;
  }
  input_.bind("MoveLeft", Key::Left);
  input_.bind("MoveRight", Key::Right);
  input_.bind("Jump", Key::Space);
  scripts_.onCreate();
  scripts_.onAwake();
  scripts_.onStart();
  initialized_ = true;
  surfaceReady_ = true;
  log::info("Cobalt2D engine initialized");
  return true;
}

void Engine::update() {
  if (!initialized_) return;
  time_.beginFrame();
  profiler_.beginFrame();
  input_.beginFrame();
  if (config_.fixedTimestep) {
    fixedAccumulator_ += time_.deltaSeconds();
    while (fixedAccumulator_ >= config_.fixedStep) {
      physics_.step(config_.fixedStep);
      scripts_.fixedUpdate(config_.fixedStep);
      fixedAccumulator_ -= config_.fixedStep;
    }
  } else {
    physics_.step(time_.deltaSeconds());
  }
  scripts_.update(time_.deltaSeconds());
  scripts_.lateUpdate(time_.deltaSeconds());
}

void Engine::render() {
  if (!initialized_ || !surfaceReady_) return;
  renderer_.beginFrame();
  renderer_.endFrame();
  profiler_.setDrawCalls(renderer_.drawCallCount());
  profiler_.endFrame(time_.fps());
  time_.endFrame();
}

void Engine::run(std::function<bool()> shouldContinue) {
  if (!initialize()) return;
  while (!shouldContinue || shouldContinue()) {
    update();
    render();
    std::this_thread::yield();
  }
}

void Engine::shutdown() {
  if (!initialized_) return;
  physics_.shutdown();
  scripts_.clear();
  renderer_.shutdown();
  initialized_ = false;
  surfaceReady_ = false;
}

void Engine::onSurfaceCreated(int width, int height) {
  if (!initialized_) initialize();
  renderer_.resize(width, height);
  surfaceReady_ = true;
}
void Engine::onSurfaceDestroyed() { surfaceReady_ = false; }
void Engine::setScene(std::unique_ptr<Scene> scene) {
  if (scene) scene_ = std::move(scene);
}

}  // namespace cobalt