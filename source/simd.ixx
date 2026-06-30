//
// Copyright (c) 2025.
// Author: Joran.
//

module;

export module FawnAlgebra:SIMD;
import std;

namespace fawn_algebra::simd::detail
{

template <typename Scalar, int Lanes>
struct raw_traits; // intentionally undefined for unsupported combos
#if defined(_MSC_VER) && !defined(__clang__)
#    define SIMD_DEFINE_RAW(SCALAR, LANES)                                                                                                                                         \
        template <>                                                                                                                                                                \
        struct raw_traits<SCALAR, LANES>                                                                                                                                           \
        {                                                                                                                                                                          \
            static constexpr int alignment = sizeof(SCALAR) * LANES <= 16 ? 16 : sizeof(SCALAR) * LANES <= 32 ? 32 : 64;                                                           \
                                                                                                                                                                                   \
            union __declspec(intrin_type) __declspec(align(16)) type                                                                                                               \
            {                                                                                                                                                                      \
                SCALAR data[LANES];                                                                                                                                                \
                std::uint8_t u8[sizeof(SCALAR) * LANES];                                                                                                                           \
                std::uint16_t u16[sizeof(SCALAR) * LANES / 2];                                                                                                                     \
                std::uint32_t u32[sizeof(SCALAR) * LANES / 4];                                                                                                                     \
                std::uint64_t u64[sizeof(SCALAR) * LANES / 8];                                                                                                                     \
                std::int8_t i8[sizeof(SCALAR) * LANES];                                                                                                                            \
                std::int16_t i16[sizeof(SCALAR) * LANES / 2];                                                                                                                      \
                std::int32_t i32[sizeof(SCALAR) * LANES / 4];                                                                                                                      \
                std::int64_t i64[sizeof(SCALAR) * LANES / 8];                                                                                                                      \
                float f32[sizeof(SCALAR) * LANES / 4];                                                                                                                             \
                double f64[sizeof(SCALAR) * LANES / 8];                                                                                                                            \
                                                                                                                                                                                   \
                constexpr type()                       = default;                                                                                                                  \
                constexpr type(const type&)            = default;                                                                                                                  \
                constexpr type& operator=(const type&) = default;                                                                                                                  \
                                                                                                                                                                                   \
                /* reinterpret-cast construction from any compatible raw type */                                                                                                   \
                template <typename Other>                                                                                                                                          \
                constexpr explicit type(const Other& o) noexcept                                                                                                                   \
                {                                                                                                                                                                  \
                    static_assert(sizeof(Other) == sizeof(type), "size mismatch");                                                                                                 \
                    std::memcpy(this, &o, sizeof(type));                                                                                                                           \
                }                                                                                                                                                                  \
                                                                                                                                                                                   \
                /* lane-list init: type{a,b,c,d} */                                                                                                                                \
                constexpr explicit type(std::initializer_list<SCALAR> il) noexcept                                                                                                 \
                {                                                                                                                                                                  \
                    std::ranges::copy(il, data);                                                                                                                                   \
                }                                                                                                                                                                  \
                                                                                                                                                                                   \
                /* scalar broadcast */                                                                                                                                             \
                constexpr explicit type(SCALAR s) noexcept                                                                                                                         \
                {                                                                                                                                                                  \
                    std::ranges::fill(data, s);                                                                                                                                    \
                }                                                                                                                                                                  \
                                                                                                                                                                                   \
                constexpr SCALAR operator[](int i) const noexcept                                                                                                                  \
                {                                                                                                                                                                  \
                    return data[i];                                                                                                                                                \
                }                                                                                                                                                                  \
                constexpr SCALAR& operator[](int i) noexcept                                                                                                                       \
                {                                                                                                                                                                  \
                    return data[i];                                                                                                                                                \
                }                                                                                                                                                                  \
                                                                                                                                                                                   \
                /* bitwise ops on the raw byte store */                                                                                                                            \
                constexpr type operator&(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < (int)sizeof(type); ++_i)                                                                                                                 \
                        r.u8[_i] = u8[_i] & rhs.u8[_i];                                                                                                                            \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator|(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < (int)sizeof(type); ++_i)                                                                                                                 \
                        r.u8[_i] = u8[_i] | rhs.u8[_i];                                                                                                                            \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator^(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < (int)sizeof(type); ++_i)                                                                                                                 \
                        r.u8[_i] = u8[_i] ^ rhs.u8[_i];                                                                                                                            \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator~() const noexcept                                                                                                                          \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < (int)sizeof(type); ++_i)                                                                                                                 \
                        r.u8[_i] = ~u8[_i];                                                                                                                                        \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                                                                                                                                                                                   \
                /* arithmetic (scalar loop; MSVC auto-vectorizes at /O2) */                                                                                                        \
                constexpr type operator+(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] + rhs.data[_i];                                                                                                                      \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator-(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] - rhs.data[_i];                                                                                                                      \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator*(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] * rhs.data[_i];                                                                                                                      \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator/(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] / rhs.data[_i];                                                                                                                      \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type& operator+=(const type& rhs) noexcept                                                                                                               \
                {                                                                                                                                                                  \
                    return *this = *this + rhs;                                                                                                                                    \
                }                                                                                                                                                                  \
                constexpr type& operator-=(const type& rhs) noexcept                                                                                                               \
                {                                                                                                                                                                  \
                    return *this = *this - rhs;                                                                                                                                    \
                }                                                                                                                                                                  \
                constexpr type& operator*=(const type& rhs) noexcept                                                                                                               \
                {                                                                                                                                                                  \
                    return *this = *this * rhs;                                                                                                                                    \
                }                                                                                                                                                                  \
                constexpr type& operator/=(const type& rhs) noexcept                                                                                                               \
                {                                                                                                                                                                  \
                    return *this = *this / rhs;                                                                                                                                    \
                }                                                                                                                                                                  \
                                                                                                                                                                                   \
                /* comparisons — return a same-sized type with all-1s/all-0s lanes */                                                                                              \
                /* mirroring GCC vector_size compare semantics */                                                                                                                  \
                constexpr type operator==(const type& rhs) const noexcept                                                                                                          \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] == rhs.data[_i] ? SCALAR(~0) : SCALAR(0);                                                                                            \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator!=(const type& rhs) const noexcept                                                                                                          \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] != rhs.data[_i] ? SCALAR(~0) : SCALAR(0);                                                                                            \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator<(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] < rhs.data[_i] ? SCALAR(~0) : SCALAR(0);                                                                                             \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator<=(const type& rhs) const noexcept                                                                                                          \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] <= rhs.data[_i] ? SCALAR(~0) : SCALAR(0);                                                                                            \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator>(const type& rhs) const noexcept                                                                                                           \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] > rhs.data[_i] ? SCALAR(~0) : SCALAR(0);                                                                                             \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                constexpr type operator>=(const type& rhs) const noexcept                                                                                                          \
                {                                                                                                                                                                  \
                    type r;                                                                                                                                                        \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        r.data[_i] = data[_i] >= rhs.data[_i] ? SCALAR(~0) : SCALAR(0);                                                                                            \
                    return r;                                                                                                                                                      \
                }                                                                                                                                                                  \
                                                                                                                                                                                   \
                constexpr explicit operator bool() const noexcept                                                                                                                  \
                {                                                                                                                                                                  \
                    for (int _i = 0; _i < LANES; ++_i)                                                                                                                             \
                        if (data[_i] != SCALAR(0))                                                                                                                                 \
                            return true;                                                                                                                                           \
                    return false;                                                                                                                                                  \
                }                                                                                                                                                                  \
            };                                                                                                                                                                     \
        };
