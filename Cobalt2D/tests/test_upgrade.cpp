#include "engine/assets/AssetManager.hpp"
#include "engine/editor/CommandHistory.hpp"
#include "engine/project/ProjectSettings.hpp"
#include "engine/scripting/LuaScript.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>

int main() {
  using namespace cobalt;
  const auto root = std::filesystem::temp_directory_path() / "cobalt-upgrade-assets";
  std::filesystem::remove_all(root);
  std::filesystem::create_directories(root / "Scripts");
  {
    std::ofstream script(root / "Scripts" / "Player.lua");
    script << "---@export\nspeed = 250\n---@export\nplayerName = \"Player\"\n";
  }

  AssetManager assets;
  assert(assets.scan(root));
  assert(assets.find("Scripts/Player.lua") != nullptr);
  assert(assets.find("Scripts/Player.lua")->type == AssetType::Script);
  assert(assets.search("player").size() == 1);
  assets.setFavorite("Scripts/Player.lua", true);
  assert(assets.find("Scripts/Player.lua")->favorite);

  const auto properties = LuaScript::inspectSource(
      "---@export\nspeed = 250\n---@export\nplayerName = \"Player\"\n");
  assert(properties.size() == 2);
  assert(properties[0].name == "speed");
  assert(properties[1].type == ScriptPropertyType::String);

  ProjectSettings settings;
  settings.name = "Upgrade Test";
  settings.android.packageId = "dev.cobalt.upgrade";
  const auto settingsPath = root / "project.cobalt";
  assert(settings.save(settingsPath.string()));
  ProjectSettings loaded;
  assert(ProjectSettings::load(settingsPath.string(), loaded));
  assert(loaded.name == settings.name);
  assert(loaded.android.packageId == settings.android.packageId);

  int value = 0;
  CommandHistory history;
  history.execute(std::make_unique<LambdaCommand>(
      "Increment", [&value] { ++value; }, [&value] { --value; }));
  assert(value == 1 && history.canUndo());
  assert(history.undo() && value == 0);
  assert(history.redo() && value == 1);

  std::filesystem::remove_all(root);
  return 0;
}