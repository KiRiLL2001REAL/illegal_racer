#include "MyRenderer.h"

#include <jni.h>
#include "../LogHelper.h"
#include "../native-lib.h"

#include <vector>

MyRenderer::MyRenderer(JNIEnv *env, jobject wrappedInstance) :
    mJVM(nullptr),
    mShader(nullptr),
    mTriangle(nullptr)
{
    // Запоминаем JavaVM для получения свежего env когда это потребуется
    env->GetJavaVM(&mJVM);
    // Запоминаем ссылку на вызвавший класс (в данном случае - MainActivity)
    mObjectRef = env->NewGlobalRef(wrappedInstance);

    jclass aClass = env->GetObjectClass(wrappedInstance);
    // Запоминаем id необходимого метода
    mMethodIdGetShaderPaths = env->GetMethodID(
            aClass,"getShaderPaths",
            "(Ljava/lang/String;Ljava/lang/String;)Ljava/util/ArrayList;");
}

MyRenderer::~MyRenderer()
{
    JNIEnv* env;
    if ((env = getJNIEnv()) != nullptr)
    {
        env->DeleteGlobalRef(mObjectRef);
    }
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

JNIEnv *MyRenderer::getJNIEnv()
{
    JavaVMAttachArgs attachArgs;
    attachArgs.version = JNI_VERSION_1_6;
    attachArgs.name = "getJNIEnv";
    attachArgs.group = nullptr;

    JNIEnv* env;
    if (mJVM->AttachCurrentThread(&env, &attachArgs) != JNI_OK) {
        env = nullptr;
    }

    return env;
}

void MyRenderer::loadShader()
{
    using namespace std;

    JNIEnv* env;
    if ((env = getJNIEnv()) != nullptr)
    {
        string vertexName = "vertex.vsh";
        string fragmentName = "fragment_shader.vsh";

        jstring jvertexName = env->NewStringUTF("vertex.vsh");
        jstring jfragmentName = env->NewStringUTF("fragment_shader.vsh");

        // Получаем ArrayList<String> из 2х элементов
        jobject jpaths = env->CallObjectMethod(mObjectRef, mMethodIdGetShaderPaths,
                                               jvertexName, jfragmentName);

        env->DeleteLocalRef(jvertexName);
        env->DeleteLocalRef(jfragmentName);

        // Разбираем ArrayList
        jmethodID arrListGet = env->GetMethodID(
                env->FindClass("java/util/ArrayList"),
                "get", "(I)Ljava/lang/Object;");
            // Извлечение пути до вершинного шейдера
        auto jstr = (jstring)env->CallObjectMethod(jpaths, arrListGet, 0);
        const char *chars = env->GetStringUTFChars(jstr, nullptr);
        string vertexFilePath = string(chars);
        env->ReleaseStringUTFChars(jstr, chars);
            // Извлечение пути до фрагментного шейдера
        jstr = (jstring)env->CallObjectMethod(jpaths, arrListGet, 1);
        chars = env->GetStringUTFChars(jstr, nullptr);
        string fragmentFilePath = string(chars);
        env->ReleaseStringUTFChars(jstr, chars);

        env->DeleteLocalRef(jstr);
        env->DeleteLocalRef(jpaths);

        mShader = load_shader(vertexFilePath, fragmentFilePath);
    }
    else
        LOGE(TAG, "Can not load shader. Environment is unavailable");
}

void MyRenderer::loadStuff()
{
    if (mShader == nullptr)
        loadShader();
}