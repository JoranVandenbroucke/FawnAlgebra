//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

// architecture specific includes
#pragma once

#if defined(__MMX__)
#    define BALBINO_SIMD_MMX 1
#else
#    define BALBINO_SIMD_MMX 0
#endif

#if defined(__SSE__) || defined(__x86_64__)
#    define BALBINO_SIMD_SSE 1
#else
#    define BALBINO_SIMD_SSE 0
#endif

#if defined(__SSE2__) || defined(__x86_64__)
#    define BALBINO_SIMD_SSE2 1
#else
#    define BALBINO_SIMD_SSE2 0
#endif

#if defined(__SSE3__)
#    define BALBINO_SIMD_SSE3 1
#else
#    define BALBINO_SIMD_SSE3 0
#endif

#if defined(__SSSE3__)
#    define BALBINO_SIMD_SSSE3 1
#else
#    define BALBINO_SIMD_SSSE3 0
#endif

#if defined(__SSE4_1__)
#    define BALBINO_SIMD_SSE41 1
#else
#    define BALBINO_SIMD_SSE41 0
#endif

#if defined(__SSE4_2__)
#    define BALBINO_SIMD_SSE42 1
#else
#    define BALBINO_SIMD_SSE42 0
#endif

#if defined(__AVX__)
#    define BALBINO_SIMD_AVX 1
#else
#    define BALBINO_SIMD_AVX 0
#endif

#if defined(__AVX2__)
#    define BALBINO_SIMD_AVX2 1
#else
#    define BALBINO_SIMD_AVX2 0
#endif

#if defined(__FMA__)
#    define BALBINO_SIMD_FMA 1
#else
#    define BALBINO_SIMD_FMA 0
#endif

#if defined(__AVX512F__)
#    define BALBINO_SIMD_AVX512F 1
#else
#    define BALBINO_SIMD_AVX512F 0
#endif
