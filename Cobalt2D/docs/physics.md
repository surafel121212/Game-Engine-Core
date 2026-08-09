# Physics

`PhysicsWorld` is the engine boundary for gravity, stepping, and collision
callbacks. Desktop builds can enable Box2D with CMake:

```bash
cmake -S . -B build -DCOBALT_ENABLE_BOX2D=ON
```

Use the fixed timestep for gameplay. Store physics state in components and
apply gameplay forces during the fixed update, not inside rendering.