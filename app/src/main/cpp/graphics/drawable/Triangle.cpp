#include "Triangle.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../../LogHelper.h"

#include <exception>

Triangle::Triangle():
    vertices{0.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f},
    VAO(0),
    VBO(0)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // позиция
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE, // нужна ли нормализация в вектор
            3 * sizeof(GLfloat), // длина шага
            (GLvoid*)(0 * sizeof(GLfloat))
            // на сколько сдвинут аттрибут относительно начала в байтах
            );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Triangle::~Triangle()
{

}

void Triangle::setVertices(const float *data)
{
    for (int i = 0; i < 9; i++)
        vertices[i] = data[i];
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Triangle::draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
