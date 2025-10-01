#include <Windows.h>
#include "glad/glad.h"
#include "PixelFormat.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static HGLRC RenderingContext;

    static Logger logger("OPENGL");
    static Shader *shader = nullptr;
    static Mesh *mesh = nullptr;

    GLuint *indices = nullptr;
    Vertex pointer[] = {Vertex(0.0f, 0.5f, 0.0f, 1.0f, 0.2f, 0.3f, 1.0f), Vertex(-0.5f, -.37f, 0.0f, 0.2f, 0.3f, 1.0f, 1.0f), Vertex(0.5f, -.37f, 0.0f, 0.23f, 0.65f, 0.12f, 1.0f)};

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

        mesh = new Mesh(pointer, 3, indices, 0);
        shader = new Shader("./shader/vertex.vert", "./shader/fragment.frag");
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