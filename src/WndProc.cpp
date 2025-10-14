#include <Windows.h>
#include "glad/glad.h"
#include "PixelFormat.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"
#include "Renderable.hpp"
#include "Matrix.hpp"

#define TIMER_ID 1
#define FRAME_INTERVAL_MS 10 // ~60 FPS

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int mouseX = 0.0f;
    static int mouseY = 0.0f;

    static HDC hdc;
    static HGLRC RenderingContext;

    static Logger logger("OPENGL");

    static Renderable *cube = nullptr;

    static float time = 0.0f;

    Position cubeVertices[] = {
        // Front face
        {-1.0f, 1.0f, 1.0f},  // 0
        {1.0f, 1.0f, 1.0f},   // 1
        {-1.0f, -1.0f, 1.0f}, // 2
        {1.0f, -1.0f, 1.0f},  // 3
        // Back face
        {-1.0f, 1.0f, -1.0f},  // 4
        {1.0f, 1.0f, -1.0f},   // 5
        {-1.0f, -1.0f, -1.0f}, // 6
        {1.0f, -1.0f, -1.0f},  // 7
        // Left face
        {-1.0f, 1.0f, -1.0f},  // 8
        {-1.0f, 1.0f, 1.0f},   // 9
        {-1.0f, -1.0f, -1.0f}, // 10
        {-1.0f, -1.0f, 1.0f},  // 11
        // Right face
        {1.0f, 1.0f, 1.0f},   // 12
        {1.0f, 1.0f, -1.0f},  // 13
        {1.0f, -1.0f, 1.0f},  // 14
        {1.0f, -1.0f, -1.0f}, // 15
        // Top face
        {-1.0f, 1.0f, -1.0f}, // 16
        {1.0f, 1.0f, -1.0f},  // 17
        {-1.0f, 1.0f, 1.0f},  // 18
        {1.0f, 1.0f, 1.0f},   // 19
        // Bottom face
        {-1.0f, -1.0f, 1.0f},  // 20
        {1.0f, -1.0f, 1.0f},   // 21
        {-1.0f, -1.0f, -1.0f}, // 22
        {1.0f, -1.0f, -1.0f}   // 23
    };
    Normal cubeNormals[] = {
        // Front face (pointing towards +Z)
        {0.0f, 0.0f, 1.0f}, // 0
        {0.0f, 0.0f, 1.0f}, // 1
        {0.0f, 0.0f, 1.0f}, // 2
        {0.0f, 0.0f, 1.0f}, // 3
        // Back face (pointing towards -Z)
        {0.0f, 0.0f, -1.0f}, // 4
        {0.0f, 0.0f, -1.0f}, // 5
        {0.0f, 0.0f, -1.0f}, // 6
        {0.0f, 0.0f, -1.0f}, // 7
        // Left face (pointing towards -X)
        {-1.0f, 0.0f, 0.0f}, // 8
        {-1.0f, 0.0f, 0.0f}, // 9
        {-1.0f, 0.0f, 0.0f}, // 10
        {-1.0f, 0.0f, 0.0f}, // 11
        // Right face (pointing towards +X)
        {1.0f, 0.0f, 0.0f}, // 12
        {1.0f, 0.0f, 0.0f}, // 13
        {1.0f, 0.0f, 0.0f}, // 14
        {1.0f, 0.0f, 0.0f}, // 15
        // Top face (pointing towards +Y)
        {0.0f, 1.0f, 0.0f}, // 16
        {0.0f, 1.0f, 0.0f}, // 17
        {0.0f, 1.0f, 0.0f}, // 18
        {0.0f, 1.0f, 0.0f}, // 19
        // Bottom face (pointing towards -Y)
        {0.0f, -1.0f, 0.0f}, // 20
        {0.0f, -1.0f, 0.0f}, // 21
        {0.0f, -1.0f, 0.0f}, // 22
        {0.0f, -1.0f, 0.0f}  // 23
    };
    UV cubeUVs[] = {
        // Front face
        {0.0f, 1.0f}, // 0
        {1.0f, 1.0f}, // 1
        {0.0f, 0.0f}, // 2
        {1.0f, 0.0f}, // 3
        // Back face
        {0.0f, 1.0f}, // 4
        {1.0f, 1.0f}, // 5
        {0.0f, 0.0f}, // 6
        {1.0f, 0.0f}, // 7
        // Left face
        {0.0f, 1.0f}, // 8
        {1.0f, 1.0f}, // 9
        {0.0f, 0.0f}, // 10
        {1.0f, 0.0f}, // 11
        // Right face
        {0.0f, 1.0f}, // 12
        {1.0f, 1.0f}, // 13
        {0.0f, 0.0f}, // 14
        {1.0f, 0.0f}, // 15
        // Top face
        {0.0f, 1.0f}, // 16
        {1.0f, 1.0f}, // 17
        {0.0f, 0.0f}, // 18
        {1.0f, 0.0f}, // 19
        // Bottom face
        {0.0f, 1.0f}, // 20
        {1.0f, 1.0f}, // 21
        {0.0f, 0.0f}, // 22
        {1.0f, 0.0f}  // 23
    };
    GLint cubeIndices[] = {
        // Front face
        0, 1, 2, 1, 3, 2,
        // Back face
        4, 5, 6, 5, 7, 6,
        // Left face
        8, 9, 10, 9, 11, 10,
        // Right face
        12, 13, 14, 13, 15, 14,
        // Top face
        16, 17, 18, 17, 19, 18,
        // Bottom face
        20, 21, 22, 21, 23, 22};

    static Matrix<float> mat;
    static Matrix<float> projection;

    Color cubeColors[24];
    for (int i = 0; i < 24; ++i)
        cubeColors[i] = {0.5f, 0.5f, 0.5f, 1.0f};

    GLint count[] = {24, 36, 24, 24, 24};

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

        mat[0][0] = 0.86602f;
        mat[0][1] = 0.5f;
        mat[0][3] = 1.0f;

        mat[1][0] = -0.5f;
        mat[1][1] = 0.86602f;
        mat[1][3] = 1.0f;

        mat[2][2] = 1.0f;
        mat[2][3] = -3.0f; // Move cube back in Z

        mat[3][3] = 1.0f;

        projection = Matrix<float>::perspective(50.0f, 1.6f, 1.0f, 100.0f);
        mat = mat.transpose();

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        cube = new Renderable(count, cubeVertices, cubeIndices, cubeColors, cubeUVs, cubeNormals, "./shader/vertex.vert", "./shader/fragment.frag", "./texture/wood.jpg");
        SetTimer(hWnd, TIMER_ID, FRAME_INTERVAL_MS, NULL);

        break;
    }
    case WM_TIMER:
    {
        if (wParam == TIMER_ID)
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    }

    case WM_LBUTTONDOWN:
    {
        mouseX = LOWORD(lParam);
        mouseY = HIWORD(lParam);
        return 0;
    }

    case WM_LBUTTONUP:
    {
        mouseX = 0.0f;
        mouseY = 0.0f;
        return 0;
    }

    case WM_PAINT:
    {
        time += 0.010f;
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->SetUniform<float>("model", mat.mat);
        cube->SetUniform<float>("projection", projection.mat);
        cube->SetUniform<float>("time", time);
        cube->Draw();

        SwapBuffers(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_SIZE:
    {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        if (cube)
        {
            cube->SetUniform<float>("resolutionX", static_cast<float>(width));
            cube->SetUniform<float>("resolutionY", static_cast<float>(height));
        }

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
        delete cube;
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}