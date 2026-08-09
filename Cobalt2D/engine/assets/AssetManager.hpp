#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace cobalt {

enum class AssetType {
  Unknown,
  Texture,
  SpriteSheet,
  Audio,
  Music,
  Font,
  Script,
  Scene,
  Prefab,
  Tilemap,
  Tileset,
  Animation,
  Material,
  Shader,
  Particle,
  UI,
};

struct TextureImportSettings {
  bool pixelArt{false};
  bool generateMipmaps{false};
  bool linearFiltering{true};
  bool premultipliedAlpha{false};
  int atlasCellWidth{0};
  int atlasCellHeight{0};
};

struct AudioImportSettings {
  bool streaming{false};
  bool loop{false};
  float defaultVolume{1.0f};
};

struct AssetMetadata {
  std::string id;
  std::string relativePath;
  AssetType type{AssetType::Unknown};
  std::uintmax_t sizeBytes{0};
  std::int64_t modifiedTime{0};
  bool favorite{false};
  TextureImportSettings texture;
  AudioImportSettings audio;
  std::string importError;
};

class AssetManager {
public:
  bool scan(const std::filesystem::path& assetsRoot);
  bool refresh();
  bool importFile(const std::filesystem::path& source);
  bool rename(const std::string& id, const std::string& newName);
  bool duplicate(const std::string& id, const std::string& newName);
  bool move(const std::string& id, const std::filesystem::path& destination);
  bool remove(const std::string& id);
  AssetMetadata* find(const std::string& id);
  const AssetMetadata* find(const std::string& id) const;
  std::vector<AssetMetadata> search(const std::string& query = {},
                                    AssetType type = AssetType::Unknown) const;
  void setFavorite(const std::string& id, bool favorite);
  const std::filesystem::path& root() const { return root_; }
  const std::vector<AssetMetadata>& assets() const { return assets_; }

  static AssetType typeForPath(const std::filesystem::path& path);
  static std::string typeName(AssetType type);

private:
  bool indexFile(const std::filesystem::path& path);
  std::filesystem::path metadataPath(const AssetMetadata& asset) const;
  bool writeMetadata(const AssetMetadata& asset) const;
  std::filesystem::path root_;
  std::vector<AssetMetadata> assets_;
  std::unordered_map<std::string, std::size_t> byId_;
};

}  // namespace cobalt