#pragma once

#include "Shader.hpp"
#include "Logger.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

class Renderable
{
    static int ID;
    static Logger logger;

    int instanceID = 0;

    Texture *texture;
    Shader *shader;
    Mesh *mesh;

public:
    Renderable();
    ~Renderable();
};