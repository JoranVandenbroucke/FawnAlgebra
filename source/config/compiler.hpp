//
// Copyright (c) 2025.
// Author: Joran.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <source_location>
#include <version>

namespace Balbino
{

// =============================================================================
// Compiler Detection
// =============================================================================

enum class compiler : std::uint8_t
{
    msvc,
    clang,
    gcc,
    unknown
};

consteval compiler DetectCompiler() noexcept
{
#if defined(__clang__)
    return compiler::clang;
#elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)
    return compiler::gcc;
#elif defined(_MSC_VER)
    return compiler::msvc;
#else
    return compiler::unknown;
#endif
}

inline constexpr compiler g_activeCompiler = DetectCompiler();

consteval bool IsClang() noexcept
{
    return g_activeCompiler == compiler::clang;
}
consteval bool IsGCC() noexcept
{
    return g_activeCompiler == compiler::gcc;
}
consteval bool IsMSVC() noexcept
{
    return g_activeCompiler == compiler::msvc;
}

// =============================================================================
// C++ Standard Version Detection
// =============================================================================

consteval int GetCppVersion() noexcept
{
#if defined(_MSVC_LANG)
    if constexpr (_MSVC_LANG > __cplusplus)
    {
        return _MSVC_LANG;
    }
    else
    {
        return __cplusplus;
    }
#else
    return __cplusplus;
#endif
}

inline constexpr int g_cppVersion = GetCppVersion();
inline constexpr int g_cpp20      = 202002L;
inline constexpr int g_cpp23      = 202302L;
inline constexpr int g_cpp26      = 202600L;

// Require C++20 minimum for modern codebases
static_assert(g_cppVersion >= g_cpp20, "C++20 or later is required");

consteval bool IsCpp20() noexcept
{
    return g_cppVersion >= g_cpp20;
}
consteval bool IsCpp23() noexcept
{
    return g_cppVersion >= g_cpp23;
}
consteval bool IsCpp26() noexcept
{
    return g_cppVersion >= g_cpp26;
}

// =============================================================================
// Feature Test Macros (C++20 and beyond)
// =============================================================================

// Core language features
#ifdef __cpp_concepts
inline constexpr bool g_hasConcepts = __cpp_concepts >= 201907L;
#else
inline constexpr bool g_hasConcepts = false;
#endif

#ifdef __cpp_modules
inline constexpr bool g_hasModules = __cpp_modules >= 201907L;
#else
inline constexpr bool g_hasModules = false;
#endif

#ifdef __cpp_coroutines
inline constexpr bool g_hasCoroutines = __cpp_coroutines >= 201902L;
#else
inline constexpr bool g_hasCoroutines = false;
#endif

#ifdef __cpp_constexpr
inline constexpr bool g_hasConstexpr20 = __cpp_constexpr >= 201907L;
#else
inline constexpr bool g_hasConstexpr20 = false;
#endif

#ifdef __cpp_consteval
inline constexpr bool g_hasConsteval = __cpp_consteval >= 201811L;
#else
inline constexpr bool g_hasConsteval = false;
#endif

#ifdef __cpp_constinit
inline constexpr bool g_hasConstinit = __cpp_constinit >= 201907L;
#else
inline constexpr bool g_hasConstinit = false;
#endif

#ifdef __cpp_designated_initializers
inline constexpr bool g_hasDesignatedInitializers = __cpp_designated_initializers >= 201707L;
#else
inline constexpr bool g_hasDesignatedInitializers = false;
#endif

// Library features
#ifdef __cpp_lib_concepts
inline constexpr bool g_hasLibConcepts = true;
#else
inline constexpr bool g_hasLibConcepts = false;
#endif

#ifdef __cpp_lib_ranges
inline constexpr bool g_hasRanges = true;
#else
inline constexpr bool g_hasRanges = false;
#endif

#ifdef __cpp_lib_coroutine
inline constexpr bool g_hasLibCoroutine = true;
#else
inline constexpr bool g_hasLibCoroutine = false;
#endif

#ifdef __cpp_lib_format
inline constexpr bool g_hasFormat = true;
#else
inline constexpr bool g_hasFormat = false;
#endif

#ifdef __cpp_lib_source_location
inline constexpr bool g_hasSourceLocation = true;
#else
inline constexpr bool g_hasSourceLocation = false;
#endif

#ifdef __cpp_lib_span
inline constexpr bool g_hasSpan = true;
#else
inline constexpr bool g_hasSpan = false;
#endif

#ifdef __cpp_lib_bit_cast
inline constexpr bool g_hasBitCast = true;
#else
inline constexpr bool g_hasBitCast = false;
#endif

#ifdef __cpp_lib_constexpr_string
inline constexpr bool g_hasConstexprString = true;
#else
inline constexpr bool g_hasConstexprString = false;
#endif

#ifdef __cpp_lib_constexpr_vector
inline constexpr bool g_hasConstexprVector = true;
#else
inline constexpr bool g_hasConstexprVector = false;
#endif

#ifdef __cpp_lib_constexpr_algorithms
inline constexpr bool g_hasConstexprAlgorithms = true;
#else
inline constexpr bool g_hasConstexprAlgorithms = false;
#endif

// =============================================================================
// Compiler Attributes (type-safe wrappers)
// =============================================================================

// Force inline
#if defined(_MSC_VER)
#    define BALBINO_FORCE_INLINE __forceinline
#    define BALBINO_NO_INLINE __declspec(noinline)
#elif defined(__clang__)
#    define BALBINO_FORCE_INLINE [[clang::always_inline]] inline
#    define BALBINO_NO_INLINE __attribute__((noinline))
#elif defined(__GNUC__)
#    define BALBINO_FORCE_INLINE [[gnu::always_inline]] inline
#    define BALBINO_NO_INLINE __attribute__((noinline))
#else
#    define BALBINO_FORCE_INLINE inline
#    define BALBINO_NO_INLINE
#endif

// No unique address (MSVC still needs special handling)
#if defined(_MSC_VER)
#    define BALBINO_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#    define BALBINO_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

// Modern C++20 attributes
#define BALBINO_NODISCARD [[nodiscard]]
#define BALBINO_MAYBE_UNUSED [[maybe_unused]]
#define BALBINO_LIKELY [[likely]]
#define BALBINO_UNLIKELY [[unlikely]]
#define BALBINO_NO_RETURN [[noreturn]]

// =============================================================================
// Debug Mode Detection
// =============================================================================

consteval bool IsDebugBuild() noexcept
{
#if !defined(NDEBUG) || defined(_DEBUG)
    return true;
#else
    return false;
#endif
}

inline constexpr bool g_isDebug   = IsDebugBuild();
inline constexpr bool g_isRelease = !g_isDebug;

// =============================================================================
// Source Location (C++20 replacement for __FILE__ and __LINE__)
// =============================================================================

struct SourceInfo
{
    const char* file;
    const char* function;
    unsigned line;
    unsigned column;

