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
- Professional upgrade foundations:
  - Lua lifecycle dispatch, safe reloads, exported properties, diagnostics,
    breakpoints, and per-entity script runtime attachment.
  - Filesystem-backed asset scanning/import, metadata sidecars, favorites,
    search, type filtering, duplicate/move/delete, and import settings.
  - Timeline keyframes, interpolation curves, animation events, and
    condition-based animation state transitions.
  - Project/build settings, categorized logging, frame profiling, command-based
    undo/redo, virtual joystick, and reusable touch buttons.

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
  assets/           asset discovery, metadata, search, and filesystem actions
  project/          project and Android build settings
  editor/           command history for undo/redo
  serialization/    scene persistence
  animation/        clips, timelines, curves, events, state machines
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

The script host turns Lua errors into a stored `lastError()` plus a structured
diagnostic and engine log entry. `ScriptRuntime` owns per-entity attachments
and dispatches the lifecycle and collision callbacks.

Scripts can expose Inspector values with `---@export`:

```lua
---@export
speed = 250

---@export
playerName = "Player"
```

`LuaScript::inspectSource` reports inferred property metadata, while
`setProperty` updates a live Lua global when Lua is enabled. The current
runtime supports integer, float, boolean, and string inference; Vector2,
Vector3, color, enum, and object-reference values are reserved for the editor
binding layer.

`LuaScript::reload` rebuilds a script state from its source path. Breakpoints
are tracked by source line so an editor debugger can add pause/step behavior
without changing the scripting API.

## Assets and project settings

`AssetManager` scans `Assets/` recursively, classifies known file types,
creates `.meta` sidecars, and supports search, type filters, favorites,
import, rename, duplicate, move, delete, and refresh. `Project.cobalt` stores
project identity, resolution, orientation, performance, Android package ID,
SDK targets, and architecture settings.

## Animation and editor foundations

`Animator` supports the original frame clips plus editable timeline tracks,
keyframes, interpolation modes, event callbacks, and an
`AnimationStateMachine`. `CommandHistory` provides execute/undo/redo for
scene, tilemap, animation, and asset editor commands.

## Known limitations

This upgrade establishes the real runtime and editor-data boundaries while
keeping the Android shell buildable. A full visual editor, texture atlas
thumbnail generation, production shader/material pipeline, audio decoder
backend, prefab authoring UI, complete Lua userdata bindings, and an in-app
script editor remain future work rather than being represented by fake
implementations. The example and upgraded modules are usable foundations for
those additions.