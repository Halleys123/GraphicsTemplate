#pragma once

struct Vertex
{
    float px, py, pz; // position
    float r, g, b, a; // colors
    float u, v;       // texture coords
    float nx, ny, nz; // normals

    Vertex(float px, float py, float pz) : px(px), py(py), pz(pz)
    {
        r = g = b = a = 1.0f;
        u = v = 1.0f;
        nx = ny = nz = 1.0f;
    }
    Vertex(float px, float py, float pz, float r, float g, float b, float a) : px(px), py(py), pz(pz)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;

        u = v = 1.0f;
        nx = ny = nz = 1.0f;
    }
};
