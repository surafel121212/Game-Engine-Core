#!/bin/bash
# 2D Game Engine - Full Professional Android Project
# Generate complete project structure

PROJECT_NAME="PixelEngine2D"
NDK_VERSION="26.3.11579264"
MIN_SDK="24"
TARGET_SDK="34"

mkdir -p $PROJECT_NAME
cd $PROJECT_NAME

# ============================================
# 1. Android Project Structure
# ============================================
mkdir -p app/src/main/{java/com/pixelengine,res,assets}
mkdir -p app/src/main/cpp/{core,renderer,physics,input,audio,scene,scripting}
mkdir -p app/src/main/cpp/third_party/{box2d,glm,stb}

# ============================================
# 2. build.gradle (App Level)
# ============================================
cat > app/build.gradle << 'EOF'
plugins {
    id 'com.android.application'
}

android {
    namespace 'com.pixelengine'
    compileSdk 34

    defaultConfig {
        applicationId "com.pixelengine"
        minSdk 24
        targetSdk 34
        versionCode 1
        versionName "1.0"
        externalNativeBuild {
            cmake {
                cppFlags "-std=c++17 -O2 -DANDROID"
                arguments "-DANDROID_STL=c++_shared"
            }
        }
    }

    externalNativeBuild {
        cmake {
            path "src/main/cpp/CMakeLists.txt"
        }
    }

    buildTypes {
        release {
            minifyEnabled true
            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt')
        }
    }
}

dependencies {
    implementation 'androidx.appcompat:appcompat:1.6.1'
    implementation 'androidx.core:core:1.12.0'
}
EOF

# ============================================
# 3. CMakeLists.txt
# ============================================
cat > app/src/main/cpp/CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.22)
project(PixelEngine2D)

set(CMAKE_CXX_STANDARD 17)

# Third-party: Box2D (physics)
add_subdirectory(third_party/box2d)

# Core engine
add_library(pixelengine SHARED
    core/Engine.cpp
    core/GameLoop.cpp
    renderer/Renderer.cpp
    renderer/Sprite.cpp
    renderer/Camera.cpp
    physics/PhysicsWorld.cpp
    physics/Collider.cpp
    input/InputManager.cpp
    audio/AudioSystem.cpp
    scene/SceneManager.cpp
    scene/GameObject.cpp
    scripting/ScriptEngine.cpp
    main.cpp
)

target_include_directories(pixelengine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb
)

target_link_libraries(pixelengine
    box2d
    GLESv3
    EGL
    OpenSLES
    android
    log
    z
)

# Copy assets
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../assets DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
EOF

# ============================================
# 4. Core Engine - Engine.h
# ============================================
cat > app/src/main/cpp/core/Engine.h << 'EOF'
#pragma once
#include <memory>
#include <chrono>

namespace PixelEngine {

class Renderer;
class PhysicsWorld;
class InputManager;
class AudioSystem;
class SceneManager;

class Engine {
public:
    static Engine& GetInstance();
    
    bool Initialize(int width, int height);
    void Shutdown();
    void Run();
    
    Renderer* GetRenderer() { return m_Renderer.get(); }
    PhysicsWorld* GetPhysics() { return m_Physics.get(); }
    InputManager* GetInput() { return m_Input.get(); }
    AudioSystem* GetAudio() { return m_Audio.get(); }
    SceneManager* GetScene() { return m_Scene.get(); }
    
private:
    Engine() = default;
    ~Engine() = default;
    
    void GameLoop();
    void Update(float deltaTime);
    void Render();
    void HandleInput();
    
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<PhysicsWorld> m_Physics;
    std::unique_ptr<InputManager> m_Input;
    std::unique_ptr<AudioSystem> m_Audio;
    std::unique_ptr<SceneManager> m_Scene;
    
    bool m_Running = false;
    std::chrono::steady_clock::time_point m_LastFrame;
};

} // namespace PixelEngine
EOF

# ============================================
# 5. Renderer - Renderer.h (OpenGL ES 3.0)
# ============================================
cat > app/src/main/cpp/renderer/Renderer.h << 'EOF'
#pragma once
#include <vector>
#include <string>
#include <GLES3/gl3.h>

namespace PixelEngine {

struct Sprite {
    GLuint textureId;
    float x, y, width, height;
    float rotation;
    float r, g, b, a;
    int layer;
};

class Renderer {
public:
    bool Initialize(int width, int height);
    void Shutdown();
    void Clear(float r, float g, float b, float a);
    void DrawSprite(const Sprite& sprite);
    void Present();
    void SetViewport(int x, int y, int width, int height);
    
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    
private:
    GLuint CreateTexture(const std::string& path);
    void SetupShaders();
    void SetupBuffers();
    
    GLuint m_Program = 0;
    GLuint m_VAO = 0, m_VBO = 0, m_EBO = 0;
    int m_Width = 0, m_Height = 0;
};

} // namespace PixelEngine
EOF

