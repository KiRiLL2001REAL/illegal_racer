#include "Texture.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../LogHelper.h"

#include <exception>
#include <fstream>
#include <sstream>

Texture::Texture(unsigned int id, int width, int height) :
    id(id),
    width(width),
    height(height)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}
