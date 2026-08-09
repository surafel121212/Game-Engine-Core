cmake_minimum_required(VERSION 3.22.1)
project(cobalt_android LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COBALT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../..")

add_library(cobalt_android SHARED
    native-lib.cpp
    "${COBALT_ROOT}/engine/core/Engine.cpp"
    "${COBALT_ROOT}/engine/core/Log.cpp"
    "${COBALT_ROOT}/engine/core/Time.cpp"
    "${COBALT_ROOT}/engine/ecs/Scene.cpp"
    "${COBALT_ROOT}/engine/input/Input.cpp"
    "${COBALT_ROOT}/engine/physics/PhysicsWorld.cpp"
    "${COBALT_ROOT}/engine/render/Renderer2D.cpp"
    "${COBALT_ROOT}/engine/scripting/LuaScript.cpp"
    "${COBALT_ROOT}/engine/serialization/SceneSerializer.cpp"
)

target_include_directories(cobalt_android PRIVATE "${COBALT_ROOT}")
target_compile_definitions(cobalt_android PRIVATE COBALT_PLATFORM_ANDROID=1)
target_link_libraries(cobalt_android PRIVATE android log EGL GLESv3)