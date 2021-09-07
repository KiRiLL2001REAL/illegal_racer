#ifndef ILLEGAL_RACER_TEXTURE_H
#define ILLEGAL_RACER_TEXTURE_H

#include <string>

class Texture
{
public:
    constexpr static const char *const TAG = "Texture";

    unsigned int id;
    int width, height;

    Texture(unsigned int id, int width, int height);
    ~Texture();

    void bind() const;
};

extern Texture* load_texture(const std::string &textureFilePath);

#endif
