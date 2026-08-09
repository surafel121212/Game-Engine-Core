# Architecture

Cobalt2D separates platform concerns from game code:

```text
Android Activity / SurfaceView
          |
       JNI + EGL
          |
   Engine lifecycle
   /      |       \
 Input  Physics  Renderer2D
   |      |       |
 Scene + components + scripts
          |
      serialization
```

`Engine` owns the lifetime of time, input, renderer, physics, and the active
scene. Systems expose narrow C++ interfaces so desktop tests and Android
runtime code can share gameplay logic.

## Ownership rules

- `Engine` owns the active `Scene`.
- `Scene` owns `Entity` instances.
- `Entity` owns typed components through RAII.
- `Renderer2D` owns the frame command buffer only; asset managers should own
  long-lived textures and decoded resources.
- `LuaScript` owns one Lua state and closes it in its destructor.
- Platform code owns EGL and native window handles.

## Frame order

1. `Time::beginFrame`
2. clear transient input edges
3. run fixed physics steps while the accumulator permits
4. submit render commands
5. sort commands by layer/order
6. swap the Android EGL surface
7. `Time::endFrame`