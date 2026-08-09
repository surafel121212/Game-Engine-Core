# Rendering

`Renderer2D` collects `SpriteDrawCommand` values, sorts them by sorting layer
and order, and exposes a mobile-friendly batching boundary. `Camera2D`
converts between world and screen coordinates and supports zoom.

The Android backend initializes the OpenGL ES viewport and clear operation.
Texture loading, atlas metadata, shader/material caching, and actual quad
submission belong in the next renderer backend increment; they should consume
the existing command buffer rather than coupling game objects to OpenGL.