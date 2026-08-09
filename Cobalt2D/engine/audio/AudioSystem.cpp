#include "engine/audio/AudioSystem.hpp"

#include <algorithm>

namespace cobalt {
bool AudioSystem::initialize() { initialized_ = true; return true; }
void AudioSystem::shutdown() { musicAsset_.clear(); initialized_ = false; }
bool AudioSystem::playSound(const std::string& asset, float volume, float pitch) {
  return initialized_ && !asset.empty() && volume >= 0.0f && pitch > 0.0f;
}
bool AudioSystem::playMusic(const std::string& asset, bool loop) {
  (void)loop;
  if (!initialized_ || asset.empty()) return false;
  musicAsset_ = asset;
  return true;
}
void AudioSystem::stopMusic() { musicAsset_.clear(); }
void AudioSystem::setGroupVolume(const std::string& group, float volume) {
  groups_[group] = std::clamp(volume, 0.0f, 1.0f);
}
float AudioSystem::groupVolume(const std::string& group) const {
  const auto it = groups_.find(group);
  return it == groups_.end() ? 0.0f : it->second;
}
}  // namespace cobalt