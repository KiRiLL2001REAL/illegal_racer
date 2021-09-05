#ifndef ILLEGAL_RACER_NATIVE_LIB_H
#define ILLEGAL_RACER_NATIVE_LIB_H

#include <jni.h>
#include <EGL/egl.h>
#include "graphics/render/MyRenderer.h"

static ANativeWindow *aWindow = nullptr;
static MyRenderer *aRenderer = nullptr;

extern "C"
{
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnStart(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnResume(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnPause(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnStop(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeSetSurface(JNIEnv *env, jobject thiz, jobject surface);
}

#endif