#include "native-lib.h"

#include "LogHelper.h"
#include <exception>

#include <android/native_window.h>
#include <android/native_window_jni.h>

const char* TAG = "NATIVE-LIB";

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnStart(
        JNIEnv *env,
        jobject thiz)
{
    aTouches = new touch::Touches();
    aTouches->setHandlerFrequency(130);
    aRenderer = new MyRenderer(env, thiz);
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnResume(
        JNIEnv *env,
        jobject thiz)
{
    aTouches->start();
    aRenderer->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnPause(
        JNIEnv *env,
        jobject thiz)
{
    aTouches->stop();
    aRenderer->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnStop(
        JNIEnv *env,
        jobject thiz)
{
    delete aTouches;
    aTouches = nullptr;
    delete aRenderer;
    aRenderer = nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeSetSurface(
        JNIEnv *env,
        jobject thiz,
        jobject surface)
{
    if (surface != nullptr)
    {
        aWindow = ANativeWindow_fromSurface(env, surface);
        LOGI(TAG, "Got window %p", aWindow);
        aRenderer->setWindow(aWindow);
    }
    else
    {
        LOGI(TAG, "Releasing window");
        ANativeWindow_release(aWindow);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_CustomSurfaceView_registerActionDown(
        JNIEnv *env,
        jobject thiz,
        jint id,
        jfloat x,
        jfloat y)
{
    aTouches->registerActionDown(id, x, y);
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_CustomSurfaceView_registerActionUp(
        JNIEnv *env,
        jobject thiz,
        jint id,
        jfloat x,
        jfloat y)
{
    aTouches->registerActionUp(id, x, y);
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_CustomSurfaceView_registerActionMove(
        JNIEnv *env,
        jobject thiz,
        jint id,
        jfloat dx,
        jfloat dy)
{
    aTouches->registerActionMove(id, dx, dy);
}