#include "Renderer.h"

#include <jni.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <android/native_window.h>
#include "../../LogHelper.h"

#include "../Shader.h"
#include "../drawable/Triangle.h"

#include <string>
#include <pthread.h>

#include "../../native-lib.h"

Renderer::Renderer() :
        mThreadId(0),
        mMutex(),
        mMsg(MSG_NONE),
        mWindow(nullptr),
        mDisplay(EGL_NO_DISPLAY),
        mSurface(EGL_NO_SURFACE),
        mContext(EGL_NO_CONTEXT)
{
    LOGI(TAG, "Renderer instance created");
    pthread_mutex_init(&mMutex, nullptr);
}

Renderer::~Renderer()
{
    LOGI(TAG, "Renderer instance destroyed");
    pthread_mutex_destroy(&mMutex);
}

void Renderer::start()
{
    LOGI(TAG, "Creating renderer thread");
    pthread_create(&mThreadId, nullptr, threadStartCallback, this);
}

void Renderer::stop()
{
    LOGI(TAG, "Stopping renderer thread");
    // Посылаем потоку рендеринга стоп-сигнал
    pthread_mutex_lock(&mMutex);
    mMsg = MSG_RENDER_LOOP_EXIT;
    pthread_mutex_unlock(&mMutex);

    pthread_join(mThreadId, nullptr);
    LOGI(TAG, "Renderer thread stopped");
}


void Renderer::setWindow(ANativeWindow *window)
{
    // Сообщаем потоку рендеринга о смене окна
    pthread_mutex_lock(&mMutex);
    mMsg = MSG_WINDOW_SET;
    mWindow = window;
    pthread_mutex_unlock(&mMutex);
}

void Renderer::renderLoop()
{
    bool renderingEnabled = true;

    LOGI(TAG, "Render loop entry");

    while (renderingEnabled)
    {
        pthread_mutex_lock(&mMutex);

        // Обработка входящих сообщений
        switch (mMsg)
        {
            case MSG_WINDOW_SET:
                unloadStuff();
                initialize();
                loadStuff();
                break;
            case MSG_RENDER_LOOP_EXIT:
                renderingEnabled = false;
                destroy();
                break;
            default:
                break;
        }
        mMsg = MSG_NONE;

        // Отрисовка сцены
        if (mDisplay)
        {
            drawFrame();
            if (!eglSwapBuffers(mDisplay, mSurface))
                LOGE(TAG, "eglSwapBuffers() returned error %d", eglGetError());
        }

        pthread_mutex_unlock(&mMutex);
    }

    LOGI(TAG, "Render loop exits");
}

bool Renderer::initialize()
{
    // Освобождение ресурсов, если метод вызывается после смены окна
    if (mDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (mContext != EGL_NO_CONTEXT)
            eglDestroyContext(mDisplay, mContext);
        if (mSurface != EGL_NO_SURFACE)
            eglDestroySurface(mDisplay, mSurface);
        eglTerminate(mDisplay);

        mDisplay = EGL_NO_DISPLAY;
        mSurface = EGL_NO_SURFACE;
        mContext = EGL_NO_CONTEXT;
    }

    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    const EGLint attribList[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
    EGLDisplay display;
    EGLConfig config;
    EGLint numConfigs;
    EGLint format;
    EGLSurface surface;
    EGLContext context;
    EGLint width;
    EGLint height;
    GLfloat ratio;

    LOGI(TAG, "Initializing context");

    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY)
    {
        LOGE(TAG, "eglGetDisplay() returned error %d", eglGetError());
        return false;
    }
    if (!eglInitialize(display, nullptr, nullptr))
    {
        LOGE(TAG, "eglInitialize() returned error %d", eglGetError());
        return false;
    }

    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs))
    {
        LOGE(TAG, "eglChooseConfig() returned error %d", eglGetError());
        destroy();
        return false;
    }

    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format))
    {
        LOGE(TAG, "eglGetConfigAttrib() returned error %d", eglGetError());
        destroy();
        return false;
    }

    ANativeWindow_setBuffersGeometry(mWindow, 0, 0, format);

    if (!(surface = eglCreateWindowSurface(display, config, mWindow, nullptr)))
    {
        LOGE(TAG, "eglCreateWindowSurface() returned error %d", eglGetError());
        destroy();
        return false;
    }

    if (!(context = eglCreateContext(display, config, nullptr, attribList)))
    {
        LOGE(TAG, "eglCreateContext() returned error %d", eglGetError());
        destroy();
        return false;
    }

    if (!eglMakeCurrent(display, surface, surface, context))
    {
        LOGE(TAG, "eglMakeCurrent() returned error %d", eglGetError());
        destroy();
        return false;
    }

    if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(display, surface, EGL_HEIGHT, &height))
    {
        LOGE(TAG, "eglQuerySurface() returned error %d", eglGetError());
        destroy();
        return false;
    }

    mDisplay = display;
    mSurface = surface;
    mContext = context;

    glDisable(GL_DITHER);
    glClearColor(0.2f,0.2f,0.2f,1.f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);

    ratio = (GLfloat) width / height;

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    LOGI(TAG, "Current OpenGL context version is %d.%d", major, minor);

    return true;
}

void Renderer::destroy()
{
    LOGI(TAG, "Destroying context");

    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(mDisplay, mContext);
    eglDestroySurface(mDisplay, mSurface);
    eglTerminate(mDisplay);

    mDisplay = EGL_NO_DISPLAY;
    mSurface = EGL_NO_SURFACE;
    mContext = EGL_NO_CONTEXT;
}

void Renderer::drawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
}

void *Renderer::threadStartCallback(void *myself)
{
    auto *renderer = (Renderer *)myself;

    renderer->renderLoop();
    pthread_exit(nullptr);
}