#ifndef ILLEGAL_RACER_RENDERER_H
#define ILLEGAL_RACER_RENDERER_H

#include "Shader.h"
#include "drawable/Triangle.h"

#include <vector>

namespace rend {
    class Renderer {
    private:
        const char *const TAG = "Renderer";
        sh::Shader *shader;
        tr::Triangle *triangle;

    public:
        Renderer();

        ~Renderer();
    };
}

#endif
