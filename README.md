You are a AAA game engine architect with 20+ years of experiencein C++, Android NDK, OpenGL ES, and game engine development. You have built production engines used by millions.

I need a COMPLETE, SHIPPABLE, PRODUCTION-READY 2D GAME ENGINE for Android, codenamed "Future 2D Game Engine ". This is NOT a demo, NOT a prototype, NOT a placeholder. This is a REAL engine that I can build games with immediately.

---

【CORE ARCHITECTURE REQUIREMENTS】

1. Language: C++20 (with Android NDK r26+)
2. Graphics: OpenGL ES 3.0 (with fallback to 2.0)
3. Target API: Android API 24+ (Android 7.0+)
4. Build System: CMake 3.22+ with full Gradle integration
5. No dependencies on Java except NativeActivity
6. Single-threaded game loop with async asset loading on worker threads

---

【RENDERER — FULL IMPLEMENTATION】

Must implement:
- Sprite batching with dynamic batching (supports 10,000+ sprites at 60fps on mid-range devices)
- Texture atlasing with runtime packing (automatic atlas generation)
- Sprite sheet animation system (frame-based with configurable FPS, looping, ping-pong)
- 9-slice scaling for UI elements
- Custom shader pipeline (vertex/fragment shaders loaded from files)
- Particle system with: point, line, radial, gravity, vortex emitters; 20+ built-in particle behaviors; particle pooling
- Tilemap renderer with: multiple layers, tile animations, collision tiles, autotiling support
- Camera system: smooth follow, zoom with limits, shake with decay, world-to-screen conversion
- Render-to-texture for UI and post-processing effects
- Primitive drawing: lines, circles, rectangles, polygons with anti-aliasing
- Text rendering: bitmap fonts (.fnt) AND TrueType (.ttf) via Freetype with kerning and Unicode support
- Viewport and scissor support
- Render statistics overlay (FPS, draw calls, sprite count, memory usage)

---

【PHYSICS — FULL Box2D INTEGRATION】

Implement COMPLETE Box2D 2.3.2+ integration with:
- All body types: static, dynamic, kinematic
- All shapes: circle, box, polygon, edge, chain (with compound shapes)
- All joints: distance, revolute, prismatic, pulley, gear, wheel, weld, friction, motor, rope, mouse
- Collision filtering with categories and masks
- Contact callbacks with impulse and contact point data
- Raycasting with closest hit, all hits, and shape filtering
- Physics debug drawing (toggleable)
- Continuous collision detection (CCD) for fast-moving objects
- Physics body sleeping for performance
- Custom gravity per world
- Multiple physics worlds (for split-screen or UI physics)

---

【SCRIPTING — COMPLETE Lua 5.4 IMPLEMENTATION】

Full Lua 5.4.6 integration with:
- Complete engine API binding (EVERY engine function exposed)
- Lua state per scene with sandboxing
- GameObject lifecycle: onStart(), onUpdate(dt), onFixedUpdate(), onCollisionEnter/Stay/Exit(), onTriggerEnter/Stay/Exit(), onDestroy()
- Hot-reload: modify scripts while running, engine reloads automatically
- Lua debugger: breakpoints, step-into, variable inspection, stack trace
- Lua console: execute arbitrary Lua code at runtime
- Coroutine support for async logic
- Serialization: save/load Lua state to JSON
- Lua C API bindings for: math, vector2, vector3, color, rect, transform
- Built-in utility functions: lerp, clamp, random, noise, table utilities
- Script templates: pre-built scripts for player, enemy, UI button, platform, bullet, etc.

---

【EDITOR — FULL TOUCH-EDITOR RUNNING ON ANDROID】

Implement a COMPLETE visual editor that runs ON THE DEVICE:

