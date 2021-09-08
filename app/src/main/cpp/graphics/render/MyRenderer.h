#ifndef ILLEGAL_RACER_MYRENDERER_H
#define ILLEGAL_RACER_MYRENDERER_H

#include <jni.h>

#include "Renderer.h"

#include "../Shader.h"
#include "../drawable/Triangle.h"
#include "../drawable/Rectangle.h"
#include "../Texture.h"
#include "../loaders/Loaders.h"
#include "../../controls/Camera.h"

#include "../../modules/glm/glm/glm.hpp"
#include "../../modules/glm/glm/ext.hpp"

class MyRenderer : public Renderer
{
public:
    constexpr static const char *const TAG = "MyRenderer";

    MyRenderer(JNIEnv *env, jobject wrappedInstance);
    virtual ~MyRenderer();

private:
    glm::mat4 mMatrixModel;

    Camera *mCamera;

    Shader *mShader;
    Texture *mTexture;
    Triangle *mTriangle;
    Rectangle *mRectangle;

    JavaVM *mJVM;
    jobject mObjectRef;
    jmethodID mMethodIdGetShaderPaths;
    jmethodID mMethodIdGetAssetFilePath;

    JNIEnv* getJNIEnv();
    void loadShader();
    void loadTexture();

protected:
    virtual void unloadStuff();
    virtual void loadStuff();
    virtual void drawScene();
};

#endif
