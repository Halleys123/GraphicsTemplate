#include <Windows.h>
#include "glad/glad.h"
#include "PixelFormat.hpp"
#include "Logger.hpp"
#include "Shader.hpp"

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static HGLRC RenderingContext;

    static Logger logger("OPENGL");
    static Shader *shader = nullptr;

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

        shader = new Shader("./shader/vertex.vert", "./shader/fragment.frag");

        break;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
        wglDeleteContext(RenderingContext);
        (*shader).~Shader();
        delete shader;
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}