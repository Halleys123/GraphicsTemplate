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

struct Normal
{
    float x, y, z;

    Normal() : x(1.0f), y(1.0f), z(1.0f) {}
    Normal(float x, float y, float z) : x(x), y(y), z(z) {}

    Normal operator()() const { return *this; }
    Normal operator()(float nx, float ny, float nz) const { return Normal(nx, ny, nz); }
};

struct UV
{
    float u, v;

    UV() : u(1.0f), v(1.0f) {}
    UV(float u, float v) : u(u), v(v) {}

    UV operator()() const { return *this; }
    UV operator()(float nu, float nv) const { return UV(nu, nv); }
};

struct Color
{
    float r, g, b, a;

    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    Color operator()() const { return *this; }
    Color operator()(float nr, float ng, float nb, float na) const { return Color(nr, ng, nb, na); }
};

struct Position
{
    float x, y, z;

    Position() : x(0.0f), y(0.0f), z(0.0f) {}
    Position(float x, float y, float z) : x(x), y(y), z(z) {}

    Position operator()() const { return *this; }
    Position operator()(float nx, float ny, float nz) const { return Position(nx, ny, nz); }
};