#pragma once

#include <string>
#include <unordered_map>

namespace cobalt {

class AudioSystem {
public:
  bool initialize();
  void shutdown();
  bool playSound(const std::string& asset, float volume = 1.0f, float pitch = 1.0f);
  bool playMusic(const std::string& asset, bool loop = true);
  void stopMusic();
  void setGroupVolume(const std::string& group, float volume);
  float groupVolume(const std::string& group) const;
  bool isInitialized() const { return initialized_; }

private:
  bool initialized_{false};
  std::unordered_map<std::string, float> groups_{{"Master", 1.0f}, {"Music", 1.0f}, {"SFX", 1.0f}};
  std::string musicAsset_;
};

}  // namespace cobalt