#else
#    define SIMD_DEFINE_RAW(SCALAR, LANES)                                                                                                                                         \
        template <>                                                                                                                                                                \
        struct raw_traits<SCALAR, LANES>                                                                                                                                           \
        {                                                                                                                                                                          \
            static constexpr int alignment = sizeof(SCALAR) * LANES <= 16 ? 16 : sizeof(SCALAR) * LANES <= 32 ? 32 : 64;                                                           \
            using type                     = SCALAR __attribute__((__vector_size__(sizeof(SCALAR) * LANES), __aligned__(alignment)));                                              \
        };
#endif

SIMD_DEFINE_RAW(float, 4)
SIMD_DEFINE_RAW(float, 8)
SIMD_DEFINE_RAW(float, 16)
SIMD_DEFINE_RAW(double, 2)
SIMD_DEFINE_RAW(double, 4)
SIMD_DEFINE_RAW(double, 8)

SIMD_DEFINE_RAW(std::int8_t, 16)
SIMD_DEFINE_RAW(std::int8_t, 32)
SIMD_DEFINE_RAW(std::int16_t, 8)
SIMD_DEFINE_RAW(std::int16_t, 16)
SIMD_DEFINE_RAW(std::int32_t, 4)
SIMD_DEFINE_RAW(std::int32_t, 8)
SIMD_DEFINE_RAW(std::int32_t, 16)
SIMD_DEFINE_RAW(std::int64_t, 2)
SIMD_DEFINE_RAW(std::int64_t, 4)

