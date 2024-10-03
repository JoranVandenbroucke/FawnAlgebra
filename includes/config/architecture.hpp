//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

// architecture specific includes
#pragma once
#if defined(__MMX__)
#include <mmintrin.h>
#endif

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

#if defined(__SSE2__)
#include <emmintrin.h>
#endif

#if defined(__SSE3__)
#include <pmmintrin.h>
#endif

#if defined(__SSSE3__)
#include <tmmintrin.h>
#endif

#if defined(__SSE4_1__)
#include <smmintrin.h>
#endif

#if defined(__SSE4_2__)
#include <nmmintrin.h>
#endif

#if defined(__SSE4A__)
#include <ammintrin.h>
#endif

#if defined(__AES__)
#include <wmmintrin.h>
#endif

#if defined(__AVX__) || defined(__AVX2__) || defined(__FMA__)
#include <immintrin.h>
#endif

