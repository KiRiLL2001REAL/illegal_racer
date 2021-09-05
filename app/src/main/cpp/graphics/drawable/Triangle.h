#ifndef ILLEGAL_RACER_TRIANGLE_H
#define ILLEGAL_RACER_TRIANGLE_H

#include <GLES3/gl3.h>

class Triangle
{
public:
    constexpr static const char *const TAG = "Triangle";

    Triangle();
    ~Triangle();

    /**
     * Update vertices of triangle
     * @param data - float array of <b>9</b> elements (XyzXyzXyz)
     */
    void setVertices(const float* data);

    void draw() const;

private:
    float vertices[9];
    /**
     * Object of vertex(es) array(s).
     * Contains: list of vertex attributes, buffers of indexes, buffers of vertex data, and etc.
     * Drawing by function glDrawArrays.
     */
    GLuint VAO;
    GLuint VBO;

};

#endif
