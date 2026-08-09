#include "engine/core/Log.hpp"

#include <cstdio>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

namespace cobalt::log {
namespace {
void write(const char* level, std::string_view message, int androidLevel) {
#if defined(__ANDROID__)
  __android_log_print(androidLevel, "Cobalt2D", "%s", std::string(message).c_str());
#else
  std::fprintf(androidLevel == 6 ? stderr : stdout, "[%s] %.*s\n",
               level, static_cast<int>(message.size()), message.data());
#endif
}
}  // namespace

void info(std::string_view message) { write("INFO", message, 4); }
void warn(std::string_view message) { write("WARN", message, 5); }
void error(std::string_view message) { write("ERROR", message, 6); }
}  // namespace cobalt::log