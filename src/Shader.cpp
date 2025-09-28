#include "stdio.h"
#include "Shader.hpp"

Logger Shader::logger = Logger("SHADERS");

void Shader::loadFromFile(char **shaderSrc, unsigned int &fileLength, const char *path)
{
    if (!path)
    {
        Shader::logger.error("Shader path not provided, Unable to compile shader");
        return;
    }
    FILE *file = fopen(path, "r");

    if (!file)
    {
        Shader::logger.error("No shader source file found at path: %s", path);
        return;
    }

    fseek(file, 0, SEEK_END);
    Shader::logger.success("Shader Source file loaded successfully: %s", path);
    fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    *shaderSrc = new char[fileLength + 1];
    fread(*shaderSrc, 1, fileLength, file);
    (*shaderSrc)[fileLength] = '\0';

    Shader::logger.silly("\n%s", *shaderSrc);

    fclose(file);
}

Shader::Shader() : initFrag(false), initVert(false) {}

Shader::Shader(const char *VertexShaderPath, const char *FragShaderPath)
{
    char *vertShdSrc = nullptr, *fragShdSrc = nullptr;
    unsigned int vertLen = 0, fragLen = 0;

    loadFromFile(&vertShdSrc, vertLen, VertexShaderPath);
    loadFromFile(&fragShdSrc, fragLen, FragShaderPath);

    free(vertShdSrc);
    free(fragShdSrc);
}
