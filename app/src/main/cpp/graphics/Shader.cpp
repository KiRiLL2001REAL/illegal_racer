#include "Shader.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../LogHelper.h"

#include "../modules/glm/glm/gtc/type_ptr.hpp"

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

void Shader::uniformMatrix(const std::string& name, glm::mat4 matrix) const
{
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader* load_shader(const std::string &vFilePath, const std::string &fFilePath)
{
    using namespace std;

    string vCode;
    string fCode;
    ifstream vShFile;
    ifstream fShFile;

    LOGI(Shader::TAG, "Loading shader...");

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

    // Вершинный
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    GLint vertex_compiled;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertex_compiled);
    if (vertex_compiled != GL_TRUE)
    {
        GLchar message[1024];
        message[0] = '\0';
        glGetShaderInfoLog(vertex, 1024, nullptr, message);
        LOGE(Shader::TAG, "Vertex compilation failed: %s", message);
        throw exception();
    }

    // Фрагментный
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    GLint fragment_compiled;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &fragment_compiled);
    if (fragment_compiled != GL_TRUE)
    {
        GLchar message[1024];
        message[0] = '\0';
        glGetShaderInfoLog(fragment, 1024, nullptr, message);
        LOGE(Shader::TAG, "Fragment compilation failed: %s", message);
        throw exception();
    }

    // Создание шейдерной программы (линковка)
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertex);
    glAttachShader(programId, fragment);
    glLinkProgram(programId);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint program_linked;
    glGetProgramiv(programId, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLchar message[1024];
        message[0] = '\0';
        glGetProgramInfoLog(programId, 1024, nullptr, message);
        LOGE(Shader::TAG, "Program linking failed: %s", message);
        glDeleteProgram(programId);
        throw exception();
    }

    LOGI(Shader::TAG, "Successful loaded.");

    return new Shader(programId);
}