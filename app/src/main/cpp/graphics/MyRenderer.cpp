#include "MyRenderer.h"

#include <jni.h>
#include "../LogHelper.h"
#include "../native-lib.h"

MyRenderer::MyRenderer() :
    mShader(nullptr),
    mTriangle(nullptr)
{
}

MyRenderer::~MyRenderer()
{
    Renderer::~Renderer();
    if (mShader)
    {
        delete mShader;
        mShader = nullptr;
    }
    if (mTriangle)
    {
        delete mTriangle;
        mTriangle = nullptr;
    }
}

void MyRenderer::loadShader(const std::string& vFilePath, const std::string& fFilePath)
{
    mShader = load_shader(vFilePath, fFilePath);
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_killthereal_illegal_1racer_graphics_Renderer_loadShader(
        JNIEnv *env,
        jobject thiz,
        jstring vertex_shader_path,
        jstring fragment_shader_path)
{
    using namespace std;

    if (!vertex_shader_path || !fragment_shader_path)
    {
        LOGE(Renderer::TAG, "Empty jstring found");
        return;
    }
    if (!aRenderer)
    {
        LOGE(Renderer::TAG, "Attempt to access uninitialized renderer");
        return;
    }

    string vShPath, fShPath;

    jclass stringClass = env->GetObjectClass(vertex_shader_path);
    jmethodID getBytes = env->GetMethodID(stringClass, "getBytes",
                                          "(Ljava/lang/String;)[B");
    env->DeleteLocalRef(stringClass);

    // Конвертация строки с местоположением файла вершинного шейдера
    auto stringJbytes = (jbyteArray) env->CallObjectMethod(
            vertex_shader_path,
            getBytes,
            env->NewStringUTF("UTF-8"));
    auto length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, nullptr);

    vShPath = string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);
    env->DeleteLocalRef(stringJbytes);

    // Конвертация строки с местоположением файла фрагментного шейдера
    stringJbytes = (jbyteArray) env->CallObjectMethod(
            vertex_shader_path,
            getBytes,
            env->NewStringUTF("UTF-8"));
    length = (size_t) env->GetArrayLength(stringJbytes);
    pBytes = env->GetByteArrayElements(stringJbytes, nullptr);

    fShPath = string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);
    env->DeleteLocalRef(stringJbytes);

    // Вызов итогового метода
    aRenderer->loadShader(vShPath, fShPath);
}