# ============================================
# 6. Physics - PhysicsWorld.h (Box2D)
# ============================================
cat > app/src/main/cpp/physics/PhysicsWorld.h << 'EOF'
#pragma once
#include <box2d/box2d.h>
#include <memory>
#include <vector>

namespace PixelEngine {

struct CollisionInfo {
    void* objectA;
    void* objectB;
    float impulse;
};

class PhysicsWorld {
public:
    bool Initialize(const b2Vec2& gravity = b2Vec2(0.0f, -9.8f));
    void Shutdown();
    void Update(float deltaTime);
    void Step(float deltaTime, int velocityIterations = 8, int positionIterations = 3);
    
    b2Body* CreateBody(const b2BodyDef& def);
    void DestroyBody(b2Body* body);
    b2Body* GetBody(void* userData);
    
    void SetGravity(const b2Vec2& gravity);
    b2Vec2 GetGravity() const;
    
    void SetContactListener(b2ContactListener* listener);
    
    std::vector<CollisionInfo> GetCollisions() const;
    
private:
    std::unique_ptr<b2World> m_World;
    std::vector<CollisionInfo> m_Collisions;
};

} // namespace PixelEngine
EOF

# ============================================
# 7. Scene Manager - SceneManager.h
# ============================================
cat > app/src/main/cpp/scene/SceneManager.h << 'EOF'
#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace PixelEngine {

class GameObject;
class Scene;

class SceneManager {
public:
    void Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void Render();
    
    void LoadScene(const std::string& name);
    void ReloadCurrentScene();
    void AddGameObject(std::shared_ptr<GameObject> obj);
    void RemoveGameObject(std::shared_ptr<GameObject> obj);
    
    Scene* GetCurrentScene() { return m_CurrentScene.get(); }
    
private:
    std::unique_ptr<Scene> m_CurrentScene;
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_Scenes;
};

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
    virtual void OnCollision(GameObject* other) {}
    
    float x = 0, y = 0;
    bool active = true;
};

class Scene {
public:
    void AddObject(std::shared_ptr<GameObject> obj);
    void RemoveObject(std::shared_ptr<GameObject> obj);
    void Update(float deltaTime);
    void Render();
    std::vector<std::shared_ptr<GameObject>>& GetObjects() { return m_Objects; }
    
private:
    std::vector<std::shared_ptr<GameObject>> m_Objects;
};

} // namespace PixelEngine
EOF

# ============================================
# 8. Input Manager - InputManager.h
# ============================================
cat > app/src/main/cpp/input/InputManager.h << 'EOF'
#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace PixelEngine {

class InputManager {
public:
    void Update();
    
    bool IsKeyDown(int key) const;
    bool IsKeyPressed(int key) const;
    bool IsKeyReleased(int key) const;
    
    bool IsTouchDown(int id = 0) const;
    bool IsTouchMoved(int id = 0) const;
    bool IsTouchUp(int id = 0) const;
    glm::vec2 GetTouchPosition(int id = 0) const;
    
    void SetKeyState(int key, bool down);
    void SetTouchState(int id, bool down, float x, float y);
    void SetTouchMoved(int id, float x, float y);
    
private:
    std::unordered_map<int, bool> m_CurrentKeys;
    std::unordered_map<int, bool> m_PreviousKeys;
    
    struct Touch {
        bool down = false;
        bool moved = false;
        glm::vec2 position;
    };
    std::unordered_map<int, Touch> m_Touches;
};

} // namespace PixelEngine
EOF

# ============================================
# 9. Audio System - AudioSystem.h (OpenSL ES)
# ============================================
cat > app/src/main/cpp/audio/AudioSystem.h << 'EOF'
#pragma once
#include <vector>
#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

namespace PixelEngine {

struct Sound {
    std::string name;
    std::vector<short> data;
    int sampleRate = 44100;
    int channels = 2;
};

class AudioSystem {
public:
    bool Initialize();
    void Shutdown();
    void PlaySound(const std::string& name, float volume = 1.0f);
    void PlayMusic(const std::string& name, bool loop = true);
    void StopMusic();
    void SetMasterVolume(float volume);
    
    bool LoadSound(const std::string& name, const std::string& path);
    bool LoadMusic(const std::string& name, const std::string& path);
    
private:
    SLObjectItf m_EngineObj = nullptr;
    SLEngineItf m_Engine = nullptr;
    SLObjectItf m_OutputMixObj = nullptr;
    std::vector<SLObjectItf> m_Players;
    float m_MasterVolume = 1.0f;
};

} // namespace PixelEngine
EOF

# ============================================
# 10. Main - main.cpp (Android Native Activity)
# ============================================
cat > app/src/main/cpp/main.cpp << 'EOF'
#include <android_native_app_glue.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "core/Engine.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "PixelEngine", __VA_ARGS__)

using namespace PixelEngine;

static android_app* g_App = nullptr;
static bool g_Initialized = false;

