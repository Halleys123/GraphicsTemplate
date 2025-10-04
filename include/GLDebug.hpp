#pragma once
#include "glad/glad.h"
#include <cstdio>

#define GL_CHECK(call)                                                   \
    do                                                                   \
    {                                                                    \
        call;                                                            \
        GLenum _e = glGetError();                                        \
        if (_e != GL_NO_ERROR)                                           \
        {                                                                \
            std::fprintf(stderr, "[GL_ERROR] 0x%04X after %s (%s:%d)\n", \
                         _e, #call, __FILE__, __LINE__);                 \
        }                                                                \
    } while (0)
