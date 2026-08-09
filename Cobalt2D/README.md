# Cobalt2D

Cobalt2D is an original, modular C++17 2D game engine aimed at Android games.
It provides a small engine core that can be embedded in a native Android
application, while keeping gameplay code in scenes, reusable components, and
Lua scripts.

## What is included

- Fixed or variable timestep engine lifecycle with pause/resume and FPS timing.
- Entity/component scene model with parent IDs, typed components, and JSON-like
  human-readable scene files.
- Transform, sprite command buffering, camera conversion, input actions,
  Box2D-ready physics world, Lua script host, sprite animation, tilemaps,
  particles, audio facade, and touch-friendly UI primitives.
- Android Studio project using Java `SurfaceView`, JNI lifecycle callbacks,
  EGL, and OpenGL ES 3.
- Platformer example with scene, tilemap, and actual Lua movement/jump code.
- Native tests for math, scene serialization, input, animation, tilemaps, and
  particles.
- GitHub Actions workflow that installs Android SDK/NDK/CMake and uploads the
  debug APK.

The project is deliberately honest about boundaries: the renderer exposes a
mobile-friendly command buffer and Android GL context, while texture decoding,
font atlas generation, and editor tooling can be layered on without changing
gameplay code. Unsupported assets fail through explicit return values and
logged errors rather than silent fallback behavior.

## Repository layout

```text
engine/
  core/             lifecycle, timing, logging
  math/             vectors, colors, transforms, matrices
  ecs/              entities, components, scenes
  render/           renderer command buffer and camera
  physics/          Box2D-backed physics boundary
  input/            keyboard, action map, touch state
  scripting/        Lua lifecycle and error boundary
  serialization/    scene persistence
  animation/        sprite clip playback
  tilemap/          grid operations and flood fill
  particles/        allocation-bounded particle simulation
  audio/            audio groups and playback boundary
  ui/               touch-friendly UI primitives
android/            Android Studio + JNI/OpenGL ES shell
Examples/Platformer/scene, tilemap, and Lua gameplay example
tests/              native regression tests
.github/workflows/  reproducible Android CI
```

## Desktop native checks

The source tree can be syntax-checked without Android:

```bash
g++ -std=c++17 -I. -DCOBALT_ENABLE_BOX2D=0 -DCOBALT_ENABLE_LUA=0 \
  -fsyntax-only engine/**/*.cpp
```

For the complete native build, install CMake, then configure:

```bash
cmake -S . -B build -DCOBALT_ENABLE_BOX2D=ON -DCOBALT_ENABLE_LUA=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Box2D and Lua are fetched by CMake for the desktop build. Android keeps the
engine build self-contained at this stage and uses the same API boundaries;
add the pinned native dependencies to `android/app/src/main/cpp/CMakeLists.txt`
when shipping a physics-heavy Android game.

## Android Studio

Open the `android/` directory in Android Studio. The app module is already
configured with:

- application ID `dev.cobalt.engine`
- minimum SDK 24
- target/compile SDK 35
- C++17 external native build
- landscape orientation
- Java `SurfaceView` and native EGL/OpenGL ES lifecycle

The engine receives `surfaceCreated`, resize, pause/resume, touch, and surface
destruction callbacks through `MainActivity` and `native-lib.cpp`.

## GitHub Actions

Push this directory as the repository root. The workflow at
`.github/workflows/android-build.yml`:

1. checks out the repository,
2. installs Java 17 and Android SDK packages,
3. installs NDK 27 and CMake 3.22,
4. builds `assembleDebug`, and
5. uploads `app-debug.apk` as `cobalt2d-debug-apk`.

No credentials are required for the debug build. Release signing should be
added with GitHub encrypted secrets rather than committed files.

## Creating a scene

Scene files use a deliberately small, readable format:

```json
{
  "name": "Main",
  "objects": [
    {"id": 1, "name": "Player", "enabled": true, "parent": 0}
  ]
}
```

In C++:

```cpp
cobalt::Scene scene{"Level1"};
auto& player = scene.createObject("Player");
player.add<cobalt::TransformComponent>().local.position = {64.0f, 96.0f};
player.add<cobalt::SpriteRendererComponent>().texture = "player.png";
player.add<cobalt::RigidBody2DComponent>();
player.add<cobalt::BoxCollider2DComponent>();
cobalt::SceneSerializer::save(scene, "Scenes/Level1.scene");
```

## Lua scripting

Attach a `LuaScript` host to a gameplay object, call `loadFile`, then call
`onStart` once and `update(deltaSeconds)` each frame. The example script is:
`Examples/Platformer/Scripts/Player.lua`.

The intended gameplay API is:

```lua
function onStart()
end

function onUpdate(dt)
  if Input.isDown("MoveRight") then
    transform.position.x = transform.position.x + 180 * dt
  end
end

function onFixedUpdate(dt)
end

function onCollision(other)
end
```

The script host turns Lua errors into a stored `lastError()` plus an engine
log entry. Game-specific bindings for transforms, input, physics, audio, and
resources should be registered in the project runtime layer.

## Known limitations

This first source release establishes the real runtime boundaries and a
buildable Android shell. A full visual editor, texture atlas importer,
material/shader asset compiler, audio decoder backend, prefab authoring UI,
and complete Lua userdata bindings are intentionally left as the next
increment rather than represented by fake implementations. The example and
core modules are usable foundations for those additions.