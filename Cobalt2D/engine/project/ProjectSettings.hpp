#pragma once

#include "engine/math/Math.hpp"

#include <string>
#include <vector>

namespace cobalt {

enum class Orientation { Portrait, Landscape, Automatic };
enum class QualityPreset { Low, Medium, High, Ultra };

struct PerformanceSettings {
  int targetFps{60};
  QualityPreset quality{QualityPreset::High};
  float resolutionScale{1.0f};
  bool vsync{true};
  bool lighting{true};
  bool shadows{false};
  int particleQuality{2};
};

struct AndroidSettings {
  std::string packageId{"dev.cobalt.game"};
  int minSdk{24};
  int targetSdk{35};
  bool arm64{true};
  bool armeabiV7a{false};
  bool vibration{false};
  bool notifications{false};
  bool internet{false};
};

struct ProjectSettings {
  std::string name{"Cobalt Game"};
  std::string engineVersion{"0.2.0"};
  std::string defaultScene{"Scenes/Main.scene"};
  Orientation orientation{Orientation::Landscape};
  Vec2 resolution{1280.0f, 720.0f};
  float gravity{ -20.0f };
  PerformanceSettings performance{};
  AndroidSettings android{};
  std::vector<std::string> inputActions{"MoveLeft", "MoveRight", "Jump", "Attack", "Pause"};

  bool save(const std::string& path, std::string* error = nullptr) const;
  static bool load(const std::string& path, ProjectSettings& settings,
                   std::string* error = nullptr);
};

}  // namespace cobalt