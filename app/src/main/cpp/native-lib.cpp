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
    aRenderer = new MyRenderer();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnResume(
        JNIEnv *env,
        jobject thiz)
{
    aRenderer->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnPause(
        JNIEnv *env,
        jobject thiz)
{
    aRenderer->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnStop(
        JNIEnv *env,
        jobject thiz)
{
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