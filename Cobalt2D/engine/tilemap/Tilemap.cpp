#include "engine/tilemap/Tilemap.hpp"

#include <algorithm>
#include <cmath>
#include <queue>

namespace cobalt {

Tilemap::Tilemap(int width, int height, int tileWidth, int tileHeight)
    : width_(width), height_(height), tileWidth_(tileWidth), tileHeight_(tileHeight),
      tiles_(static_cast<std::size_t>(width * height)) {}

bool Tilemap::inBounds(int x, int y) const {
  return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool Tilemap::set(int x, int y, Tile tile) {
  if (!inBounds(x, y)) return false;
  tiles_[static_cast<std::size_t>(y * width_ + x)] = tile;
  return true;
}

Tile Tilemap::get(int x, int y) const {
  return inBounds(x, y) ? tiles_[static_cast<std::size_t>(y * width_ + x)] : Tile{};
}

void Tilemap::clear() { std::fill(tiles_.begin(), tiles_.end(), Tile{}); }

std::vector<Tile> Tilemap::floodFill(int x, int y, Tile replacement) {
  std::vector<Tile> changed;
  if (!inBounds(x, y)) return changed;
  const Tile original = get(x, y);
  if (original.id == replacement.id && original.flippedX == replacement.flippedX &&
      original.flippedY == replacement.flippedY && original.rotation == replacement.rotation) {
    return changed;
  }
  std::queue<Vec2> pending;
  pending.push({static_cast<float>(x), static_cast<float>(y)});
  while (!pending.empty()) {
    const Vec2 cell = pending.front();
    pending.pop();
    const int cx = static_cast<int>(cell.x);
    const int cy = static_cast<int>(cell.y);
    if (!inBounds(cx, cy)) continue;
    if (get(cx, cy).id != original.id) continue;
    set(cx, cy, replacement);
    changed.push_back(replacement);
    pending.push({static_cast<float>(cx + 1), static_cast<float>(cy)});
    pending.push({static_cast<float>(cx - 1), static_cast<float>(cy)});
    pending.push({static_cast<float>(cx), static_cast<float>(cy + 1)});
    pending.push({static_cast<float>(cx), static_cast<float>(cy - 1)});
  }
  return changed;
}

Vec2 Tilemap::worldToCell(Vec2 world) const {
  return {std::floor(world.x / static_cast<float>(tileWidth_)),
          std::floor(world.y / static_cast<float>(tileHeight_))};
}

Vec2 Tilemap::cellToWorld(int x, int y) const {
  return {static_cast<float>(x * tileWidth_), static_cast<float>(y * tileHeight_)};
}

}  // namespace cobalt