#pragma once

#include "Vertex.hpp"

#include "Shader.hpp"
#include "Logger.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

class Renderable
{
    static int ID;
    static Logger logger;

    int instanceID = 0;

    Texture *texture = nullptr;
    Shader *shader = nullptr;
    Mesh *mesh = nullptr;

public:
    // GLint* count is a array of size 5 containing number of Positions, indices, color, uvs, normal in it in same order index
    // All parameters - with shaders and texture
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals, const char *vertexShader, const char *fragmentShader, const char *texture);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, const char *vertexShader, const char *fragmentShader, const char *texture);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, const char *vertexShader, const char *fragmentShader, const char *texture);
    Renderable(GLint *count, Position *positions, GLint *indices, const char *vertexShader, const char *fragmentShader, const char *texture);
    Renderable(GLint *count, Position *positions, const char *vertexShader, const char *fragmentShader, const char *texture);
    // Texture not provided - with shaders only
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals, const char *vertexShader, const char *fragmentShader);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, const char *vertexShader, const char *fragmentShader);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, const char *vertexShader, const char *fragmentShader);
    Renderable(GLint *count, Position *positions, GLint *indices, const char *vertexShader, const char *fragmentShader);
    Renderable(GLint *count, Position *positions, const char *vertexShader, const char *fragmentShader);
    // Shaders not provided - with texture only
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals, const char *texture);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, const char *texture);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, const char *texture);
    Renderable(GLint *count, Position *positions, GLint *indices, const char *texture);
    Renderable(GLint *count, Position *positions, const char *texture);
    // Only mesh data - no shaders, no texture
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs);
    Renderable(GLint *count, Position *positions, GLint *indices, Color *colors);
    Renderable(GLint *count, Position *positions, GLint *indices);
    Renderable(GLint *count, Position *positions);
    // Renderable destructor
    ~Renderable();

    void Draw();

    template <typename T>
    void SetUniform(const char *name, T val);
    template <typename T>
    void SetUniform(const char *name, T a, T b);
    template <typename T>
    void SetUniform(const char *name, T a, T b, T c);
    template <typename T>
    void SetUniform(const char *name, T a, T b, T c, T d);
    template <typename T>
    void SetUniform(const char *name, T mat[4][4]);
};

template <typename T>
void Renderable::SetUniform(const char *name, T val)
{
    if (!shader)
    {
        logger.error("SetUniform called but shader is null");
        return;
    }
    shader->useProgram();
    GLint uniformLocation = shader->getUniformLocation(name);
    if constexpr (std::is_same_v<T, int>)
        glUniform1i(uniformLocation, val);
    else if constexpr (std::is_same_v<T, float>)
        glUniform1f(uniformLocation, val);
    else
    {
        logger.fatal("Invalid type detected in uniform setting");
        logger.fatal("Shutting down the program");
        exit(1);
    }
}

template <typename T>
void Renderable::SetUniform(const char *name, T a, T b)
{
    if (!shader)
    {
        logger.error("SetUniform (2 params) called but shader is null");
        return;
    }
    shader->useProgram();
    GLint uniformLocation = shader->getUniformLocation(name);
    if constexpr (std::is_same_v<T, int>)
        glUniform2i(uniformLocation, a, b);
    else if constexpr (std::is_same_v<T, float>)
        glUniform2f(uniformLocation, a, b);
    else
    {
        logger.fatal("Invalid type detected in uniform setting");
        logger.fatal("Shutting down the program");
        exit(1);
    }
}

template <typename T>
void Renderable::SetUniform(const char *name, T a, T b, T c)
{
    if (!shader)
    {
        logger.error("SetUniform (3 params) called but shader is null");
        return;
    }
    shader->useProgram();
    GLint uniformLocation = shader->getUniformLocation(name);
    if constexpr (std::is_same_v<T, int>)
        glUniform3i(uniformLocation, a, b, c);
    else if constexpr (std::is_same_v<T, float>)
        glUniform3f(uniformLocation, a, b, c);
    else
    {
        logger.fatal("Invalid type detected in uniform setting");
        logger.fatal("Shutting down the program");
        exit(1);
    }
}

template <typename T>
void Renderable::SetUniform(const char *name, T a, T b, T c, T d)
{
    if (!shader)
    {
        logger.error("SetUniform (4 params) called but shader is null");
        return;
    }
    shader->useProgram();
    GLint uniformLocation = shader->getUniformLocation(name);
    if constexpr (std::is_same_v<T, int>)
        glUniform4i(uniformLocation, a, b, c, d);
    else if constexpr (std::is_same_v<T, float>)
        glUniform4f(uniformLocation, a, b, c, d);
    else
    {
        logger.fatal("Invalid type detected in uniform setting");
        logger.fatal("Shutting down the program");
        exit(1);
    }
}

template <typename T>
void Renderable::SetUniform(const char *name, T mat[4][4])
{
    if (!shader)
    {
        logger.error("SetUniform (4 params) called but shader is null");
        return;
    }
    shader->useProgram();
    GLint uniformLocation = shader->getUniformLocation(name);

    if constexpr (std::is_same_v<T, float>)
    {
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
    }
    else
    {
        logger.fatal("Invalid type detected in uniform setting");
        logger.fatal("Shutting down the program");
        exit(1);
    }
}
