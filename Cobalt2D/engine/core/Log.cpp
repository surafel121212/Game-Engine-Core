#include "engine/core/Log.hpp"

#include <cstdio>
#include <string>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

namespace cobalt::log {
namespace {
const char* categoryName(Category category) {
  switch (category) {
    case Category::Info: return "INFO";
    case Category::Debug: return "DEBUG";
    case Category::Warning: return "WARNING";
    case Category::Error: return "ERROR";
    case Category::Script: return "SCRIPT";
    case Category::Physics: return "PHYSICS";
    case Category::Render: return "RENDER";
    case Category::Audio: return "AUDIO";
    case Category::Android: return "ANDROID";
  }
  return "INFO";
}
void writeImpl(Category category, std::string_view message, int androidLevel) {
#if defined(__ANDROID__)
  __android_log_print(androidLevel, "Cobalt2D", "[%s] %s",
                      categoryName(category), std::string(message).c_str());
#else
  std::fprintf(androidLevel == 6 ? stderr : stdout, "[%s] %.*s\n",
               categoryName(category), static_cast<int>(message.size()), message.data());
#endif
}
}  // namespace

void write(Category category, std::string_view message) {
  const int androidLevel = category == Category::Error ? 6 :
                           category == Category::Warning ? 5 : 4;
  writeImpl(category, message, androidLevel);
}
void info(std::string_view message) { write(Category::Info, message); }
void debug(std::string_view message) { write(Category::Debug, message); }
void warn(std::string_view message) { write(Category::Warning, message); }
void error(std::string_view message) { write(Category::Error, message); }
}  // namespace cobalt::log
