#ifndef ILLEGAL_RACER_RENDERER_H
#define ILLEGAL_RACER_RENDERER_H

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

    /**     setRenderLoop вызывается при старте потока рендеринга в методе start().
     *      Он создаёт контекст рендеринга и отрисовывает сцену, пока не будет
     *   вызван метод stop().
     */
    void renderLoop();

    bool initialize();
    void destroy();

    void drawFrame();

    // Вспомогательный метод для запуска потока рендеринга
    static void* threadStartCallback(void *myself);

protected:
    /**
     * Метод, посредством которого выполняется освобождение памяти из-под компонентов, требующих
     * контекст OpenGL, таких как: шейдеры, примитивы, которые содержат VAO и VBO, и т.д.
     *  <p>Автоматически вызывается перед парной функцией <code>loadStuff()</code>. После
     * освобождения памяти <b>настоятельно рекомендуется присваивать ссылкам значение
     * <code>nullptr</code></b>
     *  <p>Может быть вызван в деструкторе класса рендера.
     */
    virtual void unloadStuff() = 0;
    /**
     * Метод, посредством которого выполняется инициализация каких-либо компонентов, требующих
     * контекст OpenGL, таких как: шейдеры, примитивы, которые содержат VAO и VBO, и т.д.
     * <p>Автоматически вызывается после создания OpenGL ES контекста.
     */
    virtual void loadStuff() = 0;
    /**
     * Метод, посредством которого выполняется рендеринг сцены
     */
    virtual void drawScene() = 0;
};

#endif
