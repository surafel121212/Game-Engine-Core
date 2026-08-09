cmake_minimum_required(VERSION 3.22)
project(Cobalt2D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(COBALT_BUILD_TESTS "Build native unit tests" ON)
option(COBALT_ENABLE_BOX2D "Fetch and build Box2D physics" ON)
option(COBALT_ENABLE_LUA "Fetch and build Lua scripting" ON)
option(COBALT_BUILD_ANDROID "Build the Android JNI bridge" OFF)

include(FetchContent)

if(COBALT_ENABLE_BOX2D)
  FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v2.4.1
    GIT_SHALLOW TRUE
  )
  set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
  set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(box2d)
endif()

if(COBALT_ENABLE_LUA)
  FetchContent_Declare(
    lua
    URL https://www.lua.org/ftp/lua-5.4.7.tar.gz
  )
  FetchContent_GetProperties(lua)
  if(NOT lua_POPULATED)
    FetchContent_Populate(lua)
    file(GLOB LUA_SOURCES "${lua_SOURCE_DIR}/src/*.c")
    list(REMOVE_ITEM LUA_SOURCES
      "${lua_SOURCE_DIR}/src/lua.c"
      "${lua_SOURCE_DIR}/src/luac.c"
    )
    add_library(lua STATIC ${LUA_SOURCES})
    target_include_directories(lua PUBLIC "${lua_SOURCE_DIR}/src")
    set_target_properties(lua PROPERTIES C_STANDARD 11)
  endif()
endif()

add_library(CobaltEngine STATIC
  engine/core/Engine.cpp
  engine/core/Log.cpp
  engine/core/Time.cpp
  engine/core/Profiler.cpp
  engine/ecs/Scene.cpp
  engine/input/Input.cpp
  engine/physics/PhysicsWorld.cpp
  engine/render/Renderer2D.cpp
  engine/scripting/LuaScript.cpp
  engine/scripting/ScriptRuntime.cpp
  engine/serialization/SceneSerializer.cpp
  engine/animation/Animator.cpp
  engine/audio/AudioSystem.cpp
  engine/particles/ParticleSystem.cpp
  engine/tilemap/Tilemap.cpp
  engine/assets/AssetManager.cpp
  engine/project/ProjectSettings.cpp
  engine/editor/CommandHistory.cpp
)

target_include_directories(CobaltEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(CobaltEngine PUBLIC
  $<$<BOOL:${COBALT_ENABLE_BOX2D}>:COBALT_HAS_BOX2D=1>
  $<$<BOOL:${COBALT_ENABLE_LUA}>:COBALT_HAS_LUA=1>
)

if(COBALT_ENABLE_BOX2D)
  target_link_libraries(CobaltEngine PUBLIC box2d)
endif()
if(COBALT_ENABLE_LUA)
  target_link_libraries(CobaltEngine PUBLIC lua)
endif()

if(ANDROID OR COBALT_BUILD_ANDROID)
  add_library(cobalt_android SHARED android/app/src/main/cpp/native-lib.cpp)
  target_link_libraries(cobalt_android PRIVATE CobaltEngine android log EGL GLESv3)
  target_compile_definitions(CobaltEngine PUBLIC COBALT_PLATFORM_ANDROID=1)
endif()

if(COBALT_BUILD_TESTS)
  enable_testing()
  foreach(test_name IN ITEMS test_math test_scene test_runtime test_upgrade)
    add_executable(cobalt_${test_name} tests/${test_name}.cpp)
    target_link_libraries(cobalt_${test_name} PRIVATE CobaltEngine)
    add_test(NAME cobalt_${test_name} COMMAND cobalt_${test_name})
  endforeach()
endif()