SIMD_DEFINE_RAW(std::uint8_t, 16)
SIMD_DEFINE_RAW(std::uint8_t, 32)
SIMD_DEFINE_RAW(std::uint16_t, 8)
SIMD_DEFINE_RAW(std::uint16_t, 16)
SIMD_DEFINE_RAW(std::uint32_t, 4)
SIMD_DEFINE_RAW(std::uint32_t, 8)
SIMD_DEFINE_RAW(std::uint64_t, 2)
SIMD_DEFINE_RAW(std::uint64_t, 4)

#undef SIMD_DEFINE_RAW

export template <typename Scalar, int Lanes>
using raw = raw_traits<Scalar, Lanes>::type;

} // namespace fawn_algebra::simd::detail

namespace fawn_algebra::simd
{

// ---- raw vector type aliases -------------------------------------------

export using raw_f32x4 = detail::raw<float, 4>;
export using raw_f32x8 = detail::raw<float, 8>;
export using raw_f64x2 = detail::raw<double, 2>;
export using raw_f64x4 = detail::raw<double, 4>;

export using raw_i32x4 = detail::raw<std::int32_t, 4>;
export using raw_i32x8 = detail::raw<std::int32_t, 8>;
export using raw_i64x2 = detail::raw<std::int64_t, 2>;
export using raw_i64x4 = detail::raw<std::int64_t, 4>;
export using raw_i16x8 = detail::raw<std::int16_t, 8>;
export using raw_i8x16 = detail::raw<std::int8_t, 16>;

export using raw_u32x4 = detail::raw<std::uint32_t, 4>;
export using raw_u32x8 = detail::raw<std::uint32_t, 8>;
export using raw_u8x16 = detail::raw<std::uint8_t, 16>;
export using raw_u16x8 = detail::raw<std::uint16_t, 8>;

template <typename T, int N>
struct vec
{
    using scalar_type          = T;
    using raw_type             = detail::raw<T, N>;
    static constexpr int lanes = N;

    raw_type r{};

    // -- construction --------------------------------------------------

    constexpr vec()           = default;
    constexpr vec(const vec&) = default;
    constexpr vec(raw_type raw)
        : r(raw)
    {
    }

    // broadcast: vec<float,4>::splat(1.0f) -> {1,1,1,1}
    // NOTE: a loop doing out.r[i] = value is NOT usable in constexpr --
    // GCC vector_size types don't permit per-lane lvalue mutation in a
    // constant expression (read-by-index is fine, write-by-index isn't).
    // Build a fresh raw_type via the index_sequence trick instead, which
    // is pure construction, no mutation.
    static constexpr vec splat(T value)
    {
        return splat_impl(value, std::make_integer_sequence<int, N>{});
    }

  private:
    template <int... Is>
    static constexpr vec splat_impl(T value, std::integer_sequence<int, Is...>)
    {
        return vec{raw_type{(static_cast<void>(Is), value)...}};
    }

