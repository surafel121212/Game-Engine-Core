# Cobalt2D

Cobalt2D is an original, modular C++17 2D game engine source project with an
Android Studio shell, JNI/OpenGL ES runtime, Lua/Box2D-ready systems, an
example platformer, native tests, and GitHub Actions APK builds.

## Run & Operate

- `pnpm --filter @workspace/api-server run dev` — run the API server (port 5000)
- `pnpm run typecheck` — full typecheck across all packages
- `pnpm run build` — typecheck + build all packages
- `pnpm --filter @workspace/api-spec run codegen` — regenerate API hooks and Zod schemas from the OpenAPI spec
- `pnpm --filter @workspace/db run push` — push DB schema changes (dev only)
- Required env: `DATABASE_URL` — Postgres connection string

## Stack

- pnpm workspaces, Node.js 24, TypeScript 5.9
- API: Express 5
- DB: PostgreSQL + Drizzle ORM
- Validation: Zod (`zod/v4`), `drizzle-zod`
- API codegen: Orval (from OpenAPI spec)
- Build: esbuild (CJS bundle)

## Where things live

- `Cobalt2D/engine/` — engine lifecycle, math, ECS/scenes, renderer, input,
  physics, scripting, animation, tilemaps, particles, audio, and UI.
- `Cobalt2D/android/` — Android Studio project, Java `SurfaceView`, JNI bridge,
  EGL/OpenGL ES surface lifecycle, and native CMake target.
- `Cobalt2D/Examples/Platformer/` — scene, tilemap, Lua gameplay script, and
  example notes.
- `Cobalt2D/tests/` — native regression tests.
- `Cobalt2D/.github/workflows/android-build.yml` — reproducible debug APK CI.
- `Cobalt2D/README.md` and `Cobalt2D/docs/` — source project documentation.

## Architecture decisions

- The Android app keeps Java limited to lifecycle/touch bridging; engine state
  and rendering remain in C++.
- The renderer collects backend-neutral sprite commands so mobile GL batching
  can evolve without coupling gameplay to OpenGL calls.
- Scene persistence is intentionally human-readable and diffable.
- Box2D and Lua are optional CMake features for the desktop/native build; the
  engine APIs remain usable in lean Android configurations.

## Product

The project is a starting point for Android-first 2D games. Developers can
create scenes and entities, attach typed components, drive gameplay from Lua,
and build a debug APK in GitHub Actions.

## User preferences

Keep implementations honest: when a subsystem is not complete, expose the
working boundary and document the limitation instead of adding a fake facade.

## Gotchas

- The native source lives under `Cobalt2D/`; Android Studio opens
  `Cobalt2D/android/`, not the workspace root.
- The Replit environment does not provide CMake, so native checks here use
  direct C++17 syntax/test compilation. GitHub Actions installs CMake, NDK,
  SDK, and Gradle.

## Pointers

- See the `pnpm-workspace` skill for workspace structure, TypeScript setup, and package details
