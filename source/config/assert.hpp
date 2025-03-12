//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include <iostream>

#include "compiler.hpp"

inline void balbino_assert( const char* condition, const char* message, const char* fileline )
{
    std::cerr << "[" << fileline << "] "
              << "Assertion `" << condition << "` failed.\n"
              << message << std::endl;
    std::abort();
}

inline void balbino_assert( const char* condition, const std::string& message, const char* fileline )
{
    balbino_assert( condition, message.c_str(), fileline );
}

#ifdef BALBINO_DEBUG
    #define BALBINO_ASSERT( expr, message ) static_cast<bool>( expr ) ? static_cast<void>( 0 ): balbino_assert(#expr, message, BALBINO_FILE ":" _STRINGIZE(__LINE__))
#else
    #define BALBINO_ASSERT( expr, message ) ( static_cast<void>( 0 ) )
#endif
