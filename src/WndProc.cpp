#include <Windows.h>
#include "glad/glad.h"
#include "PixelFormat.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static HGLRC RenderingContext;

    static Logger logger("OPENGL");
    static Shader *shader = nullptr;
    static Mesh *mesh = nullptr;
    static Texture *texture;

    static int glSamplerLocation;

    GLint indices[] = {0, 1, 2, 1, 2, 3};
    Color colors[] = {
        {1.0f, 0.5f, 0.12f, 1.0f}, // Top-left
        {0.2f, 0.8f, 0.3f, 1.0f},  // Top-right
        {0.3f, 0.4f, 0.9f, 1.0f},  // Bottom-left
        {0.9f, 0.1f, 0.5f, 1.0f}   // Bottom-right
    };
    Position pointer[] = {
        {-0.5f, 0.5f, 0.0f},  // Top-left
        {0.5f, 0.5f, 0.0f},   // Top-right
        {-0.5f, -0.5f, 0.0f}, // Bottom-left
        {0.5f, -0.5f, 0.0f}   // Bottom-right
    };
    UV uv[] = {
        {0.0f, 1.0f}, // Top-left
        {1.0f, 1.0f}, // Top-right
        {0.0f, 0.0f}, // Bottom-left
        {1.0f, 0.0f}  // Bottom-right
    };

    switch (msg)
    {
    case WM_CREATE:
    {
        hdc = GetDC(hWnd);
        setPixelFormat(hdc);
        RenderingContext = wglCreateContext(hdc);
        wglMakeCurrent(hdc, RenderingContext);

        if (!gladLoadGL())
        {
            MessageBox(NULL, "Error initializing glad function pointers, Closing program", "Glad Error", MB_ICONERROR | MB_OK);
            SendMessage(hWnd, WM_DESTROY, 0, 0);
        }
        else
        {
            logger.info("OpenGL functions have initialized successfully");
        }

        RECT rc;
        GetClientRect(hWnd, &rc);
        glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

        texture = new Texture("./texture/wood.jpg");

        mesh = new Mesh(pointer, 4, indices, 6);
        mesh->setupColors(colors, 4);
        mesh->setupUV(uv, 4);

        shader = new Shader("./shader/vertex.vert", "./shader/fragment.frag");
        shader->useProgram();
        glSamplerLocation = glGetUniformLocation(shader->ShaderProgram, "textureSampler");
        glUniform1i(glSamplerLocation, 0);
        texture->Bind();
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (shader)
            shader->useProgram();
        if (mesh)
            mesh->draw();

        SwapBuffers(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_SIZE:
    {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        if (width > 0 && height > 0)
            glViewport(0, 0, width, height);
        break;
    }
    case WM_CLOSE:
        ReleaseDC(hWnd, hdc);
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
    {
        wglDeleteContext(RenderingContext);
        delete mesh;
        delete shader;

        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}