#include "engine/serialization/SceneSerializer.hpp"

#include <fstream>
#include <regex>
#include <sstream>

namespace cobalt {
namespace {
std::string escape(const std::string& value) {
  std::string result;
  for (const char character : value) {
    if (character == '"' || character == '\\') result += '\\';
    result += character;
  }
  return result;
}
std::string readString(const std::string& source, const std::string& key) {
  const std::regex pattern("\"" + key + "\"\\s*:\\s*\"([^\"]*)\"");
  std::smatch match;
  return std::regex_search(source, match, pattern) ? match[1].str() : "";
}
}  // namespace

bool SceneSerializer::save(const Scene& scene, const std::string& path, std::string* error) {
  std::ofstream output(path);
  if (!output) {
    if (error) *error = "Unable to open scene for writing: " + path;
    return false;
  }
  output << "{\n  \"name\": \"" << escape(scene.name()) << "\",\n  \"objects\": [\n";
  const auto objects = scene.objects();
  for (std::size_t index = 0; index < objects.size(); ++index) {
    const Entity& object = *objects[index];
    output << "    {\"id\": " << object.id() << ", \"name\": \""
           << escape(object.name()) << "\", \"enabled\": "
           << (object.enabled() ? "true" : "false") << ", \"parent\": "
           << object.parent() << "}";
    if (index + 1 < objects.size()) output << ",";
    output << "\n";
  }
  output << "  ]\n}\n";
  return true;
}

bool SceneSerializer::load(Scene& scene, const std::string& path, std::string* error) {
  std::ifstream input(path);
  if (!input) {
    if (error) *error = "Unable to open scene for reading: " + path;
    return false;
  }
  std::stringstream buffer;
  buffer << input.rdbuf();
  const std::string source = buffer.str();
  const std::string sceneName = readString(source, "name");
  if (sceneName.empty()) {
    if (error) *error = "Scene is missing a name: " + path;
    return false;
  }
  scene.clear();
  scene.setName(sceneName);
  const std::regex objectPattern(
    R"REGEX(\{"id"\s*:\s*(\d+),\s*"name"\s*:\s*"([^"]*)",\s*"enabled"\s*:\s*(true|false),\s*"parent"\s*:\s*(\d+)\})REGEX");
  for (auto it = std::sregex_iterator(source.begin(), source.end(), objectPattern);
       it != std::sregex_iterator(); ++it) {
    const auto& match = *it;
    Entity& entity = scene.createObject(match[2].str());
    entity.setEnabled(match[3].str() == "true");
    entity.setParent(static_cast<EntityId>(std::stoull(match[4].str())));
  }
  return true;
}

}  // namespace cobalt