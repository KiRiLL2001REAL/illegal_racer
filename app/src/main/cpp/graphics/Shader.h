#ifndef ILLEGAL_RACER_SHADER_H
#define ILLEGAL_RACER_SHADER_H

#include <string>

class Shader
{
public:
    constexpr static const char *const TAG = "Shader";

    unsigned int id;

    Shader(unsigned int id);
    ~Shader();

    void use() const;
};

extern Shader* load_shader(const std::string &vertexFilePath, const std::string &fragmentFilePath);

#endif
