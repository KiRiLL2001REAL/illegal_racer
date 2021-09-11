#ifndef ILLEGAL_RACER_NATIVE_LIB_H
#define ILLEGAL_RACER_NATIVE_LIB_H

#include <jni.h>
#include <EGL/egl.h>
#include "graphics/render/MyRenderer.h"
#include "controls/Touches.h"

static ANativeWindow *aWindow = nullptr;
static MyRenderer *aRenderer = nullptr;
static touch::Touches *aTouches = nullptr;

extern "C"
{
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnStart(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnResume(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnPause(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeOnStop(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_MainActivity_nativeSetSurface(JNIEnv *env, jobject thiz, jobject surface);

    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_CustomSurfaceView_registerActionDown(JNIEnv *env, jobject thiz, jint id, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_CustomSurfaceView_registerActionUp(JNIEnv *env, jobject thiz, jint id, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_CustomSurfaceView_registerActionMove(JNIEnv *env, jobject thiz, jint id, jfloat dx, jfloat dy);
}

#endif