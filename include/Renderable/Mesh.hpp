#pragma once

#include "glad/glad.h"
#include "Vertex.hpp"
#include "stdlib.h"
#include "Logger.hpp"

class Mesh
{
private:
    static int MeshCount;
    int MeshID = 0;

    Vertex *vertices = nullptr;
    GLuint *indices = nullptr;
    int vertexCount = 0, indexCount = 0;

    GLuint VBO = -1, VAO = -1, EBO = -1;
    static Logger logger;

private:
    void changeLoggerName();

public:
    Mesh(Vertex *vertexSrc = nullptr, int vertexCount = 0, GLuint *indexSrc = nullptr, int indexCount = 0);
    ~Mesh();

    // Disallow copy and allow move if needed later
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    void setupVertex(Vertex *vertexSrc = nullptr, int vertexCount = 0);
    void setupIndices(GLuint *indices = nullptr, int indexCount = 0);
    void draw();
};
