#include "engine/project/ProjectSettings.hpp"

#include <fstream>
#include <sstream>

namespace cobalt {
namespace {
std::string quote(const std::string& value) {
  return "\"" + value + "\"";
}
std::string valueFor(const std::string& source, const std::string& key,
                     const std::string& fallback = {}) {
  const std::string marker = quote(key) + ":";
  const std::size_t begin = source.find(marker);
  if (begin == std::string::npos) return fallback;
  const std::size_t valueBegin = source.find_first_not_of(" \t", begin + marker.size());
  if (valueBegin == std::string::npos) return fallback;
  if (source[valueBegin] == '"') {
    const std::size_t end = source.find('"', valueBegin + 1);
    return end == std::string::npos ? fallback : source.substr(valueBegin + 1, end - valueBegin - 1);
  }
  const std::size_t end = source.find_first_of(",\n}", valueBegin);
  return source.substr(valueBegin, end == std::string::npos ? std::string::npos : end - valueBegin);
}
int intValue(const std::string& source, const std::string& key, int fallback) {
  try { return std::stoi(valueFor(source, key)); } catch (...) { return fallback; }
}
float floatValue(const std::string& source, const std::string& key, float fallback) {
  try { return std::stof(valueFor(source, key)); } catch (...) { return fallback; }
}
bool boolValue(const std::string& source, const std::string& key, bool fallback) {
  const std::string value = valueFor(source, key);
  return value == "true" ? true : value == "false" ? false : fallback;
}
}  // namespace

bool ProjectSettings::save(const std::string& path, std::string* error) const {
  std::ofstream output(path);
  if (!output) {
    if (error) *error = "Unable to open project settings for writing: " + path;
    return false;
  }
  output << "{\n"
         << "  \"name\": " << quote(name) << ",\n"
         << "  \"engineVersion\": " << quote(engineVersion) << ",\n"
         << "  \"defaultScene\": " << quote(defaultScene) << ",\n"
         << "  \"orientation\": " << quote(orientation == Orientation::Portrait ? "portrait" :
                                         orientation == Orientation::Landscape ? "landscape" : "automatic") << ",\n"
         << "  \"width\": " << resolution.x << ",\n"
         << "  \"height\": " << resolution.y << ",\n"
         << "  \"gravity\": " << gravity << ",\n"
         << "  \"targetFps\": " << performance.targetFps << ",\n"
         << "  \"resolutionScale\": " << performance.resolutionScale << ",\n"
         << "  \"vsync\": " << (performance.vsync ? "true" : "false") << ",\n"
         << "  \"packageId\": " << quote(android.packageId) << ",\n"
         << "  \"minSdk\": " << android.minSdk << ",\n"
         << "  \"targetSdk\": " << android.targetSdk << ",\n"
         << "  \"arm64\": " << (android.arm64 ? "true" : "false") << ",\n"
         << "  \"vibration\": " << (android.vibration ? "true" : "false") << ",\n"
         << "  \"internet\": " << (android.internet ? "true" : "false") << "\n}\n";
  return true;
}

bool ProjectSettings::load(const std::string& path, ProjectSettings& settings, std::string* error) {
  std::ifstream input(path);
  if (!input) {
    if (error) *error = "Unable to open project settings for reading: " + path;
    return false;
  }
  std::stringstream buffer;
  buffer << input.rdbuf();
  const std::string source = buffer.str();
  settings.name = valueFor(source, "name", settings.name);
  settings.engineVersion = valueFor(source, "engineVersion", settings.engineVersion);
  settings.defaultScene = valueFor(source, "defaultScene", settings.defaultScene);
  const std::string orientation = valueFor(source, "orientation", "landscape");
  settings.orientation = orientation == "portrait" ? Orientation::Portrait :
                         orientation == "automatic" ? Orientation::Automatic : Orientation::Landscape;
  settings.resolution.x = floatValue(source, "width", settings.resolution.x);
  settings.resolution.y = floatValue(source, "height", settings.resolution.y);
  settings.gravity = floatValue(source, "gravity", settings.gravity);
  settings.performance.targetFps = intValue(source, "targetFps", settings.performance.targetFps);
  settings.performance.resolutionScale = floatValue(source, "resolutionScale", settings.performance.resolutionScale);
  settings.performance.vsync = boolValue(source, "vsync", settings.performance.vsync);
  settings.android.packageId = valueFor(source, "packageId", settings.android.packageId);
  settings.android.minSdk = intValue(source, "minSdk", settings.android.minSdk);
  settings.android.targetSdk = intValue(source, "targetSdk", settings.android.targetSdk);
  settings.android.arm64 = boolValue(source, "arm64", settings.android.arm64);
  settings.android.vibration = boolValue(source, "vibration", settings.android.vibration);
  settings.android.internet = boolValue(source, "internet", settings.android.internet);
  return true;
}

}  // namespace cobalt