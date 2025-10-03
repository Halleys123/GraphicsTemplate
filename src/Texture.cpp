#define STB_IMAGE_IMPLEMENTATION

#include "Texture.hpp"
#include "stb_image.h"

Logger Texture::logger = Logger();
int Texture::TextureCount = 0;

Texture::Texture(const char *path)
{
    logger.info("Creating Texture from path: %s", path);

    TextureCount += 1;
    TextureID = TextureCount;

    changeLoggerName();

    stbi_set_flip_vertically_on_load(true);
    img = stbi_load(path, &width, &height, &components, 0);
    if (!img)
    {
        logger.error("No image found at %s", path);
        return;
    }
    else
        logger.success("Image of width: %d and height %d loaded", width, height);

    logger.info("Generating OpenGL texture object");
    glGenTextures(1, &TextureObj);

    glBindTexture(GL_TEXTURE_2D, TextureObj);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    logger.info("Setting texture parameters");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLenum srcFormat = GL_RGB;
    GLenum internalFormat = GL_RGB;
    if (components == 4)
    {
        srcFormat = GL_RGBA;
        internalFormat = GL_RGBA;
        logger.info("Image has 4 components, using RGBA format");
    }
    else if (components == 1)
    {
        srcFormat = GL_RED;
        internalFormat = GL_R8;
        logger.info("Image has 1 component, using RED format");
    }
    else
    {
        logger.info("Image has %d components, using RGB format", components);
    }

    logger.info("Uploading texture data to GPU");
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, srcFormat, GL_UNSIGNED_BYTE, img);

    logger.info("Unbinding texture");
    glBindTexture(GL_TEXTURE_2D, 0);

    logger.info("Freeing image memory");
    stbi_image_free(img);

    logger.success("Texture %d created successfully", TextureID);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureObj);
}

void Texture::changeLoggerName()
{
    char name[32];
    snprintf(name, sizeof(name), "Texture::%d", TextureID);

    logger.setLoggerName(name);
}