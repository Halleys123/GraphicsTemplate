#include "Renderable.hpp"

Logger Renderable::logger = Logger("RENDERABLE::");
int Renderable::ID = 0;

// Without Shaders and texture
Renderable::Renderable(GLint *count, Position *positions)
{
    logger.log("Renderable initialized");
    mesh = new Mesh(positions, count[0]);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices)
{
    mesh = new Mesh(positions, count[0], indices, count[1]);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors) : Renderable(count, positions, indices)
{
    mesh->setupColors(colors, count[2]);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs)
{
    mesh = new Mesh(positions, count[0], indices, count[1]);
    mesh->setupColors(colors, count[2]);
    mesh->setupUV(uvs, count[3]);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals)
{
    // Normals not yet supported, but still setup mesh, colors, uvs.
    logger.warn("Normals parameter provided but Mesh normal support not implemented yet");
    mesh = new Mesh(positions, count[0], indices, count[1]);
    if (colors && count[2] > 0)
        mesh->setupColors(colors, count[2]);
    if (uvs && count[3] > 0)
        mesh->setupUV(uvs, count[3]);
}

// With shaders
Renderable::Renderable(GLint *count, Position *positions, const char *vertexShaderPath, const char *fragmentShaderPath)
    : Renderable(count, positions)
{
    shader = new Shader(vertexShaderPath, fragmentShaderPath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, const char *vertexShaderPath, const char *fragmentShaderPath)
    : Renderable(count, positions, indices)
{
    shader = new Shader(vertexShaderPath, fragmentShaderPath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, const char *vertexShaderPath, const char *fragmentShaderPath)
    : Renderable(count, positions, indices, colors)
{
    shader = new Shader(vertexShaderPath, fragmentShaderPath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, const char *vertexShaderPath, const char *fragmentShaderPath)
    : Renderable(count, positions, indices, colors, uvs)
{
    shader = new Shader(vertexShaderPath, fragmentShaderPath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals, const char *vertexShaderPath, const char *fragmentShaderPath)
    : Renderable(count, positions, indices, colors, uvs, normals)
{
    shader = new Shader(vertexShaderPath, fragmentShaderPath);
}

// With Shader and Texture
Renderable::Renderable(GLint *count, Position *positions, const char *vertexShaderPath, const char *fragmentShaderPath, const char *texturePath)
    : Renderable(count, positions, vertexShaderPath, fragmentShaderPath)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, const char *vertexShaderPath, const char *fragmentShaderPath, const char *texturePath)
    : Renderable(count, positions, indices, vertexShaderPath, fragmentShaderPath)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, const char *vertexShaderPath, const char *fragmentShaderPath, const char *texturePath)
    : Renderable(count, positions, indices, colors, vertexShaderPath, fragmentShaderPath)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, const char *vertexShaderPath, const char *fragmentShaderPath, const char *texturePath)
    : Renderable(count, positions, indices, colors, uvs, vertexShaderPath, fragmentShaderPath)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals, const char *vertexShaderPath, const char *fragmentShaderPath, const char *texturePath)
    : Renderable(count, positions, indices, colors, uvs, normals, vertexShaderPath, fragmentShaderPath)
{
    texture = new Texture(texturePath);
}

// With Texture only
Renderable::Renderable(GLint *count, Position *positions, const char *texturePath)
    : Renderable(count, positions)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, const char *texturePath)
    : Renderable(count, positions, indices)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, const char *texturePath)
    : Renderable(count, positions, indices, colors)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, const char *texturePath)
    : Renderable(count, positions, indices, colors, uvs)
{
    texture = new Texture(texturePath);
}
Renderable::Renderable(GLint *count, Position *positions, GLint *indices, Color *colors, UV *uvs, Normal *normals, const char *texturePath)
    : Renderable(count, positions, indices, colors, uvs, normals)
{
    texture = new Texture(texturePath);
}

// Fraw function
void Renderable::Draw()
{
    if (shader)
        shader->useProgram();
    if (texture)
        texture->Bind();
    if (mesh)
        mesh->draw();
}

Renderable::~Renderable()
{
    if (shader)
        delete shader;
    if (texture)
        delete texture;
    if (mesh)
        delete mesh;
}