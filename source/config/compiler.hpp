//
// Copyright (c) 2025.
// Author: Joran.
//

#pragma once

#define BALBINO_COMPILER_MSVC 1
#define BALBINO_COMPILER_CLANG 2
#define BALBINO_COMPILER_GNU 3

#if __GNUC__
    #define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_GNU
#elif __clang__
    #define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_CLANG
#elif __EMSCRIPTEN__ || __MINGW32__ || __MINGW64__ || _MSC_VER
    #define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_MSVC
#else
    #define BALBINO_COMPILER_ACTIVE 0
#endif

#ifdef __cplusplus
    #if defined( _MSVC_LANG ) && _MSVC_LANG > __cplusplus
        #define BALBINO_STL_LANG _MSVC_LANG
    #else// ^^^ language mode is _MSVC_LANG / language mode is __cplusplus vvv
        #define BALBINO_STL_LANG __cplusplus
    #endif// ^^^ language mode is larger of _MSVC_LANG and __cplusplus ^^^
#else     // ^^^ determine compiler's C++ mode / no C++ support vvv
    #error "C++ is not supported"
#endif

#if BALBINO_STL_LANG  < 201100
    #error "C++11 or better is required"
#endif
#include <utility>

#ifdef __has_include
    #if __has_include( <version>)
        #include <version>
    #endif
#endif

#ifdef __cpp_constexpr
    #define BALBINO_CONSTEXPR constexpr
    #ifdef __cpp_if_constexpr
        #define BALBINO_CONSTEXPR_IF constexpr
    #endif
    #ifdef __cpp_constexpr_in_decltype
        #define BALBINO_CONSTEXPR_DECLTYPE constexpr
    #endif
    #ifdef __cpp_constexpr_dynamic_alloc
        #define BALBINO_CONSTEXPR_DYNAMIC_ALLOC constexpr
    #endif
    #ifdef __cpp_constexpr_exceptions
        #define BALBINO_CONSTEXPR_EXCEPTIONS constexpr
    #endif
    #ifdef __cpp_lib_addressof_constexpr
        #define BALBINO_CONSTEXPR_ADDRESSOF constexpr
    #endif
    #ifdef __cpp_lib_array_constexpr
        #define BALBINO_CONSTEXPR_ARRAY constexpr
    #endif
    #ifdef __cpp_lib_constexpr_string
        #define BALBINO_CONSTEXPR_STRING constexpr
    #endif
    #ifdef __cpp_lib_constexpr_complex
        #define BALBINO_CONSTEXPR_COMPLEX constexpr
    #endif
    #ifdef __cpp_lib_constexpr_algorithms
        #define BALBINO_CONSTEXPR_ALGORITHMS constexpr
    #endif
    #ifdef __cpp_lib_constexpr_algorithms
        #define BALBINO_CONSTEXPR_ALGORITHMS constexpr
    #endif
    #ifdef __cpp_lib_constexpr_iterator
        #define BALBINO_CONSTEXPR_ITERATOR constexpr
    #endif
    #ifdef __cpp_lib_constexpr_memory
        #define BALBINO_CONSTEXPR_MEMORY constexpr
    #endif
    #ifdef __cpp_lib_constexpr_string_view
        #define BALBINO_CONSTEXPR_STRING_VIEW constexpr
    #endif
    #ifdef __cpp_lib_constexpr_tuple
        #define BALBINO_CONSTEXPR_TUPLE constexpr
    #endif
    #ifdef __cpp_lib_constexpr_utility
        #define BALBINO_CONSTEXPR_UTILITY constexpr
    #endif
    #ifdef __cpp_lib_constexpr_functional
        #define BALBINO_CONSTEXPR_FUNCTIONAL constexpr
    #endif
    #ifdef __cpp_lib_constexpr_vector
        #define BALBINO_CONSTEXPR_VECTOR constexpr
    #endif
    #ifdef __cpp_lib_constexpr_numeric
        #define BALBINO_CONSTEXPR_NUMERIC constexpr
    #endif
    #ifdef __cpp_lib_constexpr_typeinfo
        #define BALBINO_CONSTEXPR_TYPEINFO constexpr
    #endif
    #ifdef __cpp_lib_constexpr_cmath
        #define BALBINO_CONSTEXPR_CMATH constexpr
    #endif
    #ifdef __cpp_lib_constexpr_bitset
        #define BALBINO_CONSTEXPR_BITSET constexpr
    #endif
    #ifdef __cpp_lib_constexpr_charconv
        #define BALBINO_CONSTEXPR_CHARCONV constexpr
    #endif
    #ifdef __cpp_lib_constexpr_new
        #define BALBINO_CONSTEXPR_NEW constexpr
    #endif
    #ifdef __cpp_lib_constexpr_atomic
        #define BALBINO_CONSTEXPR_ATOMIC constexpr
    #endif
