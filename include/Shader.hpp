#pragma once

#include "glad/glad.h"
#include "Logger.hpp"

class Shader
{
private:
    int ShaderID;

    GLuint VertShaderID, FragShaderID, ShaderProgram;

    static Logger logger;
    static int ShaderCount;

private:
    // Used for initiation of id, shadercount increasing, assigning id to this object.
    void initShader();
    void deleteShader(GLuint Shader);

    void loadFromFile(char **shaderSrc, unsigned int &shaderLen, const char *path);
    void compileShader(char *shaderSrc, GLenum ShaderType);
    void linkShaderProgram();

    void changeLoggerName();

public:
    Shader();
    ~Shader();
    Shader(const char *VertexShaderPath, const char *FragShaderPath);

    void SetupVert(const char *);
    void SetupFrag(const char *);
    void SetupShader(const char *VertexShaderPath, const char *FragShaderPath);

    void useProgram();
    GLint getUniformLocation(const char *);
};
