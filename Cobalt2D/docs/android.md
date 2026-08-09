# Android runtime

The Android app uses a Java `SurfaceView` only as the lifecycle and touch
bridge. Rendering and engine state remain in C++.

`MainActivity` forwards:

- surface creation and size changes,
- surface destruction,
- activity pause/resume,
- touch pointer IDs and coordinates.

`native-lib.cpp` creates an EGL display, chooses an OpenGL ES 3 config, owns the
render thread, and swaps the surface. The engine is paused when Android sends
the activity to the background.

Use Android's configuration changes rather than desktop assumptions. Keep all
asset paths relative to the packaged application assets or an explicit writable
save directory.