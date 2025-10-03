#include <Windows.h>
#include "glad/glad.h"
#include "PixelFormat.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"

#define TIMER_ID 1
#define FRAME_INTERVAL_MS 10 // ~60 FPS

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static HGLRC RenderingContext;

    static Logger logger("OPENGL");
    static Shader *shader = nullptr;
    static Mesh *mesh = nullptr;
    static Texture *texture;

    static int glSamplerLocation;
    static float time = 0.0f;

    // Cube with 8 vertices and 36 indices (6 faces * 2 triangles * 3 vertices)
    GLint indices[] = {
        // Front face
        0, 1, 2, 1, 3, 2,
        // Back face
        4, 5, 6, 5, 7, 6,
        // Left face
        4, 0, 6, 0, 2, 6,
        // Right face
        1, 5, 3, 5, 7, 3,
        // Top face
        4, 5, 0, 5, 1, 0,
        // Bottom face
        2, 3, 6, 3, 7, 6};

    Color colors[] = {
        {1.0f, 0.0f, 0.0f, 1.0f}, // 0: Red
        {0.0f, 1.0f, 0.0f, 1.0f}, // 1: Green
        {0.0f, 0.0f, 1.0f, 1.0f}, // 2: Blue
        {1.0f, 1.0f, 0.0f, 1.0f}, // 3: Yellow
        {1.0f, 0.0f, 1.0f, 1.0f}, // 4: Magenta
        {0.0f, 1.0f, 1.0f, 1.0f}, // 5: Cyan
        {1.0f, 0.5f, 0.0f, 1.0f}, // 6: Orange
        {0.5f, 0.0f, 1.0f, 1.0f}  // 7: Purple
    };
    // Cube centered at (1, 1, 1) with size (width=2, height=3, depth=2)
    // So, half extents: x ±1, y ±1.5, z ±1
    Position pointer[] = {
        // Front face (z = 11)
        {-1.0f, 1.0f, 5.0f},  // 0: Top-left-front
        {1.0f, 1.0f, 5.0f},   // 1: Top-right-front
        {-1.0f, -1.0f, 5.0f}, // 2: Bottom-left-front
        {1.0f, -1.0f, 5.0f},  // 3: Bottom-right-front
        // Back face (z = 9)
        {-1.0f, 1.0f, 2.0f},  // 4: Top-left-back
        {1.0f, 1.0f, 2.0f},   // 5: Top-right-back
        {-1.0f, -1.0f, 2.0f}, // 6: Bottom-left-back
        {1.0f, -1.0f, 2.0f}   // 7: Bottom-right-back
    };

    UV uv[] = {
        {0.0f, 1.0f}, // 0
        {1.0f, 1.0f}, // 1
        {0.0f, 0.0f}, // 2
        {1.0f, 0.0f}, // 3
        {0.0f, 1.0f}, // 4
        {1.0f, 1.0f}, // 5
        {0.0f, 0.0f}, // 6
        {1.0f, 0.0f}  // 7
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
        glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D

        texture = new Texture("./texture/wood.jpg");

        mesh = new Mesh(pointer, 8, indices, 36);
        mesh->setupColors(colors, 8);
        mesh->setupUV(uv, 8);

        shader = new Shader("./shader/vertex.vert", "./shader/fragment.frag");
        shader->useProgram();
        glUniform1i(shader->getUniformLocation("textureSampler"), 0);

        texture->Bind();

        SetTimer(hWnd, TIMER_ID, FRAME_INTERVAL_MS, NULL); // Start 60fps timer
        break;
    }
    case WM_TIMER:
    {
        if (wParam == TIMER_ID)
        {
            InvalidateRect(hWnd, NULL, FALSE); // Trigger WM_PAINT
        }
        break;
    }
    case WM_PAINT:
    {
        time += 0.010f; // Advance time by ~1/60s per frame
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (shader)
            shader->useProgram();
        if (mesh)
            mesh->draw();

        int timeSampler = shader->getUniformLocation("time");
        if (timeSampler == -1)
        {
            logger.error("Warning: uniform 'time' not found!\n");
        }
        else
        {
            glUniform1f(timeSampler, time);
        }

        SwapBuffers(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_SIZE:
    {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        glUniform1f(shader->getUniformLocation("resolutionX"), static_cast<float>(width));
        glUniform1f(shader->getUniformLocation("resolutionY"), static_cast<float>(height));

        if (width > 0 && height > 0)
            glViewport(0, 0, width, height);
        break;
    }
    case WM_CLOSE:
        KillTimer(hWnd, TIMER_ID); // Stop timer
        ReleaseDC(hWnd, hdc);
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
    {
        wglDeleteContext(RenderingContext);
        delete mesh;
        delete shader;
        delete texture;
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}