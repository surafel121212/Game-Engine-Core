#include "engine/assets/AssetManager.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace cobalt {
namespace {
std::string lower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
  return value;
}
std::string idFor(const std::filesystem::path& root, const std::filesystem::path& path) {
  return std::filesystem::relative(path, root).generic_string();
}
}  // namespace

AssetType AssetManager::typeForPath(const std::filesystem::path& path) {
  const std::string extension = lower(path.extension().string());
  if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".webp") return AssetType::Texture;
  if (extension == ".spritesheet" || extension == ".atlas") return AssetType::SpriteSheet;
  if (extension == ".wav" || extension == ".ogg" || extension == ".mp3") return AssetType::Audio;
  if (extension == ".music") return AssetType::Music;
  if (extension == ".ttf" || extension == ".otf") return AssetType::Font;
  if (extension == ".lua") return AssetType::Script;
  if (extension == ".scene") return AssetType::Scene;
  if (extension == ".prefab") return AssetType::Prefab;
  if (extension == ".tilemap") return AssetType::Tilemap;
  if (extension == ".tileset") return AssetType::Tileset;
  if (extension == ".anim") return AssetType::Animation;
  if (extension == ".material") return AssetType::Material;
  if (extension == ".shader" || extension == ".glsl") return AssetType::Shader;
  if (extension == ".particle") return AssetType::Particle;
  if (extension == ".ui") return AssetType::UI;
  return AssetType::Unknown;
}

std::string AssetManager::typeName(AssetType type) {
  switch (type) {
    case AssetType::Texture: return "Texture";
    case AssetType::SpriteSheet: return "Sprite Sheet";
    case AssetType::Audio: return "Audio";
    case AssetType::Music: return "Music";
    case AssetType::Font: return "Font";
    case AssetType::Script: return "Lua Script";
    case AssetType::Scene: return "Scene";
    case AssetType::Prefab: return "Prefab";
    case AssetType::Tilemap: return "Tilemap";
    case AssetType::Tileset: return "Tileset";
    case AssetType::Animation: return "Animation";
    case AssetType::Material: return "Material";
    case AssetType::Shader: return "Shader";
    case AssetType::Particle: return "Particle";
    case AssetType::UI: return "UI";
    default: return "Unknown";
  }
}

bool AssetManager::scan(const std::filesystem::path& assetsRoot) {
  root_ = assetsRoot;
  assets_.clear();
  byId_.clear();
  if (!std::filesystem::exists(root_)) {
    std::error_code error;
    if (!std::filesystem::create_directories(root_, error) || error) return false;
  }
  for (const auto& entry : std::filesystem::recursive_directory_iterator(root_)) {
    if (entry.is_regular_file() && entry.path().extension() != ".meta") indexFile(entry.path());
  }
  return true;
}

bool AssetManager::refresh() { return scan(root_); }

bool AssetManager::indexFile(const std::filesystem::path& path) {
  const AssetType type = typeForPath(path);
  if (type == AssetType::Unknown) return false;
  std::error_code error;
  AssetMetadata asset;
  asset.id = idFor(root_, path);
  asset.relativePath = asset.id;
  asset.type = type;
  asset.sizeBytes = std::filesystem::file_size(path, error);
  if (error) asset.importError = error.message();
  const auto timestamp = std::filesystem::last_write_time(path, error);
  if (!error) asset.modifiedTime = timestamp.time_since_epoch().count();
  const std::size_t index = assets_.size();
  assets_.push_back(asset);
  byId_[asset.id] = index;
  return writeMetadata(asset);
}

std::filesystem::path AssetManager::metadataPath(const AssetMetadata& asset) const {
  return root_ / (asset.relativePath + ".meta");
}

bool AssetManager::writeMetadata(const AssetMetadata& asset) const {
  std::ofstream output(metadataPath(asset));
  if (!output) return false;
  output << "id=" << asset.id << "\n"
         << "type=" << typeName(asset.type) << "\n"
         << "sizeBytes=" << asset.sizeBytes << "\n"
         << "favorite=" << (asset.favorite ? "true" : "false") << "\n"
         << "pixelArt=" << (asset.texture.pixelArt ? "true" : "false") << "\n"
         << "linearFiltering=" << (asset.texture.linearFiltering ? "true" : "false") << "\n";
  return true;
}

AssetMetadata* AssetManager::find(const std::string& id) {
  const auto it = byId_.find(id);
  return it == byId_.end() ? nullptr : &assets_[it->second];
}
const AssetMetadata* AssetManager::find(const std::string& id) const {
  const auto it = byId_.find(id);
  return it == byId_.end() ? nullptr : &assets_[it->second];
}

std::vector<AssetMetadata> AssetManager::search(const std::string& query, AssetType type) const {
  std::vector<AssetMetadata> result;
  const std::string normalizedQuery = lower(query);
  for (const auto& asset : assets_) {
    const bool matchesType = type == AssetType::Unknown || asset.type == type;
    const bool matchesQuery = normalizedQuery.empty() ||
      lower(asset.relativePath).find(normalizedQuery) != std::string::npos;
    if (matchesType && matchesQuery) result.push_back(asset);
  }
  return result;
}

void AssetManager::setFavorite(const std::string& id, bool favorite) {
  if (auto* asset = find(id)) {
    asset->favorite = favorite;
    writeMetadata(*asset);
  }
}

bool AssetManager::importFile(const std::filesystem::path& source) {
  if (!std::filesystem::is_regular_file(source)) return false;
  const std::filesystem::path destination = root_ / source.filename();
  std::error_code error;
  std::filesystem::copy_file(source, destination,
                             std::filesystem::copy_options::overwrite_existing, error);
  return !error && indexFile(destination);
}

bool AssetManager::rename(const std::string& id, const std::string& newName) {
  AssetMetadata* asset = find(id);
  if (!asset || newName.empty()) return false;
  const auto oldPath = root_ / asset->relativePath;
  const auto newPath = oldPath.parent_path() / newName;
  std::error_code error;
  std::filesystem::rename(oldPath, newPath, error);
  if (error) return false;
  return refresh();
}

bool AssetManager::duplicate(const std::string& id, const std::string& newName) {
  const AssetMetadata* asset = find(id);
  if (!asset) return false;
  std::error_code error;
  std::filesystem::copy_file(root_ / asset->relativePath,
                             root_ / newName,
                             std::filesystem::copy_options::none, error);
  return !error && refresh();
}

bool AssetManager::move(const std::string& id, const std::filesystem::path& destination) {
  AssetMetadata* asset = find(id);
  if (!asset) return false;
  std::error_code error;
  std::filesystem::create_directories(root_ / destination, error);
  if (error) return false;
  std::filesystem::rename(root_ / asset->relativePath,
                          root_ / destination / std::filesystem::path(asset->relativePath).filename(),
                          error);
  return !error && refresh();
}

bool AssetManager::remove(const std::string& id) {
  const AssetMetadata* asset = find(id);
  if (!asset) return false;
  std::error_code error;
  const bool removed = std::filesystem::remove(root_ / asset->relativePath, error);
  std::filesystem::remove(metadataPath(*asset), error);
  return removed && refresh();
}

}  // namespace cobalt