  public:
    // full lane list: vec<float,4>{1,2,3,4}
    template <typename... Args>
        requires(sizeof...(Args) == N) && (N > 1) && (std::is_convertible_v<Args, T> && ...)
    constexpr vec(Args... args)
        : r{static_cast<T>(args)...}
    {
    }

    constexpr operator raw_type() const
    {
        return r;
    }

    // -- element access ---------------------------------------------------

    constexpr T operator[](int i) const
    {
        return r[i];
    }
    constexpr T& operator[](int i)
    {
        return r[i];
    }

    static constexpr vec load(const T* p)
    {
        vec out;
        for (int i = 0; i < N; ++i)
            out.r[i] = p[i];
        return out;
    }

    constexpr void store(T* p) const
    {
        for (int i = 0; i < N; ++i)
            p[i] = r[i];
    }

    // -- arithmetic -------------------------------------------------------
    // Every operator just forwards to the builtin vector op: the compiler
    // (not us) is responsible for recognizing add/sub/mul/div and emitting
    // addps/subps/mulps/divps (or NEON/AVX equivalents) directly.
    friend constexpr vec operator+(vec a)
    {
        return a.r;
    }

    friend constexpr vec operator-(vec a)
    {
        return vec{raw_type(-a.r)};
    }

    friend constexpr vec operator+(vec a, vec b)
    {
        return vec{raw_type(a.r + b.r)};
    }
    friend constexpr vec operator-(vec a, vec b)
    {
        return vec{raw_type(a.r - b.r)};
    }
    friend constexpr vec operator*(vec a, vec b)
    {
        return vec{raw_type(a.r * b.r)};
    }
    friend constexpr vec operator/(vec a, vec b)
    {
        return vec{raw_type(a.r / b.r)};
    }

    friend constexpr vec& operator+=(vec& a, vec b)
    {
        a.r += b.r;
        return a;
    }
    friend constexpr vec& operator-=(vec& a, vec b)
    {
        a.r -= b.r;
        return a;
    }
    friend constexpr vec& operator*=(vec& a, vec b)
    {
        a.r *= b.r;
        return a;
    }
    friend constexpr vec& operator/=(vec& a, vec b)
    {
        a.r /= b.r;
        return a;
    }

    // scalar broadcast forms
    friend constexpr vec operator*(vec a, T s)
    {
        return a * vec::splat(s);
    }
    friend constexpr vec operator*(T s, vec a)
    {
        return a * vec::splat(s);
    }
    friend constexpr vec operator/(vec a, T s)
    {
        return a / vec::splat(s);
    }
    friend constexpr vec operator+(vec a, T s)
    {
        return a + vec::splat(s);
    }
    friend constexpr vec operator-(vec a, T s)
    {
        return a - vec::splat(s);
    }

    // -- comparisons --------------------------------------------------
    // GCC/Clang vector compares produce an all-1s/all-0s mask vector,
    // same lane count, signed-integer-sized-like-T. Exposed as-is for now;
    // a `mask<N>` wrapper with any()/all()/none() comes later.

