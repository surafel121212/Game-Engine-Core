#pragma once

#include <string_view>

namespace cobalt::log {
enum class Category {
  Info,
  Debug,
  Warning,
  Error,
  Script,
  Physics,
  Render,
  Audio,
  Android,
};

void write(Category category, std::string_view message);
void info(std::string_view message);
void debug(std::string_view message);
void warn(std::string_view message);
void error(std::string_view message);
}  // namespace cobalt::log