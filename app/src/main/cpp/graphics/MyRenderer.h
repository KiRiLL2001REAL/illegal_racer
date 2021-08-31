#ifndef ILLEGAL_RACER_MYRENDERER_H
#define ILLEGAL_RACER_MYRENDERER_H

#include <jni.h>

#include "base_renderer/Renderer.h"

class MyRenderer : public Renderer
{
public:
    constexpr static const char *const TAG = "MyRenderer";

    MyRenderer();
    virtual ~MyRenderer();

    void loadShader(const std::string &vertexFilePath, const std::string &fragmentFilePath);

private:
    Shader *mShader;
    Triangle *mTriangle;

};

extern "C"
{
    JNIEXPORT void JNICALL Java_ru_killthereal_illegal_1racer_graphics_Renderer_loadShader(JNIEnv *env, jobject thiz, jstring vertex_shader_path, jstring fragment_shader_path);
}


#endif
