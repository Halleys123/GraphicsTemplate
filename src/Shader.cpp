#include "stdio.h"
#include "Shader.hpp"
#include <Windows.h>

Logger Shader::logger = Logger("SHADERS");
int Shader::ShaderCount = 0;

void Shader::initShader()
{
    VertShaderID = -1;
    FragShaderID = -1;
    ShaderProgram = -1;
    ShaderCount += 1;
    ShaderID = ShaderCount;

    changeLoggerName();
}

Shader::Shader()
{
    initShader();
}

Shader::Shader(const char *VertexShaderPath, const char *FragShaderPath)
{
    initShader();
    SetupShader(VertexShaderPath, FragShaderPath);
}

Shader::~Shader()
{
    changeLoggerName();
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

void Shader::SetupShader(const char *VertexShaderPath, const char *FragShaderPath)
{
    SetupVert(VertexShaderPath);
    SetupFrag(FragShaderPath);

    linkShaderProgram();
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
    changeLoggerName();
    if (!path)
    {
        Shader::logger.error("Shader path not provided, Unable to compile shader");
        return;
    }
    // NOTE: Use binary mode so ftell gives us the exact byte size on Windows.
    FILE *file = fopen(path, "rb");

    if (!file)
    {
        Shader::logger.error("No shader source file found at path: %s", path);
        return;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    if (len < 0)
    {
        Shader::logger.error("Failed to determine shader file length: %s", path);
        fclose(file);
        return;
    }
    fileLength = static_cast<unsigned int>(len);
    fseek(file, 0, SEEK_SET);

    *shaderSrc = (char *)malloc((fileLength + 1) * sizeof(char));
    if (!*shaderSrc)
    {
        Shader::logger.error("Failed to allocate %u bytes for shader source: %s", fileLength, path);
        fclose(file);
        return;
    }
    size_t actuallyRead = fread(*shaderSrc, 1, fileLength, file);
    (*shaderSrc)[actuallyRead] = '\0';
    if (actuallyRead != fileLength)
    {
        Shader::logger.warn("Expected %u bytes but read %zu bytes for shader %s", fileLength, actuallyRead, path);
        fileLength = (unsigned int)actuallyRead; // Adjust to what we actually read
    }

    Shader::logger.success("Shader Source file loaded successfully: %s", path);

    Shader::logger.silly("\n%s", *shaderSrc);

    fclose(file);
}

void Shader::compileShader(char *shaderSrc, GLenum ShaderType)
{
    changeLoggerName();
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

    if (!shaderSrc || shaderSrc[0] == '\0')
    {
        logger.error("Empty shader source provided (type: %s)", ShaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        *shaderId = -1;
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
        errStr[len] = '\0';
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
    changeLoggerName();
    if (VertShaderID == -1 || FragShaderID == -1)
    {
        logger.fatal("Either or both fragment and vertex shader are not loaded.");
        return;
    }

    logger.info("Now Linking vertex, fragment shader with shader program");

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertShaderID);
    glAttachShader(ShaderProgram, FragShaderID);
    glLinkProgram(ShaderProgram);
    // Detach & delete shaders after a successful link attempt regardless of outcome.
    glDetachShader(ShaderProgram, VertShaderID);
    glDetachShader(ShaderProgram, FragShaderID);
    deleteShader(VertShaderID);
    deleteShader(FragShaderID);

    int status = 0;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLsizei len = 0;
        char error[1024];
        glGetProgramInfoLog(ShaderProgram, (GLsizei)sizeof(error), &len, error);
        if (len >= 0 && len < (GLsizei)sizeof(error))
            error[len] = '\0';
        logger.error("Program link failed: %s", error);
        ShaderProgram = -1;
    }
    else
    {
        logger.success("Shader program linked successfully");
    }
}

void Shader::useProgram()
{
    changeLoggerName();
    if (ShaderProgram == -1)
    {
        logger.error("Initialize and link the shader program before using.");
        return;
    }

    glUseProgram(ShaderProgram);
}

void Shader::deleteShader(GLuint Shader)
{
    changeLoggerName();
    if ((int)Shader <= 0) // Guard against invalid / already deleted IDs (we store -1 on failure)
        return;
    glDeleteShader(Shader);

    int status;
    GLint ShaderType;

    glGetShaderiv(Shader, GL_DELETE_STATUS, &status);
    glGetShaderiv(Shader, GL_SHADER_TYPE, &ShaderType);

    if (status == GL_TRUE)
        logger.info("%s shader has been erased", ShaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader");
    else
        logger.warn("%s doesn't exist or has been already deleted.", ShaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader");

    if (ShaderType == GL_VERTEX_SHADER)
        VertShaderID = -1;
    else if (ShaderType == GL_FRAGMENT_SHADER)
        FragShaderID = -1;
}

void Shader::changeLoggerName()
{
    char name[32];
    snprintf(name, sizeof(name), "SHADER::%d", ShaderID);

    logger.setLoggerName(name);
}

GLint Shader::getUniformLocation(const char *name)
{
    return glGetUniformLocation(this->ShaderProgram, name);
}