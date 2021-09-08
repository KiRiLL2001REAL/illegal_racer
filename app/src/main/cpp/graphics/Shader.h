#ifndef ILLEGAL_RACER_SHADER_H
#define ILLEGAL_RACER_SHADER_H

#include <string>
#include "../modules/glm/glm/glm.hpp"

class Shader
{
public:
    constexpr static const char *const TAG = "Shader";

    unsigned int id;

    Shader(unsigned int id);
    ~Shader();

    void use() const;
    void uniformMatrix(const std::string& name, glm::mat4 matrix) const;
};

extern Shader* load_shader(const std::string &vertexFilePath, const std::string &fragmentFilePath);

#endif
