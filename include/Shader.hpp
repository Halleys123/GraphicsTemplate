#pragma once

#include "glad/glad.h"
#include "Logger.hpp"

class Shader
{
private:
    GLuint VertShaderID, FragShaderID, ShaderProgram;
    static Logger logger;

    bool initFrag = false;
    bool initVert = false;

private:
    void loadFromFile(char **shaderSrc, unsigned int &shaderLen, const char *path);
    void compileShader(char *shaderSrc, GLenum ShaderType);

public:
    Shader();
    Shader(const char *VertexShaderPath, const char *FragShaderPath);

    void SetupVert(const char *, GLenum);
    void SetupFrag(const char *, GLenum);
};