    consteval explicit SourceInfo(const std::source_location& loc = std::source_location::current()) noexcept
        : file(loc.file_name())
        , function(loc.function_name())
        , line(loc.line())
        , column(loc.column())
    {
    }
};

// Use like: MyLog(GetSourceInfo());
[[nodiscard]] consteval SourceInfo GetSourceInfo(const std::source_location& loc = std::source_location::current()) noexcept
{
    return SourceInfo{loc};
}

// =============================================================================
// Alignment Helpers
// =============================================================================

template <std::size_t Alignment>
concept ValidAlignment = (Alignment > 0) && ((Alignment & (Alignment - 1)) == 0);

template <std::size_t Alignment>
    requires ValidAlignment<Alignment>
struct alignas(Alignment) Aligned
{
};

// Usage: struct alignas(Aligned<64>::value) CacheLine { ... };
template <std::size_t Alignment>
    requires ValidAlignment<Alignment>
inline constexpr std::size_t g_alignedSize = alignof(Aligned<Alignment>);

// =============================================================================
// Compiler Version Info
// =============================================================================

struct CompilerVersion
{
    int major{};
    int minor{};
    int patch{};

    consteval CompilerVersion() noexcept
#if defined(__clang__)
        : major(__clang_major__)
        , minor(__clang_minor__)
        , patch(__clang_patchlevel__)
#elif defined(__GNUC__)
        : major(__GNUC__)
        , minor(__GNUC_MINOR__)
        , patch(__GNUC_PATCHLEVEL__)
#elif defined(_MSC_VER)
        : major(_MSC_VER / 100)
        , minor(_MSC_VER % 100)
        , patch(_MSC_FULL_VER % 100000)
#else
        : major(0)
        , minor(0)
        , patch(0)
#endif
    {
    }

    [[nodiscard]] consteval bool operator>=(const CompilerVersion& other) const noexcept
    {
        if (major != other.major)
        {
            return major >= other.major;
        }
        if (minor != other.minor)
        {
            return minor >= other.minor;
        }
        return patch >= other.patch;
    }
};

inline constexpr CompilerVersion g_currentCompilerVersion{};

// =============================================================================
// Platform-Specific Vector Extensions
// =============================================================================

#if defined(__clang__) || defined(__GNUC__)
template <typename T, size_t N>
using vector_type [[gnu::vector_size(sizeof(T) * N)]] = T;

inline constexpr bool g_hasVectorExtensions = true;
#else
inline constexpr bool g_hasVectorExtensions = false;
#endif

// =============================================================================
// Utility: Compile-time string for feature reporting
// =============================================================================

consteval const char* GetCompilerName() noexcept
{
    if constexpr (IsClang())
    {
        return "Clang";
    }
    else if constexpr (IsGCC())
    {
        return "GCC";
    }
    else if constexpr (IsMSVC())
    {
        return "MSVC";
    }
    else
    {
        return "Unknown";
    }
}

consteval const char* GetCppVersionString() noexcept
{
    if constexpr (IsCpp26())
    {
        return "C++26";
    }
    else if constexpr (IsCpp23())
    {
        return "C++23";
    }
    else if constexpr (IsCpp20())
    {
        return "C++20";
    }
    else
    {
        return "C++17 or older";
    }
}

} // namespace Balbino

// =============================================================================
// Legacy Macro Compatibility (minimize these in modern code)
// =============================================================================

#define BALBINO_COMPILER_MSVC 1
#define BALBINO_COMPILER_CLANG 2
#define BALBINO_COMPILER_GNU 3

#if defined(__clang__)
#    define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_CLANG
#elif defined(__GNUC__)
#    define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_GNU
#elif defined(_MSC_VER)
#    define BALBINO_COMPILER_ACTIVE BALBINO_COMPILER_MSVC
#else
#    define BALBINO_COMPILER_ACTIVE 0
#endif
