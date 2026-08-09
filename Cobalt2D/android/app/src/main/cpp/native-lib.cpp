#include "engine/core/Engine.hpp"
#include "engine/core/Log.hpp"

#include <android/native_window_jni.h>
#include <jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <memory>
#include <mutex>
#include <thread>

namespace {
std::mutex mutex;
std::unique_ptr<cobalt::Engine> engine;
EGLDisplay display = EGL_NO_DISPLAY;
EGLSurface surface = EGL_NO_SURFACE;
EGLContext context = EGL_NO_CONTEXT;
ANativeWindow* window = nullptr;
bool running = false;
std::thread renderThread;

bool createContext(ANativeWindow* newWindow) {
  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (display == EGL_NO_DISPLAY || !eglInitialize(display, nullptr, nullptr)) return false;
  const EGLint attributes[] = {
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT, EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_NONE};
  EGLConfig config;
  EGLint count = 0;
  if (!eglChooseConfig(display, attributes, &config, 1, &count) || count == 0) return false;
  const EGLint contextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
  context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);
  surface = eglCreateWindowSurface(display, config, newWindow, nullptr);
  return context != EGL_NO_CONTEXT && surface != EGL_NO_SURFACE &&
         eglMakeCurrent(display, surface, surface, context);
}

void destroyContext() {
  if (display != EGL_NO_DISPLAY) {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
    if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
    eglTerminate(display);
  }
  display = EGL_NO_DISPLAY;
  surface = EGL_NO_SURFACE;
  context = EGL_NO_CONTEXT;
}

void renderLoop() {
  while (running) {
    std::lock_guard lock(mutex);
    if (engine) {
      engine->update();
      engine->render();
    }
    if (display != EGL_NO_DISPLAY) eglSwapBuffers(display, surface);
  }
}
}  // namespace

extern "C" JNIEXPORT void JNICALL
Java_dev_cobalt_engine_MainActivity_nativeSurfaceCreated(JNIEnv* env, jclass, jobject javaSurface) {
  std::lock_guard lock(mutex);
  window = ANativeWindow_fromSurface(env, javaSurface);
  if (!engine) engine = std::make_unique<cobalt::Engine>();
  engine->initialize();
  createContext(window);
  running = true;
  renderThread = std::thread(renderLoop);
}

extern "C" JNIEXPORT void JNICALL
Java_dev_cobalt_engine_MainActivity_nativeSurfaceChanged(JNIEnv*, jclass, jint width, jint height) {
  std::lock_guard lock(mutex);
  if (engine) engine->onSurfaceCreated(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_dev_cobalt_engine_MainActivity_nativeSurfaceDestroyed(JNIEnv*, jclass) {
  {
    std::lock_guard lock(mutex);
    running = false;
  }
  if (renderThread.joinable()) renderThread.join();
  std::lock_guard lock(mutex);
  if (engine) engine->onSurfaceDestroyed();
  destroyContext();
  if (window) ANativeWindow_release(window);
  window = nullptr;
}

extern "C" JNIEXPORT void JNICALL
Java_dev_cobalt_engine_MainActivity_nativePause(JNIEnv*, jclass) {
  std::lock_guard lock(mutex);
  if (engine) engine->pause();
}

extern "C" JNIEXPORT void JNICALL
Java_dev_cobalt_engine_MainActivity_nativeResume(JNIEnv*, jclass) {
  std::lock_guard lock(mutex);
  if (engine) engine->resume();
}

extern "C" JNIEXPORT void JNICALL
Java_dev_cobalt_engine_MainActivity_nativeTouch(JNIEnv*, jclass, jint pointerId,
                                                 jfloat x, jfloat y, jboolean down) {
  std::lock_guard lock(mutex);
  if (engine) engine->input().setTouch(pointerId, {x, y}, down);
}