    friend constexpr auto operator==(vec a, vec b)
    {
        return a.r == b.r;
    }
    friend constexpr auto operator!=(vec a, vec b)
    {
        return a.r != b.r;
    }
    friend constexpr auto operator<(vec a, vec b)
    {
        return a.r < b.r;
    }
    friend constexpr auto operator<=(vec a, vec b)
    {
        return a.r <= b.r;
    }
    friend constexpr auto operator>(vec a, vec b)
    {
        return a.r > b.r;
    }
    friend constexpr auto operator>=(vec a, vec b)
    {
        return a.r >= b.r;
    }
};

// ---- readable type aliases (struct wrapper, not raw) -------------------

export using f32x4 = vec<float, 4>;
export using f32x8 = vec<float, 8>;
export using f64x2 = vec<double, 2>;
export using f64x4 = vec<double, 4>;

export using i32x4 = vec<std::int32_t, 4>;
export using i32x8 = vec<std::int32_t, 8>;
export using u8x16 = vec<std::uint8_t, 16>;
export using i16x8 = vec<std::int16_t, 8>;

// ---- min / max / clamp --------------------------------------------------
// a < b ? a : b  compiles to a single minps/vminps -- GCC vector-extension
// ternary on vector_size types is recognized directly as a vector select.

export template <typename T, int N>
constexpr vec<T, N> min(vec<T, N> a, vec<T, N> b)
{
    return vec<T, N>{typename vec<T, N>::raw_type(a.r < b.r ? a.r : b.r)};
}

export template <typename T, int N>
constexpr vec<T, N> max(vec<T, N> a, vec<T, N> b)
{
    return vec<T, N>{typename vec<T, N>::raw_type(a.r > b.r ? a.r : b.r)};
}

// Verified: 2 instructions (vmaxps, vminps) -- the minimum possible.
export template <typename T, int N>
constexpr vec<T, N> clamp(vec<T, N> x, vec<T, N> lo, vec<T, N> hi)
{
    return max(lo, min(x, hi));
}

// ---- lerp / fma -----------------------------------------------------
// Verified: GCC auto-fuses a*b+c into a single vfmadd*ps when -mfma is
// available, at -O2 and above, with no special pragma needed. lerp
// compiles to vsubps + vfmadd231ps (2 instructions for 3 ops).

export template <typename T, int N>
constexpr vec<T, N> fma(vec<T, N> a, vec<T, N> b, vec<T, N> c)
{
    return vec<T, N>{typename vec<T, N>::raw_type(a.r * b.r + c.r)};
}

export template <typename T, int N>
constexpr vec<T, N> lerp(vec<T, N> a, vec<T, N> b, vec<T, N> t)
{
    return a + (b - a) * t;
}

// ---- abs --------------------------------------------------------------
// Float abs via sign-bit clear. Only valid for float/double; bitcast
// through the same-width unsigned integer raw type.

export constexpr f32x4 abs(f32x4 a)
{
    using u32_raw = detail::raw<std::uint32_t, 4>;
    constexpr u32_raw sign_mask{0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu};
    return f32x4{f32x4::raw_type(u32_raw(a.r) & sign_mask)};
}

export constexpr f32x8 abs(f32x8 a)
{
    using u32_raw = detail::raw<std::uint32_t, 8>;
    constexpr u32_raw sign_mask{0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu};
    return f32x8{f32x8::raw_type(u32_raw(a.r) & sign_mask)};
}

// ---- sqrt / rsqrt -------------------------------------------------------
// Dispatch to the best available instruction set at compile time based on
// macros GCC/Clang define automatically from -march/-msse/-mavx flags.
// No manual -D flags needed; the compiler sets these itself.
//
// If no SIMD target is detected we fall back to a scalar-per-lane loop,
// which is correct everywhere and still auto-vectorizes to whatever the
// compiler can figure out on its own -- just without the guarantee of a
// single vsqrtps instruction.
//
// rsqrt() is always an approximation (~12-bit precision). Follow with
// rsqrt_refine() if you need near-full float precision cheaply.

#if defined(__AVX__)
export inline f32x4 sqrt(f32x4 a)
{
    return f32x4{__builtin_ia32_sqrtps(a.r)};
}
export inline f32x4 rsqrt(f32x4 a)
{
    return f32x4{__builtin_ia32_rsqrtps(a.r)};
}
export inline f32x8 sqrt(f32x8 a)
{
    return f32x8{__builtin_ia32_sqrtps256(a.r)};
}
export inline f32x8 rsqrt(f32x8 a)
{
    return f32x8{__builtin_ia32_rsqrtps256(a.r)};
}

#elif defined(__SSE__)
export inline f32x4 sqrt(f32x4 a)
{
    return f32x4{__builtin_ia32_sqrtps(a.r)};
}
export inline f32x4 rsqrt(f32x4 a)
{
    return f32x4{__builtin_ia32_rsqrtps(a.r)};
}
// f32x8 has no SSE equivalent -- scalar fallback, still correct
export inline f32x8 sqrt(f32x8 a)
{
    return f32x8{std::sqrt(a[0]), std::sqrt(a[1]), std::sqrt(a[2]), std::sqrt(a[3]), std::sqrt(a[4]), std::sqrt(a[5]), std::sqrt(a[6]), std::sqrt(a[7])};
}
export inline f32x8 rsqrt(f32x8 a)
{
    return f32x8{1.f / std::sqrt(a[0]), 1.f / std::sqrt(a[1]), 1.f / std::sqrt(a[2]), 1.f / std::sqrt(a[3]),
                 1.f / std::sqrt(a[4]), 1.f / std::sqrt(a[5]), 1.f / std::sqrt(a[6]), 1.f / std::sqrt(a[7])};
}

#else
export inline f32x4 sqrt(f32x4 a)
{
    return f32x4{std::sqrt(a[0]), std::sqrt(a[1]), std::sqrt(a[2]), std::sqrt(a[3])};
}
export inline f32x4 rsqrt(f32x4 a)
{
    return f32x4{1.f / std::sqrt(a[0]), 1.f / std::sqrt(a[1]), 1.f / std::sqrt(a[2]), 1.f / std::sqrt(a[3])};
}
export inline f32x8 sqrt(f32x8 a)
{
    return f32x8{std::sqrt(a[0]), std::sqrt(a[1]), std::sqrt(a[2]), std::sqrt(a[3]), std::sqrt(a[4]), std::sqrt(a[5]), std::sqrt(a[6]), std::sqrt(a[7])};
}
export inline f32x8 rsqrt(f32x8 a)
{
    return f32x8{1.f / std::sqrt(a[0]), 1.f / std::sqrt(a[1]), 1.f / std::sqrt(a[2]), 1.f / std::sqrt(a[3]),
                 1.f / std::sqrt(a[4]), 1.f / std::sqrt(a[5]), 1.f / std::sqrt(a[6]), 1.f / std::sqrt(a[7])};
}
#endif

// One Newton-Raphson refinement step for rsqrt: turns the ~12-bit
// approximation into ~23-bit (near full float precision), still far
// cheaper than a real divide + sqrt. y_{n+1} = y_n * (1.5 - 0.5*x*y_n^2)
export template <typename V>
constexpr V rsqrt_refine(V x, V y)
{
    const auto half         = V::splat(typename V::scalar_type(0.5));
    const auto three_halves = V::splat(typename V::scalar_type(1.5));
    return y * (three_halves - (x * half) * (y * y));
}

// ---- horizontal reductions: dot / length ---------------------------
// No single-instruction horizontal sum exists on SSE/AVX2 for >2 lanes;
// this compiles to a vmulps + a shuffle/add reduction tree (verified:
// ~7 instructions for f32x4, which is at or near optimal for this ISA --
// NOT a sign something is wrong, just the real cost of a horizontal op).

export constexpr float dot(const f32x4 a, const f32x4 b)
{
    f32x4 p = a * b;
    return p[0] + p[1] + p[2] + p[3];
}

// NOTE: deliberately uses sqrt() (the raw vsqrtps-backed intrinsic) rather
// than __builtin_sqrtf/std::sqrt -- the latter goes through libm semantics
// (errno, domain checks) and was verified to emit a branch (vucomiss+ja)
// before the actual vsqrtss. Going through sqrt() keeps this branchless.
export inline float length(const f32x4 a)
{
    return sqrt(f32x4::splat(dot(a, a)))[0];
}
export constexpr float length_squared(const f32x4 a)
{
    return dot(a, a);
}

export inline f32x4 normalize(const f32x4 a)
{
    return a * f32x4::splat(1.0f / length(a));
}

// ---- select (branchless choose by mask) --------------------------------
// where(mask, a, b): per-lane choose. mask is the raw compare-result type
// (all-1s/all-0s per lane), as produced by vec's operator==/</> etc.

export template <typename T, int N>
constexpr vec<T, N> select(decltype(std::declval<vec<T, N>>().r < std::declval<vec<T, N>>().r) mask, vec<T, N> a, vec<T, N> b)
{
    return vec<T, N>{typename vec<T, N>::raw_type(mask ? a.r : b.r)};
}
} // namespace fawn_algebra::simd
