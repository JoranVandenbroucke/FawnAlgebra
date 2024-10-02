//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "compiler.hpp"

#ifdef BALBINO_DEBUG
#    define BALBINO_ASSERT(expr)                                                                                                                                                                                                               \
        if (!(expr))                                                                                                                                                                                                                           \
        {                                                                                                                                                                                                                                      \
            std::cerr << "Assertion failed: " #expr << std::endl;                                                                                                                                                                              \
            std::abort();                                                                                                                                                                                                                      \
        }
#else
#    define BALBINO_ASSERT(expr) ((void)0)
#endif

#define BALBINO_ERROR(message)                                                                                                                                                                                                                 \
    do                                                                                                                                                                                                                                         \
    {                                                                                                                                                                                                                                          \
        std::cerr << "Error: " << message << std::endl;                                                                                                                                                                                        \
        std::abort();                                                                                                                                                                                                                          \
    } while (0)
