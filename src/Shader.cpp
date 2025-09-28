#include "stdio.h"
#include "Shader.hpp"
#include <Windows.h>

Logger Shader::logger = Logger("SHADERS");
int Shader::ShaderCount = 1;

void Shader::initShader()
{
    VertShaderID = -1;
    FragShaderID = -1;
    ShaderProgram = -1;
    ShaderCount += 1;
    ShaderID = ShaderCount;
}

Shader::Shader()
{
    initShader();
}

Shader::Shader(const char *VertexShaderPath, const char *FragShaderPath)
{
    initShader();

    SetupFrag(VertexShaderPath);
    SetupVert(FragShaderPath);

    linkShaderProgram();
}

Shader::~Shader()
{
    logger.warn("Now in the process of deleting Shader Object %d", ShaderID);

    glDeleteProgram(ShaderProgram);
    int status;
    glGetProgramiv(ShaderProgram, GL_DELETE_STATUS, &status);

    if (status == GL_TRUE)
        logger.info("Shader program has been deleted from memory");
    else
        logger.info("Shader program will be deleted from memory");

    deleteShader(VertShaderID);
    deleteShader(FragShaderID);
}

void Shader::SetupVert(const char *path)
{
    char *vertShdSrc = nullptr;
    unsigned int vertLen = 0;

    loadFromFile(&vertShdSrc, vertLen, path);
    compileShader(vertShdSrc, GL_VERTEX_SHADER);
    free(vertShdSrc);
}
void Shader::SetupFrag(const char *path)
{
    char *fragShdSrc = nullptr;
    unsigned int fragLen = 0;

    loadFromFile(&fragShdSrc, fragLen, path);

    compileShader(fragShdSrc, GL_FRAGMENT_SHADER);
    free(fragShdSrc);
}

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

    *shaderSrc = (char *)malloc((fileLength + 1) * sizeof(char));
    fread(*shaderSrc, 1, fileLength, file);
    (*shaderSrc)[fileLength] = '\0';

    Shader::logger.silly("\n%s", *shaderSrc);

    fclose(file);
}

void Shader::compileShader(char *shaderSrc, GLenum ShaderType)
{
    unsigned int *shaderId = nullptr;
    if (ShaderType == GL_VERTEX_SHADER)
        shaderId = &VertShaderID;
    else if (ShaderType == GL_FRAGMENT_SHADER)
        shaderId = &FragShaderID;
    else
    {
        MessageBox(NULL, "Invalid type of ShaderType passed to compiler Shader", "Invalid Shader Type", MB_OK | MB_ICONERROR);
        PostMessage(NULL, WM_DESTROY, NULL, NULL);
        return;
    }

    *shaderId = glCreateShader(ShaderType);
    GLint srcLength = strlen(shaderSrc);
    glShaderSource(*shaderId, 1, &shaderSrc, &srcLength);
    glCompileShader(*shaderId);

    int compilePass;
    glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &compilePass);
    if (compilePass == GL_FALSE)
    {
        int len = 0;
        char errStr[500];
        glGetShaderInfoLog(*shaderId, 500, &len, errStr);
        logger.error(errStr);
        *shaderId = -1;
    }
    else
    {
        logger.success("Shader has successfully compiled");
    }

    // ! Do not free or delete shaderId as it is a pointer and not a memory allocated with malloc or calloc
}

void Shader::linkShaderProgram()
{
    if (VertShaderID == -1 || FragShaderID == -1)
    {
        logger.fatal("Either or Both fragment and vertex shader are not laoded.");
        return;
    }

    logger.info("Now Linking vertex, fragment shader with shader program");

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertShaderID);
    glAttachShader(ShaderProgram, FragShaderID);
    glLinkProgram(ShaderProgram);

    deleteShader(VertShaderID);
    deleteShader(FragShaderID);

    int status;
    glGetShaderiv(ShaderProgram, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLsizei len = 0;
        char error[500];

        glGetProgramInfoLog(ShaderProgram, 500, &len, error);
        error[len] = '\0';
        logger.error(error);
    }
}

void Shader::useProgram()
{
    if (ShaderProgram == -1)
    {
        logger.error("Initialize and link the shader program before using.");
        return;
    }

    glUseProgram(ShaderProgram);
}

void Shader::deleteShader(GLuint Shader)
{
    glDeleteShader(Shader);

    int status;
    GLint ShaderType;

    glGetShaderiv(Shader, GL_DELETE_STATUS, &status);
    glGetShaderiv(Shader, GL_SHADER_TYPE, &ShaderType);

    if (status == GL_TRUE)
        logger.info("%s shader has been erased", ShaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader");
    else
        logger.warn("%s don't exists or has been already deleted.", ShaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader");
}