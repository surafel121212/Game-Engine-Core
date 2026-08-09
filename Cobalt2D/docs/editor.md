# Editor upgrade foundations

The upgrade adds the data and command foundations needed for a responsive
editor without faking UI actions:

- `CommandHistory` provides execute/undo/redo with truncation after a new edit.
- `AssetManager` performs actual filesystem operations and metadata generation.
- `ProjectSettings` persists orientation, resolution, performance, Android,
  and build-related settings.
- `Animator` stores editable tracks, keyframes, events, and state transitions.
- `Profiler` exposes frame time, CPU time, draw calls, entity counts, physics
  body counts, and Lua timing fields.
- `MobileControls` provides reusable touch buttons and virtual joystick logic.

A future editor frontend should bind its Scene View, Inspector, Asset Browser,
Timeline, Console, and Build Settings screens to these APIs. The remaining
work is presentation and platform integration, not a fake collection of
buttons.