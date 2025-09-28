#ifdef UNICODE
#undef UNICODE
#endif

#include <Windows.h>
#include "WndProc.hpp"
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int code)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    WNDCLASS wndCls = {0};

    wndCls.lpszClassName = "FragmentShader";
    wndCls.lpfnWndProc = WndProc;
    wndCls.hInstance = hInstance;
    wndCls.hbrBackground = CreateSolidBrush(RGB(35, 35, 35));
    wndCls.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wndCls))
    {
        MessageBox(NULL, "Registeration failed", "Unable to register window class", MB_OK);
        return -1;
    }

    HWND hWnd = CreateWindow(wndCls.lpszClassName, "Graphics Programming", WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_SYSMENU, 0, 0, 720, 480, NULL, NULL, NULL, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}