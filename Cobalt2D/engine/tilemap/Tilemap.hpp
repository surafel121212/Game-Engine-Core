#pragma once

#include "engine/math/Math.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace cobalt {

struct Tile {
  std::uint32_t id{0};
  bool flippedX{false};
  bool flippedY{false};
  std::uint8_t rotation{0};
};

class Tilemap {
public:
  Tilemap(int width, int height, int tileWidth, int tileHeight);
  bool set(int x, int y, Tile tile);
  Tile get(int x, int y) const;
  void clear();
  std::vector<Tile> floodFill(int x, int y, Tile replacement);
  int width() const { return width_; }
  int height() const { return height_; }
  int tileWidth() const { return tileWidth_; }
  int tileHeight() const { return tileHeight_; }
  Vec2 worldToCell(Vec2 world) const;
  Vec2 cellToWorld(int x, int y) const;

private:
  bool inBounds(int x, int y) const;
  int width_;
  int height_;
  int tileWidth_;
  int tileHeight_;
  std::vector<Tile> tiles_;
};

}  // namespace cobalt