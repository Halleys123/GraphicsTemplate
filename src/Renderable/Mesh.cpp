#include "glad/glad.h"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "stdlib.h"

int Mesh::MeshCount = 0;
Logger Mesh::logger = Logger();

Mesh::Mesh(Vertex *vertexSrc, int vertexCount, GLuint *indexSrc, int indexCount) : vertexCount(vertexCount), indexCount(indexCount)
{
    glGenVertexArrays(1, &VAO);

    MeshID = ++MeshCount;
    changeLoggerName();

    if (vertexCount && vertexSrc)
        setupVertex(vertexSrc, vertexCount);

    if (indexCount && indexSrc)
        setupIndices(indexSrc, indexCount);
}

void Mesh::setupVertex(Vertex *vertexSrc, int vertexCount)
{
    if (vertexCount <= 0 || !vertexSrc)
    {
        logger.error("Either vertex size or vertex pointer is not provided, skipping vertices");
        return;
    }
    if (VBO == 0)
        glGenBuffers(1, &VBO);

    logger.info("Allocating memory to vertices");
    vertices = (Vertex *)malloc(sizeof(Vertex) * vertexCount);
    if (!vertices)
    {
        logger.error("Memory allocation failed for vertices.");
        return;
    }

    logger.info("Vertices initialized with size of %d", vertexCount);
    memcpy(vertices, vertexSrc, sizeof(Vertex) * vertexCount);
    logger.success("Mesh data copied to vertices");

    // Send data to GPU
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertexSrc, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        char infoLog[512];
        GLsizei length = 0;
        // Try to get more info from OpenGL (if available)
        glGetProgramInfoLog(VBO, sizeof(infoLog), &length, infoLog);
        if (length > 0)
        {
            logger.error("OpenGL error %d during vertex setup: %s", err, infoLog);
        }
        else
        {
            logger.error("OpenGL error occurred during vertex setup: %d", err);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::setupIndices(GLuint *indexSrc, int indexCount)
{
    if (!indexCount || !indexSrc)
    {
        logger.log("Either Index size or index pointer is not provided, skipping indices");
        return;
    }

    if (EBO == -1)
        glGenBuffers(1, &EBO);

    logger.info("Allocating memory to indices");
    indices = (GLuint *)malloc(sizeof(GLuint) * indexCount);
    if (!indices)
    {
        logger.error("Memory allocation failed for indices.");
        return;
    }

    logger.info("Indices initialized with size of %d", indexCount);
    memcpy(indices, indexSrc, sizeof(GLuint) * indexCount);
    logger.success("Index data copied to indices");

    // Send data to GPu
    glBindVertexArray(EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount, indexSrc, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::changeLoggerName()
{
    char name[32];
    snprintf(name, sizeof(name), "MESH::%d", MeshID);

    logger.setLoggerName(name);
}

Mesh::~Mesh()
{
    free(vertices);
    free(indices);
}

void Mesh::draw()
{
    if (vertexCount == 0)
        return;
    glBindVertexArray(VAO);
    if (indexCount > 0)
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}