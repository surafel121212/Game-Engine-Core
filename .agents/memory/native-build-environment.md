---
name: Native build environment
description: The available local toolchain constraint for the Cobalt2D source project.
---

The Replit shell used for this project has a C++ compiler but no CMake binary.

**Why:** The engine is intentionally a CMake/Android project, but local CMake
configuration cannot be executed in this environment.

**How to apply:** Use direct C++17 syntax and native test compilation for local
regressions; use the checked-in GitHub Actions workflow or Android Studio to
exercise the CMake/NDK build.