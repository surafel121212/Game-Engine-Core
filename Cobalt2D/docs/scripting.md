# Lua scripting

Lua scripts are loaded by `LuaScript::loadFile`. The host invokes the optional
callbacks `onStart`, `onUpdate`, and future fixed-step callbacks. Script errors
are returned as `false`, stored in `lastError()`, and logged.

Keep gameplay scripts deterministic: use `onFixedUpdate` for physics decisions,
avoid creating tables in tight loops, and expose only project-approved native
bindings to the Lua state.

`Examples/Platformer/Scripts/Player.lua` shows the intended movement and jump
shape.