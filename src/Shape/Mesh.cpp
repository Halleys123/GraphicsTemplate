#include "glad/glad.h"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "stdlib.h"

int Mesh::MeshCount = 0;
Logger Mesh::logger = Logger();

Mesh::Mesh(Position *vertCoords, int vertexCount, GLint *indices, int indexCount)
{
    MeshID = ++MeshCount;
    changeLoggerName();

    glGenVertexArrays(1, &VAO);

    setupPositions(vertCoords, vertexCount, indices, indexCount);
}

void Mesh::changeLoggerName()
{
    char name[32];
    snprintf(name, sizeof(name), "MESH::%d", MeshID);

    logger.setLoggerName(name);
}

Mesh::~Mesh()
{
    if (normals)
        free(normals);
    if (uvs)
        free(uvs);
    if (vertCoords)
        free(vertCoords);
    if (colors)
        free(colors);
}

void Mesh::setupPositions(Position *vertCoords, int vertexCount, GLint *indices, int indexCount)
{
    if (this->vertCoords)
        free(this->vertCoords);

    if (this->indices)
        free(this->indices);

    if (!vertCoords || vertexCount <= 0)
    {
        logger.error("At least vertex coordinates are required to be provided");
        if (vertCoords && vertexCount <= 0)
            logger.error("Please provide the count of coordinates");
        if (!vertCoords && vertexCount)
            logger.error("Pointer to coordinates is empty");

        return;
    }

    this->vertexCount = vertexCount;
    this->indexCount = indexCount;

    // Copy vertCoords to memory;
    logger.info("Copying vertex to memory");
    this->vertCoords = (Position *)malloc(sizeof(Position) * this->vertexCount);
    memcpy(this->vertCoords, vertCoords, sizeof(Position) * this->vertexCount);

    // Add those to VBO
    // Activate VAO
    glBindVertexArray(VAO);

    logger.info("GPU Transfer started");
    glGenBuffers(1, &VBO[VBO_INDEX::POSITION]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_INDEX::POSITION]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(Position), this->vertCoords, GL_STATIC_DRAW);

    // ! Providing size of three always is wastefull as coords can be two dimensional also.s
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Position), (void *)0);
    glEnableVertexAttribArray(0);

    if (glGetError() != GL_NO_ERROR)
        logger.error("OpenGL error after buffer setup");

    if (!indices || indexCount <= 0)
    {
        logger.info("Index Count or Indices not provided using array buffer mode");

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return;
    }

    this->logger.info("Using Element buffer mode as indices and indexCount are provided");

    this->indices = (GLint *)malloc(sizeof(GLint) * this->indexCount);
    memcpy(this->indices, indices, this->indexCount * sizeof(GLint));

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount * sizeof(GLint), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return;
}

void Mesh::setupColors(Color *colors, int colorCount)
{
    if (!colors || colorCount <= 0)
    {
        logger.error("Either color pointer or color count is zero");
        return;
    }

    logger.info("Setting up colors");
    this->colorCount = colorCount;
    this->colors = (Color *)malloc(sizeof(Color) * this->colorCount);

    if (!this->colors)
    {
        logger.fatal("Colors array allocation failed");
        return;
    }

    // Copy the input colors to our allocated memory
    memcpy(this->colors, colors, sizeof(Color) * this->colorCount);
    logger.info("Copied colors to memory");

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO[VBO_INDEX::COLOR]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_INDEX::COLOR]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * this->colorCount, this->colors, GL_STATIC_DRAW);

    // 4 components per color: r, g, b, a
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Color), (void *)0);
    glEnableVertexAttribArray(1);

    if (glGetError() != GL_NO_ERROR)
    {
        logger.error("OpenGL error after color buffer setup");
    }
    else
    {
        logger.success("Colors setup completed successfully");
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setupUV(UV *uvs, int uvCount)
{
    if (!uvs || uvCount <= 0)
    {
        logger.error("Either UV pointer or UV count is zero");
        return;
    }

    logger.info("Setting up UVs");
    this->uvCount = uvCount;
    this->uvs = (UV *)malloc(sizeof(UV) * this->uvCount);

    if (!this->uvs)
    {
        logger.fatal("UVs array allocation failed");
        return;
    }

    // Copy the input UVs to our allocated memory
    memcpy(this->uvs, uvs, sizeof(UV) * this->uvCount);
    logger.info("Copied UVs to memory");

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO[VBO_INDEX::UV_I]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_INDEX::UV_I]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UV) * this->uvCount, this->uvs, GL_STATIC_DRAW);

    // 2 components per UV: u, v
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(UV), (void *)0);
    glEnableVertexAttribArray(2);

    if (glGetError() != GL_NO_ERROR)
    {
        logger.error("OpenGL error after UV buffer setup");
    }
    else
    {
        logger.success("UVs setup completed successfully");
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setupNormals(Normal *normals, int normalCount)
{
    logger.info("Setup normals has started");
    if (!normals || normalCount <= 0)
    {
        logger.error("Either normal pointer or normal count is zero");
        return;
    }

    logger.info("Setting up normals");
    this->normalCount = normalCount;
    this->normals = (Normal *)malloc(sizeof(Normal) * this->normalCount);

    if (!this->normals)
    {
        logger.fatal("Normals array allocation failed");
        return;
    }

    memcpy(this->normals, normals, sizeof(Normal) * this->normalCount);
    logger.info("Copied normals to memory");

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO[VBO_INDEX::NORMAL]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_INDEX::NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * this->normalCount, this->normals, GL_STATIC_DRAW);

    // 3 components per Normal: x, y, z
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void *)0);
    glEnableVertexAttribArray(3);

    if (glGetError() != GL_NO_ERROR)
    {
        logger.error("OpenGL error after normal buffer setup");
    }
    else
    {
        logger.success("Normals setup completed successfully");
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw()
{
    if (this->vertexCount <= 0 && logWhileDrawDone)
    {
        logger.error("Vertex Coordinates not found");
        return;
    }

    if (logWhileDrawDone)
        logger.log("VAO Value: %d", VAO);

    glBindVertexArray(VAO);

    if (this->indexCount > 0 && indices)
    {
        if (logWhileDrawDone)
            logger.log("Drawing with Element buffer");
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        if (logWhileDrawDone)
            logger.log("Drawing with arrays");
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    logWhileDrawDone = false;
    glBindVertexArray(0);
}