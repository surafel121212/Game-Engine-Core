#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace cobalt {

struct Vec2 {
  float x{0.0f};
  float y{0.0f};

  constexpr Vec2() = default;
  constexpr Vec2(float xValue, float yValue) : x(xValue), y(yValue) {}

  constexpr Vec2 operator+(const Vec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
  constexpr Vec2 operator-(const Vec2& rhs) const { return {x - rhs.x, y - rhs.y}; }
  constexpr Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
  constexpr Vec2 operator/(float scalar) const { return {x / scalar, y / scalar}; }
  Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
  Vec2& operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

  float lengthSquared() const { return x * x + y * y; }
  float length() const { return std::sqrt(lengthSquared()); }
  Vec2 normalized() const {
    const float magnitude = length();
    return magnitude > 0.00001f ? *this / magnitude : Vec2{};
  }
  static float dot(const Vec2& lhs, const Vec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
};

struct Vec3 {
  float x{0.0f};
  float y{0.0f};
  float z{0.0f};
  constexpr Vec3() = default;
  constexpr Vec3(float xValue, float yValue, float zValue) : x(xValue), y(yValue), z(zValue) {}
};

struct Color {
  float r{1.0f};
  float g{1.0f};
  float b{1.0f};
  float a{1.0f};
  constexpr Color() = default;
  constexpr Color(float red, float green, float blue, float alpha = 1.0f)
      : r(red), g(green), b(blue), a(alpha) {}
};

struct Transform2D {
  Vec2 position{};
  float rotation{0.0f};
  Vec2 scale{1.0f, 1.0f};

  Transform2D combined(const Transform2D& parent) const {
    const float c = std::cos(parent.rotation);
    const float s = std::sin(parent.rotation);
    const Vec2 scaled{position.x * parent.scale.x, position.y * parent.scale.y};
    return {
      parent.position + Vec2{scaled.x * c - scaled.y * s, scaled.x * s + scaled.y * c},
      parent.rotation + rotation,
      {parent.scale.x * scale.x, parent.scale.y * scale.y}
    };
  }
};

struct Rect {
  Vec2 min{};
  Vec2 max{};
  bool contains(const Vec2& point) const {
    return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
  }
};

struct Mat4 {
  float values[16]{};

  static Mat4 identity() {
    Mat4 result{};
    result.values[0] = result.values[5] = result.values[10] = result.values[15] = 1.0f;
    return result;
  }
  static Mat4 orthographic(float left, float right, float bottom, float top) {
    Mat4 result = identity();
    result.values[0] = 2.0f / (right - left);
    result.values[5] = 2.0f / (top - bottom);
    result.values[10] = -1.0f;
    result.values[12] = -(right + left) / (right - left);
    result.values[13] = -(top + bottom) / (top - bottom);
    return result;
  }
};

}  // namespace cobalt