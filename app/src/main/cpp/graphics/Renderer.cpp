#include "Renderer.h"

#include <jni.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../LogHelper.h"

rend::Renderer::Renderer() {
    shader = new sh::Shader();
    triangle = new tr::Triangle();
}

rend::Renderer::~Renderer() {
    delete shader;
    delete triangle;
}
