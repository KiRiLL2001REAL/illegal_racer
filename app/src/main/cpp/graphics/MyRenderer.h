#ifndef ILLEGAL_RACER_MYRENDERER_H
#define ILLEGAL_RACER_MYRENDERER_H

#include <jni.h>

#include "base_renderer/Renderer.h"

class MyRenderer : public Renderer
{
public:
    constexpr static const char *const TAG = "MyRenderer";

    MyRenderer(JNIEnv *env, jobject wrappedInstance);
    virtual ~MyRenderer();

private:
    Shader *mShader;
    Triangle *mTriangle;

    JavaVM *mJVM;
    jobject mObjectRef;
    jmethodID mMethodIdGetShaderPaths;

    JNIEnv* getJNIEnv();
    void loadShader();

};

#endif