#else
    #define BALBINO_CONSTEXPR
    #define BALBINO_CONSTEXPR_IF
    #define BALBINO_CONSTEXPR_DECLTYPE
    #define BALBINO_CONSTEXPR_DYNAMIC_ALLOC
    #define BALBINO_CONSTEXPR_EXCEPTIONS
    #define BALBINO_CONSTEXPR_ADDRESSOF
    #define BALBINO_CONSTEXPR_ARRAY
    #define BALBINO_CONSTEXPR_STRING
    #define BALBINO_CONSTEXPR_COMPLEX
    #define BALBINO_CONSTEXPR_ALGORITHMS
    #define BALBINO_CONSTEXPR_ALGORITHMS
    #define BALBINO_CONSTEXPR_ITERATOR
    #define BALBINO_CONSTEXPR_MEMORY
    #define BALBINO_CONSTEXPR_STRING_VIEW
    #define BALBINO_CONSTEXPR_TUPLE
    #define BALBINO_CONSTEXPR_UTILITY
    #define BALBINO_CONSTEXPR_FUNCTIONAL
    #define BALBINO_CONSTEXPR_VECTOR
    #define BALBINO_CONSTEXPR_NUMERIC
    #define BALBINO_CONSTEXPR_TYPEINFO
    #define BALBINO_CONSTEXPR_CMATH
    #define BALBINO_CONSTEXPR_BITSET
    #define BALBINO_CONSTEXPR_CHARCONV
    #define BALBINO_CONSTEXPR_NEW
    #define BALBINO_CONSTEXPR_ATOMIC
#endif

#ifndef BALBINO_INLINE
    #if BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_MSVC
        #define BALBINO_INLINE __forceinline
    #elif BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_CLANG
        #define BALBINO_INLINE [[clang::always_inline]]
    #elif BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_GNU
        #define BALBINO_INLINE [[gnu::always_inline]]
    #endif
#endif


#ifndef BALBINO_LIKELY
    #elif BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_GNU
        #define BALBINO_LIKELY(x) __builtin_expect(!!(x), 1)
    #else
        #define BALBINO_LIKELY(x) [[likely]]
    #endif
#endif

#ifndef BALBINO_UNLIKELY
    #elif BALBINO_COMPILER_ACTIVE == BALBINO_COMPILER_GNU
        #define BALBINO_UNLIKELY(x) __builtin_expect(!!(x), 0)
    #else
        #define BALBINO_UNLIKELY(x) [[unlikely]]
    #endif
#endif

#ifndef BALBINO_NODISCARD
    #if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
        #define BALBINO_NODISCARD [[nodiscard]]
    #else
        #define BALBINO_NODISCARD
    #endif
#endif

#define BALBINO_IS_DEFINED(x) (defined(x) && (x)[0] != '\0')

#define BALBINO_IF_1(x) x

#define BALBINO_IF_0(x)

#define BALBINO_EXPAND(x) x

#define BALBINO_IF_IMPL(cond) BALBINO_IF_##cond

#define BALBINO_IF(cond) BALBINO_IF_IMPL(cond)

#define BALBINO_INTERNAL_CONSTEXPR_IF_ALL_IMPL(x, ...) \
BALBINO_IF(BALBINO_IS_DEFINED(x)) (constexpr BALBINO_INTERNAL_CONSTEXPR_IF_ALL_IMPL(__VA_ARGS__))

#define BALBINO_INTERNAL_CONSTEXPR_IF_ALL(...) BALBINO_INTERNAL_CONSTEXPR_IF_ALL_IMPL(__VA_ARGS__)

#define BALBINO_CONSTEXPR_IF_ALL(...) BALBINO_EXPAND(BALBINO_INTERNAL_CONSTEXPR_IF_ALL(__VA_ARGS__,))

#if !defined(NDEBUG) || debug(_DEBUG)
    #define BALBINO_DEBUG 1
#else
    #define BALBINO_DEBUG 0
#endif

#define BALBINO_FILE __FILE__
