#include "Triangle.h"

#include "../../LogHelper.h"

#include <exception>

Triangle::Triangle():
    vertices{0.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f},
    texUVcoords{0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f},
    combinedData{},
    VAO(0),
    VBO(0)
{
    for (int i = 0; i < 9; i++) combinedData[    i + 2 * (i / 3)] = vertices[i];
    for (int i = 0; i < 6; i++) combinedData[3 + i + 3 * (i / 2)] = texUVcoords[i];

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(combinedData), combinedData, GL_STATIC_DRAW);
    // позиция
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE, // нужна ли нормализация в вектор
            5 * sizeof(GLfloat), // длина шага
            (GLvoid*)(0 * sizeof(GLfloat))
            // на сколько сдвинут аттрибут относительно начала в байтах
            );
    glEnableVertexAttribArray(0);
    // текстура
    // позиция
    glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE, // нужна ли нормализация в вектор
            5 * sizeof(GLfloat), // длина шага
            (GLvoid*)(3 * sizeof(GLfloat))
            // на сколько сдвинут аттрибут относительно начала в байтах
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Triangle::~Triangle()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Triangle::setVertices(const float *data)
{
    for (int i = 0; i < 9; i++)
    {
        vertices[i] = data[i];
        combinedData[i + 2 * (i / 3)] = data[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(combinedData), combinedData, GL_STATIC_DRAW);
}

void Triangle::setTexUVcoords(const float *data)
{
    for (int i = 0; i < 6; i++)
    {
        texUVcoords[i] = data[i];
        combinedData[3 + i + 3 * (i / 2)] = data[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(combinedData), combinedData, GL_STATIC_DRAW);
}

void Triangle::draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
