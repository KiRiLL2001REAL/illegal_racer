#include "Rectangle.h"

Rectangle::Rectangle() :
        vertices{0.0f, 0.0f, 0.0f,
                 1.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f,
                 1.0f, 1.0f, 0.0f},
        texUVcoords{0.0f, 0.0f,
                    1.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f},
        combinedData{},
        VAO(0),
        VBO(0)
{
    for (int i = 0; i < 12; i++) combinedData[   i + 2 * (i / 3)] = vertices[i];
    for (int i = 0; i < 8; i++) combinedData[3 + i + 3 * (i / 2)] = texUVcoords[i];

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

Rectangle::~Rectangle()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Rectangle::setVertices(const float *data)
{
    for (int i = 0; i < 12; i++)
    {
        vertices[i] = data[i];
        combinedData[i + 2 * (i / 3)] = data[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(combinedData), combinedData, GL_STATIC_DRAW);
}

void Rectangle::setTexUVcoords(const float *data)
{
    for (int i = 0; i < 8; i++)
    {
        texUVcoords[i] = data[i];
        combinedData[3 + i + 3 * (i / 2)] = data[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(combinedData), combinedData, GL_STATIC_DRAW);
}

void Rectangle::draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