void HandleCmd(android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window && !g_Initialized) {
                int width = ANativeWindow_getWidth(app->window);
                int height = ANativeWindow_getHeight(app->window);
                
                if (Engine::GetInstance().Initialize(width, height)) {
                    g_Initialized = true;
                    Engine::GetInstance().Run();
                }
            }
            break;
        case APP_CMD_TERM_WINDOW:
            if (g_Initialized) {
                Engine::GetInstance().Shutdown();
                g_Initialized = false;
            }
            break;
        case APP_CMD_DESTROY:
            if (g_Initialized) {
                Engine::GetInstance().Shutdown();
                g_Initialized = false;
            }
            break;
    }
}

int32_t HandleInput(android_app* app, AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int action = AMotionEvent_getAction(event);
        int actionMasked = action & AMOTION_EVENT_ACTION_MASK;
        int id = AMotionEvent_getPointerId(event, 0);
        float x = AMotionEvent_getX(event, 0);
        float y = AMotionEvent_getY(event, 0);
        
        auto* input = Engine::GetInstance().GetInput();
        if (!input) return 0;
        
        switch (actionMasked) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                input->SetTouchState(id, true, x, y);
                break;
            case AMOTION_EVENT_ACTION_MOVE:
                input->SetTouchMoved(id, x, y);
                break;
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
                input->SetTouchState(id, false, x, y);
                break;
        }
        return 1;
    }
    return 0;
}

void android_main(struct android_app* app) {
    g_App = app;
    app->onAppCmd = HandleCmd;
    app->onInputEvent = HandleInput;
    
    while (true) {
        int events;
        struct android_poll_source* source;
        while (ALooper_pollAll(0, nullptr, &events, (void**)&source) >= 0) {
            if (source) source->process(app, source);
            if (app->destroyRequested) return;
        }
        
        if (g_Initialized) {
            Engine::GetInstance().GetInput()->Update();
        }
    }
}
EOF

# ============================================
# 11. Java Activity - MainActivity.java
# ============================================
cat > app/src/main/java/com/pixelengine/MainActivity.java << 'EOF'
package com.pixelengine;

import android.app.NativeActivity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

public class MainActivity extends NativeActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        super.onCreate(savedInstanceState);
    }
}
EOF

# ============================================
# 12. AndroidManifest.xml
# ============================================
cat > app/src/main/AndroidManifest.xml << 'EOF'
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.pixelengine">

    <uses-feature
        android:glEsVersion="0x00030000"
        android:required="true" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />

    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="Pixel Engine 2D"
        android:theme="@style/Theme.AppCompat.NoActionBar"
        android:hardwareAccelerated="true">

        <activity android:name=".MainActivity"
            android:configChanges="orientation|keyboardHidden|screenSize"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
EOF

# ============================================
# 13. gradle.properties
# ============================================
cat > gradle.properties << 'EOF'
org.gradle.jvmargs=-Xmx2048m
android.useAndroidX=true
android.enableJetifier=true
android.buildToolsVersion=34.0.0
EOF

# ============================================
# 14. settings.gradle
# ============================================
cat > settings.gradle << 'EOF'
pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositories {
        google()
        mavenCentral()
    }
}
rootProject.name = "PixelEngine2D"
include ':app'
EOF

# ============================================
# 15. Box2D integration (minimal stub)
# ============================================
cat > app/src/main/cpp/third_party/box2d/CMakeLists.txt << 'EOF'
# Box2D stub - replace with actual Box2D source
add_library(box2d STATIC)
target_sources(box2d PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2BroadPhase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2CollideCircle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2CollideEdge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2CollidePolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2Collision.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2Distance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2DynamicTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collision/b2TimeOfImpact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/b2BlockAllocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/b2Draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/b2Math.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/b2Settings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/b2StackAllocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/b2Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/b2Body.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/b2ContactManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/b2Fixture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/b2Island.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/b2World.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/b2WorldCallbacks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2ChainAndCircleContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2ChainAndPolygonContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2CircleContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2Contact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2ContactSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2EdgeAndCircleContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2EdgeAndPolygonContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2PolygonAndCircleContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/contacts/b2PolygonContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2DistanceJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2FrictionJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2GearJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2Joint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2MotorJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2MouseJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2PrismaticJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2PulleyJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2RevoluteJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2RopeJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2WeldJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamics/joints/b2WheelJoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rope/b2Rope.cpp
)
target_include_directories(box2d PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
EOF

# ============================================
# 16. Build script
# ============================================
cat > build.sh << 'EOF'
#!/bin/bash
export ANDROID_NDK_HOME=/opt/android-ndk
./gradlew assembleDebug
EOF

chmod +x build.sh

echo "✅ 2D Game Engine project generated successfully!"
echo "📁 Location: ./$PROJECT_NAME"
echo ""
echo "Next steps:"
echo "1. cd $PROJECT_NAME"
echo "2. Place Box2D source in app/src/main/cpp/third_party/box2d/src/"
echo "3. Place GLM in app/src/main/cpp/third_party/glm/"
echo "4. Place STB in app/src/main/cpp/third_party/stb/"
echo "5. ./build.sh"
