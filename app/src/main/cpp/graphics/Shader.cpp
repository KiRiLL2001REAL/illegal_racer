#include "Shader.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../LogHelper.h"

#include <exception>
#include <fstream>
#include <sstream>

Shader::Shader(unsigned int id) :
    id(id)
{
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::use() const
{
    glUseProgram(id);
}

Shader* load_shader(const std::string &vFilePath, const std::string &fFilePath)
{
    using namespace std;

    string vCode;
    string fCode;
    ifstream vShFile;
    ifstream fShFile;

    // Чтение файлов
    vShFile.exceptions(ifstream::badbit);
    fShFile.exceptions(ifstream::badbit);
    try
    {
        vShFile.open(vFilePath);
        fShFile.open(fFilePath);
        stringstream vShStream, fShStream;

        vShStream << vShFile.rdbuf();
        fShStream << fShFile.rdbuf();

        vShFile.close();
        fShFile.close();

        vCode = vShStream.str();
        fCode = fShStream.str();
    }
    catch (ifstream::failure &e)
    {
        LOGE(Shader::TAG, "File not found");
        return nullptr;
    }
    const GLchar *vShaderCode = vCode.c_str();
    const GLchar *fShaderCode = fCode.c_str();

    // Компиляция шейдеров
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Вершинный
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        LOGE(Shader::TAG, "Vertex compilation failed: %s", infoLog);
        return nullptr;
    }

    // Фрагментный
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        LOGE(Shader::TAG, "Fragment compilation failed: %s", infoLog);
        return nullptr;
    }

    // Создание шейдерной программы (линковка)
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        LOGE(Shader::TAG, "Program linking failed: %s", infoLog);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(id);
        return nullptr;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return new Shader(id);
}