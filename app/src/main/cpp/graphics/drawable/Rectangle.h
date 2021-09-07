#ifndef ILLEGAL_RACER_RECTANGLE_H
#define ILLEGAL_RACER_RECTANGLE_H

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class Rectangle
{
public:
    constexpr static const char *const TAG = "Rectangle";

    Rectangle();
    ~Rectangle();

    /**
     * Update vertices of rectangle
     * @param data - float array of <b>12</b> elements (XyzXyz...)
     */
    void setVertices(const float* data);

    /**
     * Update UV coordinates of texture
     * @param data - float array of <b>8</b> elements (UvUv...)
     */
    void setTexUVcoords(const float* data);

    void draw() const;

private:
    float vertices[12];
    float texUVcoords[8];
    float combinedData[20];
    /**
     * Object of vertex(es) array(s).
     * Contains: list of vertex attributes, buffers of indexes, buffers of vertex data, and etc.
     * Drawing by function glDrawArrays.
     */
    GLuint VAO;
    GLuint VBO;

};


#endif
