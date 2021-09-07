#include "Loaders.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../../LogHelper.h"

#include <png.h>
#include "../Texture.h"

int load_png(const char* file, int* width, int* height)
{
    FILE *f;
    int is_png, bit_depth, color_type, row_bytes;
    png_infop info_ptr, end_info;
    png_uint_32 t_width, t_height;
    png_byte header[8], *image_data;
    png_bytepp row_pointers;
    png_structp png_ptr;
    GLuint texture;
    int alpha;

    if (!(f = fopen(file, "rb")))
    {
        return 0;
    }
    fread(header, 1, 8, f);
    is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png)
    {
        fclose(f);
        return 0;
    }
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                     nullptr, nullptr);
    if (!png_ptr)
    {
        fclose(f);
        return 0;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp) nullptr,
                                (png_infopp) nullptr);
        fclose(f);
        return 0;
    }
    end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp) nullptr,
                                (png_infopp) nullptr);
        fclose(f);
        return 0;
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        return 0;
    }
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth, &color_type,
                 nullptr, nullptr, nullptr);
    *width = t_width;
    *height = t_height;
    png_read_update_info(png_ptr, info_ptr);
    row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    image_data = (png_bytep) malloc(row_bytes * t_height * sizeof(png_byte));
    if (!image_data)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        return 0;
    }
    row_pointers = (png_bytepp) malloc(t_height * sizeof(png_bytep));
    if (!row_pointers)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(f);
        return 0;
    }
    for (unsigned int i = 0; i < t_height; ++i)
    {
        row_pointers[t_height - 1 - i] = image_data + i * row_bytes;
    }
    png_read_image(png_ptr, row_pointers);
    switch (png_get_color_type(png_ptr, info_ptr))
    {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB;
            break;
        default:
            LOGE("load_png", "Color type %d not supported!\n",
                 png_get_color_type(png_ptr, info_ptr));
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            return 0;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // функция передачи текстурных данных в текстуру
    glTexImage2D(GL_TEXTURE_2D,
            // уровень мипмаппинга
                 0,
            // формат, в котором данные будут представлены в памяти
                 GL_RGBA,
                 t_width, t_height,
                 0,
            // формат данных
                 alpha,
            // тип данных пикселя
                 GL_UNSIGNED_BYTE,
            // указатель на данные
                 (GLvoid *) image_data);
    // фильтрация текстуры (пиксельная)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(f);

    return texture;
}

Texture* load_texture(const std::string &filepath)
{
    int width, height;
    LOGD(Texture::TAG, "Loading texture...");
    GLuint texture = load_png(filepath.c_str(), &width, &height);
    if (texture == 0)
    {
        LOGE(Texture::TAG, "Couldn't load texture %s", filepath.c_str());
        return nullptr;
    }
    LOGD(Texture::TAG, "Successful loaded.");
    return new Texture(texture, width, height);
}

