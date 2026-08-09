# Assets

`AssetManager` is the filesystem-backed import/index boundary. Call
`scan("Assets")` at project open and `refresh()` after filesystem changes.

Supported classifications include textures, sprite sheets, audio, music,
fonts, Lua scripts, scenes, prefabs, tilemaps, tilesets, animations,
materials, shaders, particles, and UI assets. Each known file receives a
human-readable `.meta` sidecar with its ID, type, size, favorite flag, and
basic import settings.

The manager supports real operations:

- search by path and filter by `AssetType`,
- favorite/unfavorite,
- import a file into the asset root,
- rename, duplicate, move, and delete,
- refresh the recursive index.

Texture and audio settings are represented in `AssetMetadata` and are ready
for importer-specific persistence as the decoder/rendering backends mature.