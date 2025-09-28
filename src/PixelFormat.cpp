#include <Windows.h>

void setPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),                              // Size fof PFD
        1,                                                          // DOn't know this property
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
        PFD_TYPE_RGBA,                                              // Pixel Type
        32,                                                         // Bits for colors excluding alpha but for memory allighment use 32 instead of 24
        0, 0, 0, 0, 0, 0,                                           // color bit and shift for rgb respectively
        0, 0,                                                       // alpha bit and shift
        0,                                                          // accumulation bits // QUESTION: Not sure what it is and how it affects
        0, 0, 0, 0,                                                 // accum rgba bits respectvily
        24,                                                         // Depth bits
        8,                                                          // Stencil bits
        0,                                                          // cAuxBuffers
        PFD_MAIN_PLANE,                                             // iLayerType
        0,                                                          // bReserved
        0, 0, 0                                                     // dwLayerMask, dwVisibleMask, dwDamageMask
    };
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);
}