UI Framework (custom ImGui or native):
- Scene viewport with: touch pan (two-finger), pinch zoom, object selection (tap), transform gizmo (drag handles)
- Hierarchy panel: tree view of all GameObjects, drag to reorder, parent/child relationships, search/filter
- Inspector panel: editable properties (transform, components, scripts), color picker, dropdowns, sliders, input fields
- Asset browser: grid/thumbnails view, folder navigation, drag-to-scene
- Script editor: syntax highlighting (Lua), line numbers, auto-indent, find/replace, save/load
- Toolbar: Play, Pause, Stop buttons; Frame-Advance button; Step-Into button
- Console panel: log output, error messages, warnings, filter by level
- Project settings: screen resolution, physics gravity, default scene, input mappings, render settings
- Build settings: package APK, sign with keystore, version naming
- Timeline animation editor: keyframe animation for any property (position, rotation, scale, color, etc.) with Easing curves (linear, ease-in, ease-out, bounce, elastic, etc.)

---

【AUDIO — FULL OpenSL ES IMPLEMENTATION】

- Sound effects: load WAV, OGG, MP3; play with pitch, volume, panning
- Streaming music: OGG and MP3 with seamless looping
- 3D audio: positional sound with doppler effect, attenuation curves
- Audio groups: SFX, Music, UI, Ambient with independent volume control
- Audio mixing: multiple channels with mixing
- Audio effects: low-pass filter, reverb, echo (with presets)
- Audio format support: 8-bit, 16-bit, 32-bit PCM; mono/stereo
- Audio streaming from assets and from file system
- Audio performance: object pooling for sound instances

---

【INPUT — COMPLETE IMPLEMENTATION】

- Touch: multi-touch (up to 10 fingers), gesture recognition (tap, double-tap, long press, swipe, pinch)
- Mouse: click, double-click, right-click, middle-click, scroll wheel (for editor)
- Keyboard: full key mapping with key codes, modifier keys
- Gamepad: support for Xbox, PlayStation, and generic HID controllers (via Android gamepad API)
- Input mapping: configurable mapping of touch/keys to actions
- Input events in Lua: onTouchBegan/Moved/Ended, onKeyDown/Up, onMouseMove/Click
- Input recording/playback: record input sequences for automated testing

---

【ASSET PIPELINE — COMPLETE IMPLEMENTATION】

- Asynchronous loading with progress callbacks
- Asset cache with LRU eviction policy
- Memory budgeting: per-asset and total memory limits
- Image: PNG, JPG, WEBP, TGA, BMP (via STB)
- Audio: WAV, OGG, MP3, FLAC
- Fonts: TTF, OTF, FNT (BMFont format)
- Scripts: Lua (.lua)
- Data: JSON, XML, YAML, CSV, TOML
- Models: SVG (for 2D vector graphics)
- Archiving: read/write .pkg files (zip-based)
- Asset hot-reload: detect changes and reload automatically
- Asset compression: texture compression (ETC2, ASTC)

---

【SCENE SYSTEM — COMPLETE IMPLEMENTATION】

- Scene tree with parent-child hierarchy
- Component-based GameObject architecture
- Prefabs: reusable GameObject templates (with overrides)
- Scene serialization: save/load to JSON
- Scene transitions: fade, wipe, zoom, slide (with custom duration)
- Multiple scenes: load/unload additive scenes
- Scene streaming: load large scenes incrementally
- DontDestroyOnLoad: persist GameObjects across scenes

---

【UI SYSTEM — COMPLETE IMPLEMENTATION】

- Canvas-based UI with resolution independence
- UI components: Button, Label, TextInput, Image, Panel, ScrollView, Slider, Toggle, Dropdown, ProgressBar, ListView, GridView
- Layout managers: HorizontalLayout, VerticalLayout, GridLayout, AnchorLayout
- UI styling: themes (JSON-based), skinning, state-based (normal, hover, pressed, disabled)
- UI events: onClick, onHover, onDrag, onScroll, onFocus, onTextChanged
- UI animation: tweening with multiple easing functions
- UI binding: bind UI elements to script variables (MVVM-style)

---

【BUILD SYSTEM — COMPLETE ANDROID PROJECT】

