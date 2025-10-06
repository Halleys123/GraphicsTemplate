#pragma once

#include "glad/glad.h"
#include "Vertex.hpp"
#include "stdlib.h"
#include "Logger.hpp"

class Mesh
{
private:
    const enum VBO_INDEX {
        POSITION = 0,
        COLOR = 1,
        UV_I = 2,
        NORMAL = 3
    };

    static int MeshCount;
    static Logger logger;

    int MeshID = 0;

    GLuint VBO[4] = {0, 0, 0, 0}; // Position, Color, UV, Normal
    GLuint VAO = 0, EBO = 0;

    bool solidifed = false;

    int indexCount = 0, vertexCount = 0;
    int normalCount = 0, uvCount = 0, colorCount = 0;

    // Not too good for GPU cache but good to protect from unnecessary load memory
    GLint *indices = nullptr;
    Normal *normals = nullptr;
    UV *uvs = nullptr;
    Position *vertCoords = nullptr;
    Color *colors = nullptr;

    bool logWhileDrawDone = true;

private:
    void changeLoggerName();

public:
    Mesh(Position *positions, int positionCount, GLint *indices = nullptr, int indexCount = 0);
    ~Mesh();

    // Disallow copy and allow move if needed later
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    void setupPositions(Position *positions, int positionCount, GLint *indices = nullptr, int indexCount = 0);
    void setupColors(Color *colors, int colorCount);
    void setupUV(UV *uv, int count);
    void setupNormals(Normal *normals, int count);
    void draw();
};
