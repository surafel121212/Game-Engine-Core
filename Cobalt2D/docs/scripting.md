# Lua scripting

Lua scripts are loaded by `LuaScript::loadFile`. The host invokes the optional
callbacks `onCreate`, `onAwake`, `onStart`, `onEnable`, `onDisable`, `onUpdate`,
`onFixedUpdate`, `onLateUpdate`, and `onDestroy`. Collision and trigger
callbacks are available through `ScriptRuntime`. Script errors are returned as
`false`, stored in `lastError()`, added to structured diagnostics with file and
line information when available, and logged.

## Exposed Inspector properties

Place `---@export` immediately above a simple Lua assignment:

```lua
---@export
speed = 250

---@export
isPlayer = true
```

`LuaScript::inspectSource` reports the property name and inferred type.
`setProperty` updates the property in the live Lua state when Lua is enabled.
The current runtime supports integer, float, boolean, and string inference;
Vector2, Vector3, color, enum, and object-reference values are reserved for
the editor binding layer.

`LuaScript::reload` closes and rebuilds the script state from its source path.
Breakpoints are tracked by line in the script object so an editor debugger can
attach pause/step behavior without changing the engine API.

Keep gameplay scripts deterministic: use `onFixedUpdate` for physics decisions,
avoid creating tables in tight loops, and expose only project-approved native
bindings to the Lua state.

`Examples/Platformer/Scripts/Player.lua` shows the intended movement and jump
shape.