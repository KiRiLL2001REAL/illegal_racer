#ifndef ILLEGAL_RACER_RENDERER_H
#define ILLEGAL_RACER_RENDERER_H

#include "../Shader.h"
#include "../drawable/Triangle.h"

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <string>
#include <pthread.h>

class Renderer {
public:
    constexpr static const char *const TAG = "Renderer";

    Renderer();
    virtual ~Renderer();

    void start();
    void stop();
    void setWindow(ANativeWindow* window);

    void setDrawFunction(void (*function)());

private:
    enum RenderThreadMessage {
        MSG_NONE = 0,
        MSG_WINDOW_SET,
        MSG_RENDER_LOOP_EXIT
    };

    pthread_t mThreadId;
    pthread_mutex_t mMutex;
    enum RenderThreadMessage mMsg;

    // Андройдовское окно
    ANativeWindow* mWindow;

    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;

    // Подменяемая функция рисования
    void (*drawStuff)();

    //   RenderLoop вызывается при старте потока рендеринга в методе start(). Он создаёт
    // контекст рендеринга и отрисовывает сцену, пока не будет вызван метод stop().
    void renderLoop();

    bool initialize();
    void destroy();

    void drawFrame();

    // Вспомогательный метод для запуска потока рендеринга
    static void* threadStartCallback(void *myself);

};

#endif
