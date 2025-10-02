#pragma once

#include "glad/glad.h"
#include "Logger.hpp"

class Texture
{
private:
    static Logger logger;

    static int TextureCount;
    int TextureID = 0;

    unsigned char *img;
    int width, height, components;

    GLuint TextureObj;

private:
    void changeLoggerName();

public:
    Texture(const char *pathToTexture);
    void Bind();
};