//
// Copyright (c) 2026.
// Author: Joran.
//

module;
#include "config/architecture.hpp"
#include "config/assert.hpp"
#include "config/compiler.hpp"

#if BALBINO_SIMD_SSE41 || BALBINO_SIMD_SSE42
#    include <smmintrin.h> // SSE4.1
#elif BALBINO_SIMD_SSE3
#    include <pmmintrin.h> // SSE3
#elif BALBINO_SIMD_SSE2
#    include <emmintrin.h> // SSE2
#elif BALBINO_SIMD_SSE
#    include <xmmintrin.h> // SSE
#endif

export module FawnAlgebra:Arithmetics;
import std;

namespace fawn_algebra
{
namespace Internal
{
float promote_impl(float);
double promote_impl(double);
double promote_impl(char);
double promote_impl(int);
double promote_impl(unsigned);
double promote_impl(long);
double promote_impl(unsigned long);
double promote_impl(long long);
double promote_impl(unsigned long long);

template <class... Args>
using promote_t = decltype((promote_impl(Args()) + ...));

template <typename Fp>
BALBINO_NODISCARD constexpr auto ilerp(Fp min, Fp max, Fp value) noexcept -> Fp
{
    const Fp delta = max - min;
    if (delta == 0)
    {
        return static_cast<Fp>(0); // Avoid division by zero, return 0 as default
    }

    // Clamp t to [0, 1] if x is outside [a, b]
    if (min < max && value < min)
    {
        return static_cast<Fp>(0);
    }
    if (min < max && value > max)
    {
        return static_cast<Fp>(1);
    }
    if (min > max && value > min)
    {
        return static_cast<Fp>(0);
    }
    if (min > max && value < max)
    {
        return static_cast<Fp>(1);
    }

    return (value - min) / delta;
}
} // namespace Internal

BALBINO_NODISCARD constexpr float ilerp(const float min, const float max, const float value) noexcept
{
    return Internal::ilerp(min, max, value);
}

BALBINO_NODISCARD constexpr double ilerp(const double min, const double max, const double value) noexcept
{
    return Internal::ilerp(min, max, value);
}

BALBINO_NODISCARD constexpr long double ilerp(const long double min, const long double max, const long double value) noexcept
{
    return Internal::ilerp(min, max, value);
}

template <class A1, class A2, class A3>
    requires(std::is_arithmetic_v<A1> && std::is_arithmetic_v<A2> && std::is_arithmetic_v<A3>)
BALBINO_NODISCARD constexpr Internal::promote_t<A1, A2, A3> ilerp(A1 min, A2 max, A3 value) noexcept
{
    using result_type = Internal::promote_t<A1, A2, A3>;
    static_assert(!(std::is_same_v<A1, result_type> && std::is_same_v<A2, result_type> && std::is_same_v<A3, result_type>));
    return ilerp(static_cast<result_type>(min), static_cast<result_type>(max), static_cast<result_type>(value));
}
// identical to std::sqrt
BALBINO_FORCE_INLINE float Sqrt(const float value) noexcept
{
    __m128 temp = _mm_set_ss(value);
    temp        = _mm_sqrt_ss(temp);
    return _mm_cvtss_f32(temp);
}

// faster than  1.0f/std::sqrt, but with little accuracy.
BALBINO_FORCE_INLINE float Rsqrt(const float value) noexcept
{
    __m128 temp = _mm_set_ss(value);
    temp        = _mm_rsqrt_ss(temp);
    return _mm_cvtss_f32(temp);
}

// identical to std::sqrt
BALBINO_NODISCARD BALBINO_FORCE_INLINE double Sqrt(const double value) noexcept
{
    __m128d temp = _mm_set_sd(value);
    temp         = _mm_sqrt_sd(temp, temp);
    return _mm_cvtsd_f64(temp);
}

// identical to 1.0 / std::sqrt
// .... there isn't an instruction for Rsqrt with double,
// so 1.0 / std::sqrt is the best you've got.
BALBINO_FORCE_INLINE double Rsqrt(const double value)
{
    __m128d temp = _mm_set_sd(value);
    temp         = _mm_div_sd(_mm_set_sd(1.0), _mm_sqrt_sd(temp, temp));
    return _mm_cvtsd_f64(temp);
}

inline constexpr float g_toRadians = std::numbers::pi_v<float> / 180.0F;
inline constexpr float g_toDegree  = 180.0F / std::numbers::pi_v<float>;

export template <typename T, std::uint8_t N>
struct Vec;

export template <typename T, std::uint8_t N>
struct VecSimd;

export template <typename T, std::uint8_t N>
struct Mat;

export template <typename T>
struct Quat;

export template <typename T>
struct QuatSimd;

template <typename T>
struct Vec<T, 2>
{
    using value_type = T;
    static constexpr size_t element_count{2};
    value_type x{};
    value_type y{};

    constexpr const value_type& operator[](const std::uint8_t idx) const
    {
        // assert(idx < element_count);
        return idx == 0 ? x : y;
    }

    constexpr value_type& operator[](const std::uint8_t idx)
    {
        // assert(idx < element_count);
        return idx == 0 ? x : y;
    }

    constexpr bool operator==(const Vec& other) const noexcept
    {
        return x == other.x && y == other.y;
    }

    constexpr auto operator<=>(const Vec& other) const noexcept
    {
        if (auto value{x <=> other.x}; value != 0)
        {
            return value;
        }
        return y <=> other.y;
    }

    constexpr Vec operator+() const noexcept
    {
        return *this;
    }
    constexpr Vec operator-() const noexcept
    {
        return {-x, -y};
    }
    constexpr Vec& operator+=(const Vec& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    constexpr Vec& operator-=(const Vec& other) noexcept
    {
        *this += -other;
        return *this;
    }
    constexpr Vec& operator*=(const Vec& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    constexpr Vec& operator/=(const Vec& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    friend constexpr Vec operator+(Vec lhs, const Vec& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Vec operator-(Vec lhs, const Vec& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr Vec operator*(Vec lhs, const Vec& rhs) noexcept
    {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr Vec operator/(Vec lhs, const Vec& rhs) noexcept
    {
        lhs /= rhs;
        return lhs;
    }

    constexpr Vec& operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    constexpr Vec& operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    friend constexpr Vec operator*(Vec lhs, T scalar) noexcept
    {
        lhs *= scalar;
        return lhs;
    }
    friend constexpr Vec operator*(T scalar, Vec rhs) noexcept
    {
        rhs *= scalar;
        return rhs;
    }
    friend constexpr Vec operator/(Vec lhs, T scalar) noexcept
    {
        lhs /= scalar;
        return lhs;
    }

    BALBINO_NODISCARD constexpr value_type Dot(const Vec& rhs) const noexcept
    {
        return x * rhs.x + y * rhs.y;
    }
    BALBINO_NODISCARD constexpr Vec Rotate90() const noexcept
    {
        return Vec{y, -x};
    }
    static constexpr Vec Rotate90(Vec vec) noexcept
    {
        return vec.Rotate90();
    }
    static constexpr value_type Dot(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Dot(rhs);
    }
    BALBINO_NODISCARD constexpr value_type Cross(const Vec& rhs) const noexcept
    {
        return x * rhs.y - y * rhs.x;
    }
    static constexpr value_type Cross(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Cross(rhs);
    }
    BALBINO_NODISCARD constexpr value_type LengthSqr() const noexcept
    {
        return Dot(*this);
    }
    static constexpr value_type LengthSqr(const Vec& val) noexcept
    {
        return val.LengthSqr();
    }
    BALBINO_NODISCARD constexpr auto Length() const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        return std::sqrt(static_cast<promo_type>(LengthSqr()));
    }
    static constexpr value_type Length(const Vec& val) noexcept
    {
        return val.Length();
    }
    BALBINO_NODISCARD constexpr value_type DistanceSqr(const Vec& to) const noexcept
    {
        const Vec dist = to - *this;
        return Dot(dist, dist);
    }
    static constexpr value_type DistanceSqr(const Vec& from, const Vec& to) noexcept
    {
        return from.DistanceSqr(to);
    }
    BALBINO_NODISCARD constexpr auto Distance(const Vec& to) const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        return std::sqrt(static_cast<promo_type>(DistanceSqr(to)));
    }
    static constexpr value_type Distance(const Vec& from, const Vec& to) noexcept
    {
        return from.Distance(to);
    }

    BALBINO_NODISCARD constexpr auto Normalize() const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        promo_type len   = static_cast<promo_type>(1) / Length();
        return *this * len;
    }
    static constexpr Vec Normalize(const Vec& val) noexcept
    {
        return val.Normalize();
    }
    BALBINO_NODISCARD constexpr auto NormalizeSafe() const noexcept
    {
        auto len          = Length();
        using length_type = std::decay_t<decltype(len)>;
        if (len < static_cast<length_type>(1E-6))
        {
            return *this;
        }
        return (*this / len);
    }
    static constexpr auto NormalizeSafe(const Vec& val) noexcept
    {
        return val.NormalizeSafe();
    }
    constexpr auto Angle() const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        return std::atan2(static_cast<promo_type>(y), static_cast<promo_type>(x));
    }
    static constexpr auto Angle(const Vec& val) noexcept
    {
        return val.Angle();
    }
    constexpr auto AngleBetween(const Vec& other) const noexcept
    {
        return (*this - other).Angle();
    }
    static constexpr auto Angle(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Angle(rhs);
    }

    [[nodiscard]] constexpr Vec Project(const Vec& other) const noexcept
    {
        return (Dot(other) / other.Dot(other)) * other;
    }

    static constexpr Vec Project(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Project(rhs);
    }

    [[nodiscard]] constexpr Vec Reject(const Vec& other) const noexcept
    {
        return *this - Project(other);
    }

    static constexpr Vec Reject(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Project(rhs);
    }

    static constexpr auto Round(const Vec& value) noexcept
    {
        return Vec{std::round(value.x), std::round(value.y)};
    }

    static constexpr auto Floor(const Vec& value) noexcept
    {
        return Vec{std::floor(value.x), std::floor(value.y)};
    }

    static constexpr Vec Ceil(const Vec& value) noexcept
    {
        return Vec{std::ceil(value.x), std::ceil(value.y)};
    }

    static constexpr Vec Min(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Vec{std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
    }
    static constexpr Vec Max(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Vec{std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
    }
    static constexpr Vec Clamp(const Vec& lhs, const Vec& min, const Vec& max) noexcept
    {
        return Vec{std::clamp(lhs.x, min.x, max.x), std::clamp(lhs.y, min.y, max.y)};
    }
    static constexpr auto Lerp(const Vec& start, const Vec& end, const Vec& delta) noexcept
    {
        using return_type = Internal::promote_t<value_type>;
        return Vec<return_type, element_count>{std::lerp(start.x, end.x, delta.x), std::lerp(start.y, end.y, delta.y)};
    }
    static constexpr auto ILerp(const Vec& start, const Vec& end, const Vec& value) noexcept
    {
        using return_type = Internal::promote_t<value_type>;
        return Vec<return_type, element_count>{ilerp(start.x, end.x, value.x), ilerp(start.y, end.y, value.y)};
    }
    static constexpr auto Remap(const Vec& value, const Vec& from, const Vec& to, const Vec& min, const Vec& max) noexcept
    {
        return Lerp(from, to, ILerp(min, max, value));
    }

    friend constexpr auto operator|(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Dot(lhs, rhs);
    }

    friend constexpr auto operator^(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Cross(lhs, rhs);
    }

    friend constexpr auto operator&(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Project(lhs, rhs);
    }

    friend constexpr auto operator%(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Reject(lhs, rhs);
    }
};

export using uchar2  = Vec<std::uint8_t, 2>;
export using char2   = Vec<std::uint8_t, 2>;
export using ushort2 = Vec<std::uint16_t, 2>;
export using short2  = Vec<std::uint16_t, 2>;
export using uint2   = Vec<std::uint32_t, 2>;
export using int2    = Vec<std::uint32_t, 2>;
export using float2  = Vec<float, 2>;
export using double2 = Vec<double, 2>;

#pragma region Vec2
// Basic type traits
static_assert(std::is_trivially_copyable_v<float2>);
static_assert(std::is_standard_layout_v<float2>);
static_assert(sizeof(float2) == sizeof(float) * 2);
static_assert(alignof(float2) == alignof(float));

// Constexpr construction and initialization
static_assert(float2{} == float2{0.0F, 0.0F});
static_assert(float2{1.0F, 2.0F}.x == 1.0F);
static_assert(float2{1.0F, 2.0F}.y == 2.0F);

// Element access
static_assert(float2{3.0F, 4.0F}[0] == 3.0F);
static_assert(float2{3.0F, 4.0F}[1] == 4.0F);

// Comparison operators
static_assert(float2{1.0F, 2.0F} == float2{1.0F, 2.0F});
static_assert(float2{1.0F, 2.0F} != float2{2.0F, 1.0F});
static_assert(float2{1.0F, 2.0F} < float2{2.0F, 1.0F});
static_assert(float2{1.0F, 2.0F} < float2{1.0F, 3.0F});
static_assert(float2{2.0F, 1.0F} > float2{1.0F, 2.0F});

// Unary operators
static_assert(+float2{1.0F, 2.0F} == float2{1.0F, 2.0F});
static_assert(-float2{1.0F, 2.0F} == float2{-1.0F, -2.0F});
static_assert(-float2{-3.0F, 4.0F} == float2{3.0F, -4.0F});

// Vector addition
static_assert(float2{1.0F, 2.0F} + float2{3.0F, 4.0F} == float2{4.0F, 6.0F});
static_assert(float2{5.0F, -2.0F} + float2{-3.0F, 7.0F} == float2{2.0F, 5.0F});

// Vector subtraction
static_assert(float2{5.0F, 8.0F} - float2{2.0F, 3.0F} == float2{3.0F, 5.0F});
static_assert(float2{1.0F, 1.0F} - float2{1.0F, 1.0F} == float2{0.0F, 0.0F});

// Component-wise multiplication
static_assert(float2{2.0F, 3.0F} * float2{4.0F, 5.0F} == float2{8.0F, 15.0F});
static_assert(float2{-2.0F, 3.0F} * float2{4.0F, -5.0F} == float2{-8.0F, -15.0F});

// Component-wise division
static_assert(float2{8.0F, 15.0F} / float2{2.0F, 3.0F} == float2{4.0F, 5.0F});
static_assert(float2{10.0F, 20.0F} / float2{5.0F, 4.0F} == float2{2.0F, 5.0F});

// Scalar multiplication
static_assert(float2{2.0F, 3.0F} * 2.0F == float2{4.0F, 6.0F});
static_assert(3.0F * float2{2.0F, 4.0F} == float2{6.0F, 12.0F});
static_assert(float2{1.0F, -2.0F} * -2.0F == float2{-2.0F, 4.0F});

// Scalar division
static_assert(float2{10.0F, 20.0F} / 2.0F == float2{5.0F, 10.0F});
static_assert(float2{6.0F, -9.0F} / 3.0F == float2{2.0F, -3.0F});

// Dot product
static_assert(float2{1.0F, 2.0F}.Dot(float2{3.0F, 4.0F}) == 11.0F);          // 1*3 + 2*4 = 11
static_assert(float2::Dot(float2{2.0F, 3.0F}, float2{4.0F, 5.0F}) == 23.0F); // 2*4 + 3*5 = 23
static_assert(float2{1.0F, 0.0F}.Dot(float2{0.0F, 1.0F}) == 0.0F);           // perpendicular

// Cross product (2D returns scalar)
static_assert(float2{1.0F, 0.0F}.Cross(float2{0.0F, 1.0F}) == 1.0F);
static_assert(float2{0.0F, 1.0F}.Cross(float2{1.0F, 0.0F}) == -1.0F);
static_assert(float2{2.0F, 3.0F}.Cross(float2{4.0F, 5.0F}) == -2.0F);          // 2*5 - 3*4 = -2
static_assert(float2::Cross(float2{3.0F, 1.0F}, float2{2.0F, 4.0F}) == 10.0F); // 3*4 - 1*2 = 10

// Length squared
static_assert(float2{3.0F, 4.0F}.LengthSqr() == 25.0F); // 3² + 4² = 25
static_assert(float2{1.0F, 0.0F}.LengthSqr() == 1.0F);
static_assert(float2{0.0F, 0.0F}.LengthSqr() == 0.0F);
static_assert(float2::LengthSqr(float2{5.0F, 12.0F}) == 169.0F); // 5² + 12² = 169

// Distance squared
static_assert(float2{0.0F, 0.0F}.DistanceSqr(float2{3.0F, 4.0F}) == 25.0F);
static_assert(float2{1.0F, 1.0F}.DistanceSqr(float2{4.0F, 5.0F}) == 25.0F); // (4-1)² + (5-1)² = 25
static_assert(float2::DistanceSqr(float2{0.0F, 0.0F}, float2{5.0F, 12.0F}) == 169.0F);

// Min/Max
static_assert(float2::Min(float2{1.0F, 5.0F}, float2{3.0F, 2.0F}) == float2{1.0F, 2.0F});
static_assert(float2::Max(float2{1.0F, 5.0F}, float2{3.0F, 2.0F}) == float2{3.0F, 5.0F});
static_assert(float2::Min(float2{-1.0F, -5.0F}, float2{-3.0F, -2.0F}) == float2{-3.0F, -5.0F});

// Clamp
static_assert(float2::Clamp(float2{5.0F, 15.0F}, float2{0.0F, 0.0F}, float2{10.0F, 10.0F}) == float2{5.0F, 10.0F});
static_assert(float2::Clamp(float2{-5.0F, 5.0F}, float2{0.0F, 0.0F}, float2{10.0F, 10.0F}) == float2{0.0F, 5.0F});
static_assert(float2::Clamp(float2{5.0F, 5.0F}, float2{0.0F, 0.0F}, float2{10.0F, 10.0F}) == float2{5.0F, 5.0F});

// Lerp
static_assert(float2::Lerp(float2{0.0F, 0.0F}, float2{10.0F, 20.0F}, float2{0.5F, 0.5F}) == float2{5.0F, 10.0F});
static_assert(float2::Lerp(float2{0.0F, 0.0F}, float2{10.0F, 20.0F}, float2{0.0F, 0.0F}) == float2{0.0F, 0.0F});
static_assert(float2::Lerp(float2{0.0F, 0.0F}, float2{10.0F, 20.0F}, float2{1.0F, 1.0F}) == float2{10.0F, 20.0F});

// Compound assignment operators preserve type
static_assert(std::is_same_v<decltype(std::declval<float2&>() += std::declval<float2>()), float2&>);
static_assert(std::is_same_v<decltype(std::declval<float2&>() -= std::declval<float2>()), float2&>);
static_assert(std::is_same_v<decltype(std::declval<float2&>() *= std::declval<float2>()), float2&>);
static_assert(std::is_same_v<decltype(std::declval<float2&>() /= std::declval<float2>()), float2&>);
static_assert(std::is_same_v<decltype(std::declval<float2&>() *= std::declval<float>()), float2&>);
static_assert(std::is_same_v<decltype(std::declval<float2&>() /= std::declval<float>()), float2&>);

// Binary operators return value type
static_assert(std::is_same_v<decltype(std::declval<float2>() + std::declval<float2>()), float2>);
static_assert(std::is_same_v<decltype(std::declval<float2>() - std::declval<float2>()), float2>);
static_assert(std::is_same_v<decltype(std::declval<float2>() * std::declval<float2>()), float2>);
static_assert(std::is_same_v<decltype(std::declval<float2>() / std::declval<float2>()), float2>);

// Noexcept guarantees
static_assert(noexcept(float2{} + float2{}));
static_assert(noexcept(float2{} - float2{}));
static_assert(noexcept(float2{} * float2{}));
static_assert(noexcept(float2{} / float2{}));
static_assert(noexcept(float2{}.Dot(float2{})));
static_assert(noexcept(float2{}.Cross(float2{})));
static_assert(noexcept(float2{}.LengthSqr()));
static_assert(noexcept(float2{} == float2{}));
static_assert(noexcept(float2{} <=> float2{}));
#pragma endregion Vec2

template <typename T>
struct Vec<T, 3>
{
    using value_type                      = T;
    static constexpr size_t element_count = 3;
    value_type x{};
    value_type y{};
    value_type z{};

    constexpr const value_type& operator[](const std::uint8_t idx) const noexcept
    {
        // assert(idx < element_count);
        return idx != 0 ? idx != 1 ? z : y : x;
    }

    constexpr value_type& operator[](const std::uint8_t idx) noexcept
    {
        // assert(idx < element_count);
        return idx != 0 ? idx != 1 ? z : y : x;
    }

    constexpr bool operator==(const Vec& other) const noexcept
    {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr auto operator<=>(const Vec& other) const noexcept
    {
        if (auto value{x <=> other.x}; value != 0)
        {
            return value;
        }
        if (auto value{y <=> other.y}; value != 0)
        {
            return value;
        }
        return z <=> other.z;
    }

    constexpr Vec operator+() const noexcept
    {
        return *this;
    }
    constexpr Vec operator-() const noexcept
    {
        return {-x, -y, -z};
    }
    constexpr Vec& operator+=(const Vec& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    constexpr Vec& operator-=(const Vec& other) noexcept
    {
        *this += -other;
        return *this;
    }
    constexpr Vec& operator*=(const Vec& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    constexpr Vec& operator/=(const Vec& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    friend constexpr Vec operator+(Vec lhs, const Vec& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Vec operator-(Vec lhs, const Vec& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr Vec operator*(Vec lhs, const Vec& rhs) noexcept
    {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr Vec operator/(Vec lhs, const Vec& rhs) noexcept
    {
        lhs /= rhs;
        return lhs;
    }

    constexpr Vec& operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    constexpr Vec& operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    friend constexpr Vec operator*(Vec lhs, T scalar) noexcept
    {
        lhs *= scalar;
        return lhs;
    }
    friend constexpr Vec operator*(T scalar, Vec rhs) noexcept
    {
        rhs *= scalar;
        return rhs;
    }
    friend constexpr Vec operator/(Vec lhs, T scalar) noexcept
    {
        lhs /= scalar;
        return lhs;
    }

    [[nodiscard]] constexpr value_type Dot(const Vec& rhs) const noexcept
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }
    [[nodiscard]] constexpr Vec Abs() const
    {
        return Vec{std::abs(x), std::abs(y), std::abs(z)};
    }
    static constexpr Vec Abs(const Vec& value)
    {
        return value.Abs();
    }
    static constexpr value_type Dot(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Dot(rhs);
    }
    [[nodiscard]] constexpr Vec Cross(const Vec& other) const noexcept
    {
        return Vec{y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }
    static constexpr Vec Cross(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Cross(rhs);
    }
    [[nodiscard]] constexpr value_type LengthSqr() const noexcept
    {
        return Dot(*this);
    }
    static constexpr value_type LengthSqr(const Vec& val) noexcept
    {
        return val.LengthSqr();
    }
    [[nodiscard]] constexpr Internal::promote_t<value_type> Length() const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        return std::sqrt(static_cast<promo_type>(LengthSqr()));
    }
    static constexpr auto Length(const Vec& val) noexcept
    {
        return val.Length();
    }
    [[nodiscard]] constexpr value_type DistanceSqr(const Vec& to) const noexcept
    {
        const Vec dist = to - *this;
        return Dot(dist, dist);
    }
    static constexpr value_type DistanceSqr(const Vec& from, const Vec& to) noexcept
    {
        return from.DistanceSqr(to);
    }
    [[nodiscard]] constexpr auto Distance(const Vec& to) const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        return std::sqrt(static_cast<promo_type>(DistanceSqr(to)));
    }
    static constexpr value_type Distance(const Vec& from, const Vec& to) noexcept
    {
        return from.Distance(to);
    }

    [[nodiscard]] constexpr Vec Normalize() const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        promo_type len{Sqrt(Length())};
        return *this / len;
    }
    static constexpr Vec Normalize(const Vec& val) noexcept
    {
        return val.Normalize();
    }
    [[nodiscard]] constexpr Vec NormalizeSafe() const noexcept
    {
        auto lenSqr{LengthSqr()};
        using length_type = std::decay_t<decltype(lenSqr)>;
        if (lenSqr < static_cast<length_type>(1E-6))
        {
            return *this;
        }
        return (*this / Sqrt(lenSqr));
    }
    static constexpr auto NormalizeSafe(const Vec& val) noexcept
    {
        return val.NormalizeSafe();
    }

    [[nodiscard]] constexpr Vec Project(const Vec& other) const noexcept
    {
        return (Dot(other) / other.Dot(other)) * other;
    }

    static constexpr Vec Project(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Project(rhs);
    }

    [[nodiscard]] constexpr Vec Reject(const Vec& other) const noexcept
    {
        return *this - Project(other);
    }

    static constexpr Vec Reject(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Project(rhs);
    }

    static constexpr Vec Min(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Vec{std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z)};
    }
    static constexpr Vec Max(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Vec{std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z)};
    }

    static constexpr auto Round(const Vec& value) noexcept
    {
        return Vec{std::round(value.x), std::round(value.y), std::round(value.z)};
    }

    static constexpr auto Floor(const Vec& value) noexcept
    {
        return Vec{std::floor(value.x), std::floor(value.y), std::floor(value.z)};
    }

    static constexpr Vec Ceil(const Vec& value) noexcept
    {
        return Vec{std::ceil(value.x), std::ceil(value.y), std::ceil(value.z)};
    }

    static constexpr Vec Clamp(const Vec& lhs, const Vec& min, const Vec& max) noexcept
    {
        return Vec{std::clamp(lhs.x, min.x, max.x), std::clamp(lhs.y, min.y, max.y), std::clamp(lhs.z, min.z, max.z)};
    }

    static constexpr auto Lerp(const Vec& start, const Vec& end, const Vec& delta) noexcept
    {
        using return_type = Internal::promote_t<value_type>;
        return Vec<return_type, element_count>{std::lerp(start.x, end.x, delta.x), std::lerp(start.y, end.y, delta.y), std::lerp(start.z, end.z, delta.z)};
    }
    static constexpr auto ILerp(const Vec& start, const Vec& end, const Vec& value) noexcept
    {
        using return_type = Internal::promote_t<value_type>;
        return Vec<return_type, element_count>{ilerp(start.x, end.x, value.x), ilerp(start.y, end.y, value.y), ilerp(start.z, end.z, value.z)};
    }
    static constexpr auto Remap(const Vec& value, const Vec& from, const Vec& to, const Vec& min, const Vec& max) noexcept
    {
        return Lerp(from, to, ILerp(min, max, value));
    }

    friend constexpr auto operator|(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Dot(lhs, rhs);
    }

    friend constexpr auto operator^(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Cross(lhs, rhs);
    }

    friend constexpr auto operator&(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Project(lhs, rhs);
    }

    friend constexpr auto operator%(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Reject(lhs, rhs);
    }
};

export using uchar3  = Vec<std::uint8_t, 3>;
export using char3   = Vec<std::int8_t, 3>;
export using ushort3 = Vec<std::uint16_t, 3>;
export using short3  = Vec<std::int16_t, 3>;
export using uint3   = Vec<std::uint32_t, 3>;
export using int3    = Vec<std::int32_t, 3>;
export using float3  = Vec<float, 3>;
export using double3 = Vec<double, 3>;

#pragma region Vec3
// Basic type traits
static_assert(std::is_trivially_copyable_v<float3>);
static_assert(std::is_standard_layout_v<float3>);
static_assert(sizeof(float3) == sizeof(float) * 3);
static_assert(alignof(float3) == alignof(float));

// Constexpr construction and initialization
static_assert(float3{} == float3{0.0f, 0.0f, 0.0f});
static_assert(float3{1.0f, 2.0f, 3.0f}.x == 1.0f);
static_assert(float3{1.0f, 2.0f, 3.0f}.y == 2.0f);
static_assert(float3{1.0f, 2.0f, 3.0f}.z == 3.0f);

// Element access
static_assert(float3{3.0f, 4.0f, 5.0f}[0] == 3.0f);
static_assert(float3{3.0f, 4.0f, 5.0f}[1] == 4.0f);
static_assert(float3{3.0f, 4.0f, 5.0f}[2] == 5.0f);

// Comparison operators
static_assert(float3{1.0f, 2.0f, 3.0f} == float3{1.0f, 2.0f, 3.0f});
static_assert(float3{1.0f, 2.0f, 3.0f} != float3{3.0f, 2.0f, 1.0f});
static_assert(float3{1.0f, 2.0f, 3.0f} < float3{2.0f, 1.0f, 1.0f});
static_assert(float3{1.0f, 2.0f, 3.0f} < float3{1.0f, 3.0f, 1.0f});
static_assert(float3{1.0f, 2.0f, 3.0f} < float3{1.0f, 2.0f, 4.0f});
static_assert(float3{2.0f, 1.0f, 1.0f} > float3{1.0f, 2.0f, 3.0f});

// Unary operators
static_assert(+float3{1.0f, 2.0f, 3.0f} == float3{1.0f, 2.0f, 3.0f});
static_assert(-float3{1.0f, 2.0f, 3.0f} == float3{-1.0f, -2.0f, -3.0f});
static_assert(-float3{-3.0f, 4.0f, -5.0f} == float3{3.0f, -4.0f, 5.0f});

// Vector addition
static_assert(float3{1.0f, 2.0f, 3.0f} + float3{4.0f, 5.0f, 6.0f} == float3{5.0f, 7.0f, 9.0f});
static_assert(float3{5.0f, -2.0f, 1.0f} + float3{-3.0f, 7.0f, -1.0f} == float3{2.0f, 5.0f, 0.0f});

// Vector subtraction
static_assert(float3{5.0f, 8.0f, 10.0f} - float3{2.0f, 3.0f, 4.0f} == float3{3.0f, 5.0f, 6.0f});
static_assert(float3{1.0f, 1.0f, 1.0f} - float3{1.0f, 1.0f, 1.0f} == float3{0.0f, 0.0f, 0.0f});

// Component-wise multiplication
static_assert(float3{2.0f, 3.0f, 4.0f} * float3{5.0f, 6.0f, 7.0f} == float3{10.0f, 18.0f, 28.0f});
static_assert(float3{-2.0f, 3.0f, -4.0f} * float3{4.0f, -5.0f, 2.0f} == float3{-8.0f, -15.0f, -8.0f});

// Component-wise division
static_assert(float3{10.0f, 18.0f, 28.0f} / float3{2.0f, 3.0f, 4.0f} == float3{5.0f, 6.0f, 7.0f});
static_assert(float3{12.0f, 20.0f, 30.0f} / float3{4.0f, 5.0f, 6.0f} == float3{3.0f, 4.0f, 5.0f});

// Scalar multiplication
static_assert(float3{2.0f, 3.0f, 4.0f} * 2.0f == float3{4.0f, 6.0f, 8.0f});
static_assert(3.0f * float3{2.0f, 4.0f, 6.0f} == float3{6.0f, 12.0f, 18.0f});
static_assert(float3{1.0f, -2.0f, 3.0f} * -2.0f == float3{-2.0f, 4.0f, -6.0f});

// Scalar division
static_assert(float3{10.0f, 20.0f, 30.0f} / 2.0f == float3{5.0f, 10.0f, 15.0f});
static_assert(float3{6.0f, -9.0f, 12.0f} / 3.0f == float3{2.0f, -3.0f, 4.0f});

// Dot product
static_assert(float3{1.0f, 2.0f, 3.0f}.Dot(float3{4.0f, 5.0f, 6.0f}) == 32.0f);          // 1*4 + 2*5 + 3*6 = 32
static_assert(float3::Dot(float3{2.0f, 3.0f, 4.0f}, float3{5.0f, 6.0f, 7.0f}) == 56.0f); // 2*5 + 3*6 + 4*7 = 56
static_assert(float3{1.0f, 0.0f, 0.0f}.Dot(float3{0.0f, 1.0f, 0.0f}) == 0.0f);           // perpendicular

// Cross product (3D returns vector)
static_assert(float3{1.0f, 0.0f, 0.0f}.Cross(float3{0.0f, 1.0f, 0.0f}) == float3{0.0f, 0.0f, 1.0f});
static_assert(float3{0.0f, 1.0f, 0.0f}.Cross(float3{1.0f, 0.0f, 0.0f}) == float3{0.0f, 0.0f, -1.0f});
static_assert(float3{0.0f, 1.0f, 0.0f}.Cross(float3{0.0f, 0.0f, 1.0f}) == float3{1.0f, 0.0f, 0.0f});
static_assert(float3{1.0f, 2.0f, 3.0f}.Cross(float3{4.0f, 5.0f, 6.0f}) == float3{-3.0f, 6.0f, -3.0f});
static_assert(float3::Cross(float3{2.0f, 3.0f, 1.0f}, float3{1.0f, 4.0f, 2.0f}) == float3{2.0f, -3.0f, 5.0f});

// Cross product anti-commutativity
static_assert(float3{1.0f, 2.0f, 3.0f}.Cross(float3{4.0f, 5.0f, 6.0f}) == -float3{4.0f, 5.0f, 6.0f}.Cross(float3{1.0f, 2.0f, 3.0f}));

// Cross product with itself is zero
static_assert(float3{1.0f, 2.0f, 3.0f}.Cross(float3{1.0f, 2.0f, 3.0f}) == float3{0.0f, 0.0f, 0.0f});

// Length squared
static_assert(float3{3.0f, 4.0f, 0.0f}.LengthSqr() == 25.0f); // 3² + 4² + 0² = 25
static_assert(float3{1.0f, 0.0f, 0.0f}.LengthSqr() == 1.0f);
static_assert(float3{0.0f, 0.0f, 0.0f}.LengthSqr() == 0.0f);
static_assert(float3::LengthSqr(float3{2.0f, 3.0f, 6.0f}) == 49.0f); // 2² + 3² + 6² = 49

// Distance squared
static_assert(float3{0.0f, 0.0f, 0.0f}.DistanceSqr(float3{3.0f, 4.0f, 0.0f}) == 25.0f);
static_assert(float3{1.0f, 1.0f, 1.0f}.DistanceSqr(float3{4.0f, 5.0f, 1.0f}) == 25.0f); // (4-1)² + (5-1)² + 0² = 25
static_assert(float3::DistanceSqr(float3{0.0f, 0.0f, 0.0f}, float3{2.0f, 3.0f, 6.0f}) == 49.0f);

// Min/Max
static_assert(float3::Min(float3{1.0f, 5.0f, 3.0f}, float3{3.0f, 2.0f, 4.0f}) == float3{1.0f, 2.0f, 3.0f});
static_assert(float3::Max(float3{1.0f, 5.0f, 3.0f}, float3{3.0f, 2.0f, 4.0f}) == float3{3.0f, 5.0f, 4.0f});
static_assert(float3::Min(float3{-1.0f, -5.0f, 0.0f}, float3{-3.0f, -2.0f, 1.0f}) == float3{-3.0f, -5.0f, 0.0f});

// Clamp
static_assert(float3::Clamp(float3{5.0f, 15.0f, 8.0f}, float3{0.0f, 0.0f, 0.0f}, float3{10.0f, 10.0f, 10.0f}) == float3{5.0f, 10.0f, 8.0f});
static_assert(float3::Clamp(float3{-5.0f, 5.0f, 12.0f}, float3{0.0f, 0.0f, 0.0f}, float3{10.0f, 10.0f, 10.0f}) == float3{0.0f, 5.0f, 10.0f});
static_assert(float3::Clamp(float3{5.0f, 5.0f, 5.0f}, float3{0.0f, 0.0f, 0.0f}, float3{10.0f, 10.0f, 10.0f}) == float3{5.0f, 5.0f, 5.0f});

// Lerp
static_assert(float3::Lerp(float3{0.0f, 0.0f, 0.0f}, float3{10.0f, 20.0f, 30.0f}, float3{0.5f, 0.5f, 0.5f}) == float3{5.0f, 10.0f, 15.0f});
static_assert(float3::Lerp(float3{0.0f, 0.0f, 0.0f}, float3{10.0f, 20.0f, 30.0f}, float3{0.0f, 0.0f, 0.0f}) == float3{0.0f, 0.0f, 0.0f});
static_assert(float3::Lerp(float3{0.0f, 0.0f, 0.0f}, float3{10.0f, 20.0f, 30.0f}, float3{1.0f, 1.0f, 1.0f}) == float3{10.0f, 20.0f, 30.0f});

// Compound assignment operators preserve type
static_assert(std::is_same_v<decltype(std::declval<float3&>() += std::declval<float3>()), float3&>);
static_assert(std::is_same_v<decltype(std::declval<float3&>() -= std::declval<float3>()), float3&>);
static_assert(std::is_same_v<decltype(std::declval<float3&>() *= std::declval<float3>()), float3&>);
static_assert(std::is_same_v<decltype(std::declval<float3&>() /= std::declval<float3>()), float3&>);
static_assert(std::is_same_v<decltype(std::declval<float3&>() *= std::declval<float>()), float3&>);
static_assert(std::is_same_v<decltype(std::declval<float3&>() /= std::declval<float>()), float3&>);

// Binary operators return value type
static_assert(std::is_same_v<decltype(std::declval<float3>() + std::declval<float3>()), float3>);
static_assert(std::is_same_v<decltype(std::declval<float3>() - std::declval<float3>()), float3>);
static_assert(std::is_same_v<decltype(std::declval<float3>() * std::declval<float3>()), float3>);
static_assert(std::is_same_v<decltype(std::declval<float3>() / std::declval<float3>()), float3>);

// Cross product returns vector type
static_assert(std::is_same_v<decltype(std::declval<float3>().Cross(std::declval<float3>())), float3>);

// Noexcept guarantees
static_assert(noexcept(float3{} + float3{}));
static_assert(noexcept(float3{} - float3{}));
static_assert(noexcept(float3{} * float3{}));
static_assert(noexcept(float3{} / float3{}));
static_assert(noexcept(float3{}.Dot(float3{})));
static_assert(noexcept(float3{}.Cross(float3{})));
static_assert(noexcept(float3{}.LengthSqr()));
static_assert(noexcept(float3{} == float3{}));
static_assert(noexcept(float3{} <=> float3{}));
#pragma endregion Vec3

template <typename T>
struct Vec<T, 4>
{
    using value_type                      = T;
    static constexpr size_t element_count = 4;
    value_type x{};
    value_type y{};
    value_type z{};
    value_type w{};

    constexpr Vec<T, 2> xy() noexcept
    {
        return Vec<T, 2>{x, y};
    }
    constexpr Vec<T, 3> xyz() noexcept
    {
        return Vec<T, 3>{x, y, z};
    }

    constexpr const value_type& operator[](const std::uint8_t idx) const
    {
        // assert(idx < element_count);
        return idx != 0 ? idx != 1 ? idx != 2 ? w : z : y : x;
    }

    constexpr value_type& operator[](const std::uint8_t idx)
    {
        // assert(idx < element_count);
        return idx != 0 ? idx != 1 ? idx != 2 ? w : z : y : x;
    }

    constexpr bool operator==(const Vec& other) const noexcept
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    constexpr auto operator<=>(const Vec& other) const noexcept
    {
        if (auto value{x <=> other.x}; value != 0)
        {
            return value;
        }
        if (auto value{y <=> other.y}; value != 0)
        {
            return value;
        }
        if (auto value{z <=> other.z}; value != 0)
        {
            return value;
        }
        return w <=> other.w;
    }

    constexpr Vec operator+() const noexcept
    {
        return *this;
    }
    constexpr Vec operator-() const noexcept
    {
        return {-x, -y, -z, -w};
    }
    constexpr Vec& operator+=(const Vec& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    constexpr Vec& operator-=(const Vec& other) noexcept
    {
        *this += -other;
        return *this;
    }
    constexpr Vec& operator*=(const Vec& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }
    constexpr Vec& operator/=(const Vec& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }
    friend constexpr Vec operator+(Vec lhs, const Vec& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Vec operator-(Vec lhs, const Vec& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr Vec operator*(Vec lhs, const Vec& rhs) noexcept
    {
        lhs *= rhs;
        return lhs;
    }

    friend constexpr Vec operator/(Vec lhs, const Vec& rhs) noexcept
    {
        lhs /= rhs;
        return lhs;
    }

    constexpr Vec& operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    constexpr Vec& operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    friend constexpr Vec operator*(Vec lhs, T scalar) noexcept
    {
        lhs *= scalar;
        return lhs;
    }

    friend constexpr Vec operator*(T scalar, Vec rhs) noexcept
    {
        rhs *= scalar;
        return rhs;
    }

    friend constexpr Vec operator/(Vec lhs, T scalar) noexcept
    {
        lhs /= scalar;
        return lhs;
    }

    [[nodiscard]] constexpr value_type Dot(const Vec& rhs) const noexcept
    {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }
    static constexpr value_type Dot(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Dot(rhs);
    }
    [[nodiscard]] constexpr value_type LengthSqr() const noexcept
    {
        return Dot(*this);
    }
    static constexpr value_type LengthSqr(const Vec& val) noexcept
    {
        return val.LengthSqr();
    }

    [[nodiscard]] constexpr auto Length() const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        return std::sqrt(static_cast<promo_type>(LengthSqr()));
    }

    static constexpr value_type Length(const Vec& val) noexcept
    {
        return val.Length();
    }

    [[nodiscard]] constexpr value_type DistanceSqr(const Vec& to) const noexcept
    {
        const Vec dist = to - *this;
        return Dot(dist, dist);
    }

    static constexpr value_type DistanceSqr(const Vec& from, const Vec& to) noexcept
    {
        return from.DistanceSqr(to);
    }

    [[nodiscard]] constexpr auto Distance(const Vec& to) const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        return std::sqrt(static_cast<promo_type>(DistanceSqr(to)));
    }

    static constexpr value_type Distance(const Vec& from, const Vec& to) noexcept
    {
        return from.Distance(to);
    }

    [[nodiscard]] constexpr auto Normalize() const noexcept
    {
        using promo_type = Internal::promote_t<value_type>;
        promo_type len   = static_cast<promo_type>(1) / Length();
        return Vec<promo_type, 4>{x * len, y * len, z * len, w * len};
    }

    static constexpr Vec Normalize(const Vec& val) noexcept
    {
        return val.Normalize();
    }

    [[nodiscard]] constexpr auto NormalizeSafe() const noexcept
    {
        auto len          = Length();
        using length_type = std::decay_t<decltype(len)>;
        if (len < static_cast<length_type>(1E-6))
        {
            return *this;
        }
        return (*this / len);
    }

    static constexpr auto NormalizeSafe(const Vec& val) noexcept
    {
        return val.NormalizeSafe();
    }

    [[nodiscard]] constexpr Vec Project(const Vec& other) const noexcept
    {
        return (Dot(other) / other.Dot(other)) * other;
    }

    static constexpr Vec Project(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Project(rhs);
    }

    [[nodiscard]] constexpr Vec Reject(const Vec& other) const noexcept
    {
        return *this - Project(other);
    }

    static constexpr Vec Reject(const Vec& lhs, const Vec& rhs) noexcept
    {
        return lhs.Project(rhs);
    }

    static constexpr Vec Min(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Vec{std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w)};
    }

    static constexpr Vec Max(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Vec{std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w)};
    }

    static constexpr Vec Clamp(const Vec& lhs, const Vec& min, const Vec& max) noexcept
    {
        return Vec{std::clamp(lhs.x, min.x, max.x), std::clamp(lhs.y, min.y, max.y), std::clamp(lhs.z, min.z, max.z), std::clamp(lhs.w, min.w, max.w)};
    }

    static constexpr auto Round(const Vec& value) noexcept
    {
        return Vec{std::round(value.x), std::round(value.y), std::round(value.z), std::round(value.w)};
    }

    static constexpr auto Floor(const Vec& value) noexcept
    {
        return Vec<std::uint32_t, element_count>{std::floor(value.x), std::floor(value.y), std::floor(value.z), std::floor(value.w)};
    }

    static constexpr Vec Ceil(const Vec& value) noexcept
    {
        return Vec{std::ceil(value.x), std::ceil(value.y), std::ceil(value.z), std::ceil(value.w)};
    }

    static constexpr auto Lerp(const Vec& start, const Vec& end, const Vec& delta) noexcept
    {
        using return_type = Internal::promote_t<value_type>;
        return Vec<return_type, element_count>{std::lerp(start.x, end.x, delta.x), std::lerp(start.y, end.y, delta.y), std::lerp(start.z, end.z, delta.z),
                                               std::lerp(start.w, end.w, delta.w)};
    }

    static constexpr auto ILerp(const Vec& start, const Vec& end, const Vec& value) noexcept
    {
        using return_type = Internal::promote_t<value_type>;
        return Vec<return_type, element_count>{ilerp(start.x, end.x, value.x), ilerp(start.y, end.y, value.y), ilerp(start.z, end.z, value.z), ilerp(start.w, end.w, value.w)};
    }

    static constexpr auto Remap(const Vec& value, const Vec& from, const Vec& to, const Vec& min, const Vec& max) noexcept
    {
        return Lerp(from, to, ILerp(min, max, value));
    }

    friend constexpr auto operator|(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Dot(lhs, rhs);
    }

    friend constexpr auto operator^(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Cross(lhs, rhs);
    }

    friend constexpr auto operator&(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Project(lhs, rhs);
    }

    friend constexpr auto operator%(const Vec& lhs, const Vec& rhs) noexcept
    {
        return Reject(lhs, rhs);
    }
};

export using uchar4  = Vec<std::uint8_t, 4>;
export using char4   = Vec<std::uint8_t, 4>;
export using ushort4 = Vec<std::uint16_t, 4>;
export using short4  = Vec<std::uint16_t, 4>;
export using uint4   = Vec<std::uint32_t, 4>;
export using int4    = Vec<std::uint32_t, 4>;
export using float4  = Vec<float, 4>;
export using double4 = Vec<double, 4>;

#pragma region Vec4
// Basic type traits
static_assert(std::is_trivially_copyable_v<float4>);
static_assert(std::is_standard_layout_v<float4>);
static_assert(sizeof(float4) == sizeof(float) * 4);
static_assert(alignof(float4) == alignof(float));

// Constexpr construction and initialization
static_assert(float4{} == float4{0.0f, 0.0f, 0.0f, 0.0f});
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f}.x == 1.0f);
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f}.y == 2.0f);
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f}.z == 3.0f);
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f}.w == 4.0f);

// Element access
static_assert(float4{3.0f, 4.0f, 5.0f, 6.0f}[0] == 3.0f);
static_assert(float4{3.0f, 4.0f, 5.0f, 6.0f}[1] == 4.0f);
static_assert(float4{3.0f, 4.0f, 5.0f, 6.0f}[2] == 5.0f);
static_assert(float4{3.0f, 4.0f, 5.0f, 6.0f}[3] == 6.0f);

// Comparison operators
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f} == float4{1.0f, 2.0f, 3.0f, 4.0f});
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f} != float4{4.0f, 3.0f, 2.0f, 1.0f});
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f} < float4{2.0f, 1.0f, 1.0f, 1.0f});
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f} < float4{1.0f, 3.0f, 1.0f, 1.0f});
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f} < float4{1.0f, 2.0f, 4.0f, 1.0f});
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f} < float4{1.0f, 2.0f, 3.0f, 5.0f});
static_assert(float4{2.0f, 1.0f, 1.0f, 1.0f} > float4{1.0f, 2.0f, 3.0f, 4.0f});

// Unary operators
static_assert(+float4{1.0f, 2.0f, 3.0f, 4.0f} == float4{1.0f, 2.0f, 3.0f, 4.0f});
static_assert(-float4{1.0f, 2.0f, 3.0f, 4.0f} == float4{-1.0f, -2.0f, -3.0f, -4.0f});
static_assert(-float4{-3.0f, 4.0f, -5.0f, 6.0f} == float4{3.0f, -4.0f, 5.0f, -6.0f});

// Vector addition
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f} + float4{5.0f, 6.0f, 7.0f, 8.0f} == float4{6.0f, 8.0f, 10.0f, 12.0f});
static_assert(float4{5.0f, -2.0f, 1.0f, 0.0f} + float4{-3.0f, 7.0f, -1.0f, 1.0f} == float4{2.0f, 5.0f, 0.0f, 1.0f});

// Vector subtraction
static_assert(float4{5.0f, 8.0f, 10.0f, 12.0f} - float4{2.0f, 3.0f, 4.0f, 5.0f} == float4{3.0f, 5.0f, 6.0f, 7.0f});
static_assert(float4{1.0f, 1.0f, 1.0f, 1.0f} - float4{1.0f, 1.0f, 1.0f, 1.0f} == float4{0.0f, 0.0f, 0.0f, 0.0f});

// Component-wise multiplication
static_assert(float4{2.0f, 3.0f, 4.0f, 5.0f} * float4{6.0f, 7.0f, 8.0f, 9.0f} == float4{12.0f, 21.0f, 32.0f, 45.0f});
static_assert(float4{-2.0f, 3.0f, -4.0f, 5.0f} * float4{4.0f, -5.0f, 2.0f, -3.0f} == float4{-8.0f, -15.0f, -8.0f, -15.0f});

// Component-wise division
static_assert(float4{12.0f, 21.0f, 32.0f, 45.0f} / float4{2.0f, 3.0f, 4.0f, 5.0f} == float4{6.0f, 7.0f, 8.0f, 9.0f});
static_assert(float4{12.0f, 20.0f, 30.0f, 40.0f} / float4{4.0f, 5.0f, 6.0f, 8.0f} == float4{3.0f, 4.0f, 5.0f, 5.0f});

// Scalar multiplication
static_assert(float4{2.0f, 3.0f, 4.0f, 5.0f} * 2.0f == float4{4.0f, 6.0f, 8.0f, 10.0f});
static_assert(3.0f * float4{2.0f, 4.0f, 6.0f, 8.0f} == float4{6.0f, 12.0f, 18.0f, 24.0f});
static_assert(float4{1.0f, -2.0f, 3.0f, -4.0f} * -2.0f == float4{-2.0f, 4.0f, -6.0f, 8.0f});

// Scalar division
static_assert(float4{10.0f, 20.0f, 30.0f, 40.0f} / 2.0f == float4{5.0f, 10.0f, 15.0f, 20.0f});
static_assert(float4{6.0f, -9.0f, 12.0f, -15.0f} / 3.0f == float4{2.0f, -3.0f, 4.0f, -5.0f});

// Dot product
static_assert(float4{1.0f, 2.0f, 3.0f, 4.0f}.Dot(float4{5.0f, 6.0f, 7.0f, 8.0f}) == 70.0f);           // 1*5 + 2*6 + 3*7 + 4*8 = 70
static_assert(float4::Dot(float4{2.0f, 3.0f, 4.0f, 5.0f}, float4{6.0f, 7.0f, 8.0f, 9.0f}) == 110.0f); // 2*6 + 3*7 + 4*8 + 5*9 = 110
static_assert(float4{1.0f, 0.0f, 0.0f, 0.0f}.Dot(float4{0.0f, 1.0f, 0.0f, 0.0f}) == 0.0f);            // perpendicular

// Length squared
static_assert(float4{3.0f, 4.0f, 0.0f, 0.0f}.LengthSqr() == 25.0f); // 3² + 4² + 0² + 0² = 25
static_assert(float4{1.0f, 0.0f, 0.0f, 0.0f}.LengthSqr() == 1.0f);
static_assert(float4{0.0f, 0.0f, 0.0f, 0.0f}.LengthSqr() == 0.0f);
static_assert(float4::LengthSqr(float4{2.0f, 3.0f, 6.0f, 0.0f}) == 49.0f); // 2² + 3² + 6² + 0² = 49
static_assert(float4{1.0f, 2.0f, 2.0f, 4.0f}.LengthSqr() == 25.0f);        // 1² + 2² + 2² + 4² = 25

// Distance squared
static_assert(float4{0.0f, 0.0f, 0.0f, 0.0f}.DistanceSqr(float4{3.0f, 4.0f, 0.0f, 0.0f}) == 25.0f);
static_assert(float4{1.0f, 1.0f, 1.0f, 1.0f}.DistanceSqr(float4{4.0f, 5.0f, 1.0f, 1.0f}) == 25.0f); // (4-1)² + (5-1)² + 0² + 0² = 25
static_assert(float4::DistanceSqr(float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{2.0f, 3.0f, 6.0f, 0.0f}) == 49.0f);

// Min/Max
static_assert(float4::Min(float4{1.0f, 5.0f, 3.0f, 7.0f}, float4{3.0f, 2.0f, 4.0f, 6.0f}) == float4{1.0f, 2.0f, 3.0f, 6.0f});
static_assert(float4::Max(float4{1.0f, 5.0f, 3.0f, 7.0f}, float4{3.0f, 2.0f, 4.0f, 6.0f}) == float4{3.0f, 5.0f, 4.0f, 7.0f});
static_assert(float4::Min(float4{-1.0f, -5.0f, 0.0f, 2.0f}, float4{-3.0f, -2.0f, 1.0f, -1.0f}) == float4{-3.0f, -5.0f, 0.0f, -1.0f});

// Clamp
static_assert(float4::Clamp(float4{5.0f, 15.0f, 8.0f, 12.0f}, float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{10.0f, 10.0f, 10.0f, 10.0f}) == float4{5.0f, 10.0f, 8.0f, 10.0f});
static_assert(float4::Clamp(float4{-5.0f, 5.0f, 12.0f, -2.0f}, float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{10.0f, 10.0f, 10.0f, 10.0f}) == float4{0.0f, 5.0f, 10.0f, 0.0f});
static_assert(float4::Clamp(float4{5.0f, 5.0f, 5.0f, 5.0f}, float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{10.0f, 10.0f, 10.0f, 10.0f}) == float4{5.0f, 5.0f, 5.0f, 5.0f});

// Lerp
static_assert(float4::Lerp(float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{10.0f, 20.0f, 30.0f, 40.0f}, float4{0.5f, 0.5f, 0.5f, 0.5f}) == float4{5.0f, 10.0f, 15.0f, 20.0f});
static_assert(float4::Lerp(float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{10.0f, 20.0f, 30.0f, 40.0f}, float4{0.0f, 0.0f, 0.0f, 0.0f}) == float4{0.0f, 0.0f, 0.0f, 0.0f});
static_assert(float4::Lerp(float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{10.0f, 20.0f, 30.0f, 40.0f}, float4{1.0f, 1.0f, 1.0f, 1.0f}) == float4{10.0f, 20.0f, 30.0f, 40.0f});

// Per-component lerp
static_assert(float4::Lerp(float4{0.0f, 0.0f, 0.0f, 0.0f}, float4{10.0f, 20.0f, 30.0f, 40.0f}, float4{0.0f, 0.25f, 0.5f, 1.0f}) == float4{0.0f, 5.0f, 15.0f, 40.0f});

// Compound assignment operators preserve type
static_assert(std::is_same_v<decltype(std::declval<float4&>() += std::declval<float4>()), float4&>);
static_assert(std::is_same_v<decltype(std::declval<float4&>() -= std::declval<float4>()), float4&>);
static_assert(std::is_same_v<decltype(std::declval<float4&>() *= std::declval<float4>()), float4&>);
static_assert(std::is_same_v<decltype(std::declval<float4&>() /= std::declval<float4>()), float4&>);
static_assert(std::is_same_v<decltype(std::declval<float4&>() *= std::declval<float>()), float4&>);
static_assert(std::is_same_v<decltype(std::declval<float4&>() /= std::declval<float>()), float4&>);

// Binary operators return value type
static_assert(std::is_same_v<decltype(std::declval<float4>() + std::declval<float4>()), float4>);
static_assert(std::is_same_v<decltype(std::declval<float4>() - std::declval<float4>()), float4>);
static_assert(std::is_same_v<decltype(std::declval<float4>() * std::declval<float4>()), float4>);
static_assert(std::is_same_v<decltype(std::declval<float4>() / std::declval<float4>()), float4>);

// Noexcept guarantees
static_assert(noexcept(float4{} + float4{}));
static_assert(noexcept(float4{} - float4{}));
static_assert(noexcept(float4{} * float4{}));
static_assert(noexcept(float4{} / float4{}));
static_assert(noexcept(float4{}.Dot(float4{})));
static_assert(noexcept(float4{}.LengthSqr()));
static_assert(noexcept(float4{} == float4{}));
static_assert(noexcept(float4{} <=> float4{}));

// Homogeneous coordinate tests (common use case for float4)
// Position with w=1.0
static_assert(float4{1.0f, 2.0f, 3.0f, 1.0f}.w == 1.0f);
// direction/vector with w=0.0
static_assert(float4{1.0f, 2.0f, 3.0f, 0.0f}.w == 0.0f);

// RGBA color component access (alternative interpretation)
static_assert(float4{1.0f, 0.5f, 0.25f, 1.0f}.x == 1.0f);  // r
static_assert(float4{1.0f, 0.5f, 0.25f, 1.0f}.y == 0.5f);  // g
static_assert(float4{1.0f, 0.5f, 0.25f, 1.0f}.z == 0.25f); // b
static_assert(float4{1.0f, 0.5f, 0.25f, 1.0f}.w == 1.0f);  // a
#pragma endregion Vec4

#if BALBINO_SIMD_SSE

template <std::uint8_t N>
    requires(N == 3 || N == 4)
struct VecSimd<float, N>
{
    using value_type = float;
    __m128 data{_mm_setzero_ps()};

    // ========================================================================
    // Construction
    // ========================================================================

    [[nodiscard]] static constexpr VecSimd Create(const float x, const float y, const float z, const float w = 0.0f) noexcept
    {
        return VecSimd{.data = _mm_set_ps(w, z, y, x)};
    }

    [[nodiscard]] static constexpr VecSimd Create(const Vec<float, 3>& v, const float w = 0.0f) noexcept
    {
        return VecSimd{.data = _mm_set_ps(w, v.z, v.y, v.x)};
    }

    [[nodiscard]] static constexpr VecSimd Create(const Vec<float, 4>& v) noexcept
    {
        return VecSimd{.data = _mm_set_ps(v.w, v.z, v.y, v.x)};
    }

    [[nodiscard]] static constexpr VecSimd Create(__m128 v) noexcept
    {
        return VecSimd{.data = v};
    }

    // ========================================================================
    // Component Access
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float x() const noexcept
    {
        return _mm_cvtss_f32(data);
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float y() const noexcept
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(1, 1, 1, 1)));
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float z() const noexcept
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(2, 2, 2, 2)));
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float w() const noexcept
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 3, 3, 3)));
    }

    BALBINO_FORCE_INLINE void SetX(const float val) noexcept
    {
        data = _mm_move_ss(data, _mm_set_ss(val));
    }

    BALBINO_FORCE_INLINE void SetY(const float val) noexcept
    {
        __m128 tmp = _mm_move_ss(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 2, 0, 1)), _mm_set_ss(val));
        data       = _mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3, 2, 0, 1));
    }

    BALBINO_FORCE_INLINE void SetZ(const float val) noexcept
    {
        __m128 tmp = _mm_move_ss(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 0, 1, 2)), _mm_set_ss(val));
        data       = _mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3, 0, 1, 2));
    }

    BALBINO_FORCE_INLINE void SetW(const float val) noexcept
    {
        __m128 tmp = _mm_move_ss(_mm_shuffle_ps(data, data, _MM_SHUFFLE(0, 2, 1, 3)), _mm_set_ss(val));
        data       = _mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3, 2, 1, 0));
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float operator[](const std::uint8_t idx) const noexcept
    {
        // BALBINO_ASSERT_RANGE(idx, static_cast<std::uint8_t>(0), static_cast<std::uint8_t>(N - 1), "Index out of range: float{}_simd has size of {}, but index {} was taken", N,
        // N,
        //                      idx);

        switch (idx)
        {
        case 0: return x();
        case 1: return y();
        case 2: return z();
        case 3:
            return w();
            // default: BALBINO_UNREACHABLE("Invalid vector index");
        default: std::unreachable();
        }
    }

    // ========================================================================
    // Comparison Operators
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr bool operator==(const VecSimd other) const noexcept
    {
        constexpr int mask = (N == 3) ? 0x7 : 0xF;
        return (_mm_movemask_ps(_mm_cmpeq_ps(data, other.data)) & mask) == mask;
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr std::strong_ordering operator<=>(const VecSimd other) const noexcept
    {
        const int lt = _mm_movemask_ps(_mm_cmplt_ps(data, other.data));
        const int gt = _mm_movemask_ps(_mm_cmpgt_ps(data, other.data));

        if ((lt | gt) == 0)
            BALBINO_LIKELY
            {
                return std::strong_ordering::equal;
            }
        if (lt != 0 && gt == 0)
            return std::strong_ordering::less;
        if (gt != 0 && lt == 0)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    // ========================================================================
    // Arithmetic Operators
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator+(const VecSimd rhs) const noexcept
    {
        return VecSimd{_mm_add_ps(data, rhs.data)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator-(const VecSimd rhs) const noexcept
    {
        return VecSimd{_mm_sub_ps(data, rhs.data)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator*(const VecSimd rhs) const noexcept
    {
        return VecSimd{_mm_mul_ps(data, rhs.data)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator/(const VecSimd rhs) const noexcept
    {
        return VecSimd{_mm_div_ps(data, rhs.data)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator-() const noexcept
    {
        return VecSimd{_mm_xor_ps(data, _mm_set1_ps(-0.0f))};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator+() const noexcept
    {
        return *this;
    }

    BALBINO_FORCE_INLINE constexpr VecSimd& operator+=(const VecSimd rhs) noexcept
    {
        data = _mm_add_ps(data, rhs.data);
        return *this;
    }

    BALBINO_FORCE_INLINE constexpr VecSimd& operator-=(const VecSimd rhs) noexcept
    {
        data = _mm_sub_ps(data, rhs.data);
        return *this;
    }

    BALBINO_FORCE_INLINE constexpr VecSimd& operator*=(const VecSimd rhs) noexcept
    {
        data = _mm_mul_ps(data, rhs.data);
        return *this;
    }

    BALBINO_FORCE_INLINE constexpr VecSimd& operator/=(const VecSimd rhs) noexcept
    {
        data = _mm_div_ps(data, rhs.data);
        return *this;
    }

    // ========================================================================
    // Scalar Operations
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator*(const float s) const noexcept
    {
        return VecSimd{_mm_mul_ps(data, _mm_set1_ps(s))};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE friend constexpr VecSimd operator*(const float lhs, const VecSimd rhs) noexcept
    {
        return VecSimd{_mm_mul_ps(_mm_set1_ps(lhs), rhs.data)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd operator/(const float s) const noexcept
    {
        // assert(std::abs(s) > 1E-6f && "Division by near-zero scalar");
        return VecSimd{_mm_div_ps(data, _mm_set1_ps(s))};
    }

    BALBINO_FORCE_INLINE constexpr VecSimd& operator*=(const float s) noexcept
    {
        data = _mm_mul_ps(data, _mm_set1_ps(s));
        return *this;
    }

    BALBINO_FORCE_INLINE constexpr VecSimd& operator/=(const float s) noexcept
    {
        // assert(std::abs(s) > 1E-6F && "Division by near-zero scalar");
        data = _mm_div_ps(data, _mm_set1_ps(s));
        return *this;
    }

    // ========================================================================
    // Vector Operations
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float Dot(VecSimd other) const noexcept
    {
        constexpr int mask = (N == 3) ? 0x71 : 0xF1;
#    if BALBINO_SIMD_SSE41
        return _mm_cvtss_f32(_mm_dp_ps(data, other.data, mask));
#    else
        __m128 mul  = _mm_mul_ps(data, other.data);
        __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(mul, shuf);
        sums        = _mm_add_ss(sums, _mm_movehl_ps(shuf, sums));
        return _mm_cvtss_f32(sums);
#    endif
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Abs(const VecSimd& vec)
    {
        const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
        return VecSimd{_mm_and_ps(vec.data, mask)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr float Dot(const VecSimd lhs, const VecSimd rhs) noexcept
    {
        return lhs.Dot(rhs);
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd Cross(VecSimd other) const noexcept
        requires(N == 3)
    {
        // Reference: https://geometrian.com/resources/cross_product/
        // Right-handed: result = this × other
        __m128 a_yzx = _mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 b_yzx = _mm_shuffle_ps(other.data, other.data, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 c     = _mm_sub_ps(_mm_mul_ps(data, b_yzx), _mm_mul_ps(a_yzx, other.data));
        return VecSimd{_mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1))};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Cross(const VecSimd lhs, const VecSimd rhs) noexcept
        requires(N == 3)
    {
        return lhs.Cross(rhs);
    }

    // ========================================================================
    // Length and Distance
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float LengthSqr() const noexcept
    {
        return Dot(*this);
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr float LengthSqr(const VecSimd v) noexcept
    {
        return v.LengthSqr();
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float Length() const noexcept
    {
#    if BALBINO_SIMD_SSE41
        constexpr int mask = (N == 3) ? 0x71 : 0xF1;
        return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(data, data, mask)));
#    else
        return std::sqrt(LengthSqr());
#    endif
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr float Length(const VecSimd v) noexcept
    {
        return v.Length();
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float DistanceSqr(VecSimd other) const noexcept
    {
        return (*this - other).LengthSqr();
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr float DistanceSqr(VecSimd from, VecSimd to) noexcept
    {
        return (to - from).LengthSqr();
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float Distance(VecSimd other) const noexcept
    {
        return (*this - other).Length();
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr float Distance(VecSimd from, VecSimd to) noexcept
    {
        return (to - from).Length();
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr VecSimd Normalize() const noexcept
    {
#    if BALBINO_SIMD_SSE41
        constexpr int mask = (N == 3) ? 0x7F : 0xFF;
        __m128 len_sqr     = _mm_dp_ps(data, data, mask);
        len_sqr            = _mm_max_ps(len_sqr, _mm_set1_ps(1e-6f));
        __m128 inv_len     = _mm_rsqrt_ps(len_sqr);
        return VecSimd{_mm_mul_ps(data, inv_len)};
#    elif BALBINO_SIMD_SSE
        __m128 mul  = _mm_mul_ps(data, data);
        __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(mul, shuf);
        sums        = _mm_add_ss(sums, _mm_movehl_ps(shuf, sums));

        __m128 len_sqr = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(0, 0, 0, 0));
        len_sqr        = _mm_max_ps(len_sqr, _mm_set1_ps(1e-6f));

        __m128 inv_len = _mm_rsqrt_ps(len_sqr);
        return VecSimd{_mm_mul_ps(data, inv_len)};
#    else
        const float len_sqr = LengthSqr();
        if (len_sqr < 1e-6f)
            BALBINO_UNLIKELY
            {
                return VecSimd{};
            }
        return *this / std::sqrt(len_sqr);
#    endif
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Normalize(const VecSimd v) noexcept
    {
        return v.Normalize();
    }

    // ========================================================================
    // Utility Functions
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Min(const VecSimd lhs, const VecSimd rhs) noexcept
    {
        return VecSimd{_mm_min_ps(lhs.data, rhs.data)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Max(const VecSimd lhs, const VecSimd rhs) noexcept
    {
        return VecSimd{_mm_max_ps(lhs.data, rhs.data)};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Clamp(const VecSimd val, const VecSimd min, const VecSimd max) noexcept
    {
        return Max(Min(val, max), min);
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr float Area() const noexcept
    {
        __m128 y    = _mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 prod = _mm_mul_ps(data, y);

#    if BALBINO_SIMD_SSE3
        __m128 sum = _mm_hadd_ps(prod, prod);
        return _mm_cvtss_f32(_mm_hadd_ps(sum, sum));
#    else
        __m128 temp = _mm_movehl_ps(prod, prod);
        __m128 sum  = _mm_add_ps(prod, temp);
        return _mm_cvtss_f32(_mm_add_ss(sum, _mm_shuffle_ps(sum, sum, 1)));
#    endif
    }

    // ========================================================================
    // Interpolation
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Lerp(const VecSimd a, const VecSimd b, const VecSimd t) noexcept
    {
        return VecSimd{_mm_add_ps(a.data, _mm_mul_ps(t.data, _mm_sub_ps(b.data, a.data)))};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd ILerp(const VecSimd a, const VecSimd b, const VecSimd v) noexcept
    {
        const __m128 zero = _mm_setzero_ps();
        const __m128 one  = _mm_set1_ps(1.0f);
        __m128 delta      = _mm_sub_ps(b.data, a.data);

        // Avoid division by zero
        __m128 is_zero = _mm_cmpeq_ps(delta, zero);
        delta          = _mm_or_ps(delta, _mm_and_ps(is_zero, one));

        // Calculate t
        __m128 t = _mm_div_ps(_mm_sub_ps(v.data, a.data), delta);
        t        = _mm_max_ps(zero, _mm_min_ps(t, one));

        // Invert t if range is inverted
        __m128 inv_mask = _mm_cmpgt_ps(a.data, b.data);

#    if BALBINO_SIMD_SSE41
        return VecSimd{_mm_blendv_ps(t, _mm_sub_ps(one, t), inv_mask)};
#    else
        __m128 t_inv = _mm_sub_ps(one, t);
        return VecSimd{_mm_or_ps(_mm_and_ps(inv_mask, t_inv), _mm_andnot_ps(inv_mask, t))};
#    endif
    }

    [[nodiscard]] BALBINO_FORCE_INLINE static constexpr VecSimd Remap(const VecSimd val, const VecSimd from_min, const VecSimd from_max, const VecSimd to_min,
                                                                      const VecSimd to_max) noexcept
    {
        return Lerp(to_min, to_max, ILerp(from_min, from_max, val));
    }

    // ========================================================================
    // Conversion
    // ========================================================================

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr Vec<float, 2> ToVec2() const noexcept
    {
        alignas(16) float tmp[4];
        _mm_store_ps(tmp, data);
        return Vec<float, 2>{tmp[0], tmp[1]};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr Vec<float, 3> ToVec3() const noexcept
    {
        alignas(16) float tmp[4];
        _mm_store_ps(tmp, data);
        return Vec<float, 3>{tmp[0], tmp[1], tmp[2]};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr Vec<float, 4> ToVec4() const noexcept
        requires(N == 4)
    {
        alignas(16) float tmp[4];
        _mm_store_ps(tmp, data);
        return Vec<float, 4>{tmp[0], tmp[1], tmp[2], tmp[3]};
    }

    [[nodiscard]] BALBINO_FORCE_INLINE constexpr __m128 Raw() const noexcept
    {
        return data;
    }

    constexpr value_type operator|(const VecSimd& other) noexcept
    {
        return Dot(other);
    }

    constexpr value_type operator^(const VecSimd& other) noexcept
    {
        return Cross(other);
    }
};

// ============================================================================
// Type Aliases
// ============================================================================

export using float3_simd = VecSimd<float, 3>;
export using float4_simd = VecSimd<float, 4>;

// ============================================================================
// Compile-Time Checks
// ============================================================================

namespace Detail
{
// BALBINO_STATIC_ASSERT(std::is_trivially_copyable_v<float3_simd>, "float3_simd must be trivially copyable for memcpy optimization");
// BALBINO_STATIC_ASSERT(std::is_standard_layout_v<float3_simd>, "float3_simd must have standard layout for interop");
// BALBINO_STATIC_ASSERT(sizeof(float3_simd) == sizeof(float) * 4, "float3_simd must be 16 bytes for SSE alignment");
// BALBINO_STATIC_ASSERT(alignof(float3_simd) == 16, "float3_simd must be 16-byte aligned for SSE performance");

// BALBINO_STATIC_ASSERT(std::is_trivially_copyable_v<float4_simd>, "float4_simd must be trivially copyable");
// BALBINO_STATIC_ASSERT(std::is_standard_layout_v<float4_simd>, "float4_simd must have standard layout");
// BALBINO_STATIC_ASSERT(sizeof(float4_simd) == sizeof(float) * 4, "float4_simd must be 16 bytes");
// BALBINO_STATIC_ASSERT(alignof(float4_simd) == 16, "float4_simd must be 16-byte aligned");
} // namespace Detail

#endif // BALBINO_SIMD_SSE

namespace direction
{

template <typename T, std::uint8_t N>
[[nodiscard]] consteval Vec<T, N> Up() noexcept
{
    if constexpr (N == 2)
    {
        return Vec<T, 2>{T(0), T(1)};
    }
    else if constexpr (N == 3)
    {
        return Vec<T, 3>{T(0), T(0), T(1)};
    }
    else if constexpr (N == 4)
    {
        return Vec<T, 4>{T(0), T(0), T(1), T(0)};
    }
    else
    {
        static_assert(false);
        return {};
    }
}

template <typename T, std::uint8_t N>
[[nodiscard]] consteval Vec<T, N> Down() noexcept
{
    if constexpr (N == 2)
    {
        return Vec<T, 2>{T(0), T(-1)};
    }
    else if constexpr (N == 3)
    {
        return Vec<T, 3>{T(0), T(0), T(-1)};
    }
    else if constexpr (N == 4)
    {
        return Vec<T, 4>{T(0), T(0), T(-1), T(0)};
    }
    else
    {
        static_assert(false);
        return {};
    }
}

template <typename T, std::uint8_t N>
[[nodiscard]] consteval Vec<T, N> Right() noexcept
{
    if constexpr (N == 2)
    {
        return Vec<T, 2>{T(1), T(0)};
    }
    else if constexpr (N == 3)
    {
        return Vec<T, 3>{T(1), T(0), T(0)};
    }
    else if constexpr (N == 4)
    {
        return Vec<T, 4>{T(1), T(0), T(0), T(0)};
    }
    else
    {
        static_assert(false);
        return {};
    }
}

template <typename T, std::uint8_t N>
[[nodiscard]] consteval Vec<T, N> Left() noexcept
{
    if constexpr (N == 2)
    {
        return Vec<T, 2>{T(-1), T(0)};
    }
    else if constexpr (N == 3)
    {
        return Vec<T, 3>{T(-1), T(0), T(0)};
    }
    else if constexpr (N == 4)
    {
        return Vec<T, 4>{T(-1), T(0), T(0), T(0)};
    }
    else
    {
        static_assert(false);
        return {};
    }
}

template <typename T, std::uint8_t N>
[[nodiscard]] consteval Vec<T, N> Forward() noexcept
{
    if constexpr (N == 2)
    {
        return Vec<T, 2>{T(1), T(0)};
    }
    else if constexpr (N == 3)
    {
        return Vec<T, 3>{T(0), T(1), T(0)};
    }
    else if constexpr (N == 4)
    {
        return Vec<T, 4>{T(0), T(1), T(0), T(0)};
    }
    else
    {
        static_assert(false);
        return {};
    }
}

template <typename T, std::uint8_t N>
[[nodiscard]] consteval Vec<T, N> Back() noexcept
{
    if constexpr (N == 2)
    {
        return Vec<T, 2>{T(-1), T(0)};
    }
    else if constexpr (N == 3)
    {
        return Vec<T, 3>{T(0), T(-1), T(0)};
    }
    else if constexpr (N == 4)
    {
        return Vec<T, 4>{T(0), T(-1), T(0), T(0)};
    }
    else
    {
        static_assert(false);
        return {};
    }
}
} // namespace direction

template <typename T>
struct Mat<T, 2>
{
    using column_type                    = T;
    using value_type                     = T::value_type; // get scalar type from Vec
    static constexpr size_t column_count = 2;
    static constexpr size_t row_count    = T::element_count;

    column_type mat[column_count]{};

    constexpr const column_type& operator[](size_t i) const
    {
        // assert(i < column_count);
        return mat[i];
    }

    constexpr column_type& operator[](size_t i)
    {
        // assert(i < column_count);
        return mat[i];
    }

    constexpr Mat operator+() const
    {
        return *this;
    }
    constexpr Mat operator-() const
    {
        return Mat(-mat[0], -mat[1]);
    }

    constexpr Mat& operator+=(const Mat& rhs)
    {
        mat[0] += rhs.mat[0];
        mat[1] += rhs.mat[1];
        return *this;
    }

    constexpr Mat& operator-=(const Mat& rhs)
    {
        mat[0] -= rhs.mat[0];
        mat[1] -= rhs.mat[1];
        return *this;
    }

    constexpr Mat& operator*=(const Mat& rhs) noexcept
    {
        Mat result{};
        for (size_t i = 0; i < column_count; ++i)
        {
            for (size_t j = 0; j < row_count; ++j)
            {
                for (size_t k = 0; k < column_count; ++k)
                {
                    result[i][j] += mat[k][j] * rhs[i][k];
                }
            }
        }
        *this = result;
        return *this;
    }

    friend constexpr Mat operator*(Mat lhs, const Mat& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    friend constexpr Mat<T, 3> operator*(const Mat<T, 3>& a, const Mat<T, 3>& b)
    {
        return {{a[0] * b[0].x + a[1] * b[0].y, a[0] * b[1].x + a[1] * b[1].y}};
    }

    constexpr Mat& operator*=(value_type scalar)
    {
        mat[0] *= scalar;
        mat[1] *= scalar;
        return *this;
    }

    constexpr Mat& operator/=(value_type scalar)
    {
        mat[0] /= scalar;
        mat[1] /= scalar;
        return *this;
    }

    friend constexpr Mat operator+(Mat lhs, const Mat& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Mat operator-(Mat lhs, const Mat& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr Mat operator*(Mat lhs, value_type s)
    {
        lhs *= s;
        return lhs;
    }
    friend constexpr Mat operator*(value_type s, Mat rhs)
    {
        rhs *= s;
        return rhs;
    }
    friend constexpr Mat operator/(Mat lhs, value_type s)
    {
        lhs /= s;
        return lhs;
    }

    template <typename Q>
    friend constexpr auto operator*(Mat lhs, Q s)
    {
        using upcast_type = Internal::promote_t<Q>;
        Mat<Vec<upcast_type, row_count>, column_count> out{};
        for (size_t i = 0; i < row_count; ++i)
        {
            out[0][i] = lhs[0][i] * s;
            out[1][i] = lhs[1][i] * s;
        }
        return out;
    }
    template <typename Q>
    friend constexpr auto operator*(Q s, Mat rhs)
    {
        rhs *= s;
        return rhs;
    }
    template <typename Q>
    friend constexpr auto operator/(Mat lhs, Q s)
    {
        using upcast_type = Internal::promote_t<Q>;
        auto val          = static_cast<upcast_type>(s);
        if (val < static_cast<upcast_type>(1E-6) && val > static_cast<upcast_type>(-1E-6))
        {
            return Mat{};
        }
        upcast_type inv = static_cast<upcast_type>(1) / s;
        return lhs * s;
    }

    friend constexpr bool operator==(const Mat& a, const Mat& b)
    {
        return a.mat[0] == b.mat[0] && a.mat[1] == b.mat[1];
    }

    static constexpr Mat<Vec<value_type, 2>, 2> Identity()
    {
        return Mat<Vec<value_type, 2>, 2>{Vec<value_type, 2>{1, 0}, Vec<value_type, 2>{0, 1}};
    }

    static Mat<Vec<value_type, column_count>, row_count> Transpose(const Mat& m)
    {
        Mat<Vec<value_type, column_count>, row_count> result{};
        for (size_t i = 0; i < row_count; ++i)
        {
            result[i][0] = m[1][i];
            result[i][1] = m[0][i];
        }
        return result;
    }

    static value_type Determinant(const Mat& m)
        requires(column_count == row_count)
    {
        return m[0][0] * m[1][1] - m[1][0] * m[0][1];
    }

    static Mat Adjugate(const Mat& m)
        requires(column_count == row_count)
    {
        return Mat{m[1][1], -m[0][1], -m[1][0], m[0][0]};
    }

    static auto Inverse(const Mat& m)
        requires(column_count == row_count)
    {
        value_type oneOverDeterminant = static_cast<value_type>(1) / (+m[0][0] * m[1][1] - m[1][0] * m[0][1]);

        Mat inverse{+m[1][1] * oneOverDeterminant, -m[0][1] * oneOverDeterminant, -m[1][0] * oneOverDeterminant, +m[0][0] * oneOverDeterminant};

        return inverse;
    }
};

export using uchar2x2  = Mat<uchar2, 2>;
export using uchar3x2  = Mat<uchar3, 2>;
export using uchar4x2  = Mat<uchar4, 2>;
export using char2x2   = Mat<char2, 2>;
export using char3x2   = Mat<char3, 2>;
export using char4x2   = Mat<char4, 2>;
export using ushort2x2 = Mat<ushort2, 2>;
export using ushort3x2 = Mat<ushort3, 2>;
export using ushort4x2 = Mat<ushort4, 2>;
export using short2x2  = Mat<short2, 2>;
export using short3x2  = Mat<short3, 2>;
export using short4x2  = Mat<short4, 2>;
export using uint2x2   = Mat<uint2, 2>;
export using uint3x2   = Mat<uint3, 2>;
export using uint4x2   = Mat<uint4, 2>;
export using int2x2    = Mat<int2, 2>;
export using int3x2    = Mat<int3, 2>;
export using int4x2    = Mat<int4, 2>;
export using float2x2  = Mat<float2, 2>;
export using float3x2  = Mat<float3, 2>;
export using float4x2  = Mat<float4, 2>;
export using double2x2 = Mat<double2, 2>;
export using double3x2 = Mat<double3, 2>;
export using double4x2 = Mat<double4, 2>;

template <typename T>
struct Mat<T, 3>
{
    using column_type                    = T;
    using value_type                     = T::value_type; // get scalar type from Vec
    static constexpr size_t column_count = 3;
    static constexpr size_t row_count    = T::element_count;

    column_type mat[column_count]{};

    constexpr const column_type& operator[](size_t i) const
    {
        // assert(i < column_count);
        return mat[i];
    }

    constexpr column_type& operator[](size_t i)
    {
        // assert(i < column_count);
        return mat[i];
    }

    constexpr Mat operator+() const
    {
        return *this;
    }
    constexpr Mat operator-() const
    {
        return Mat(-mat[0], -mat[1], -mat[2]);
    }

    constexpr Mat& operator+=(const Mat& rhs) noexcept
    {
        mat[0] += rhs.mat[0];
        mat[1] += rhs.mat[1];
        mat[2] += rhs.mat[2];
        return *this;
    }

    constexpr Mat& operator-=(const Mat& rhs) noexcept
    {
        mat[0] -= rhs.mat[0];
        mat[1] -= rhs.mat[1];
        mat[2] -= rhs.mat[2];
        return *this;
    }

    constexpr Mat& operator*=(value_type scalar) noexcept
    {
        mat[0] *= scalar;
        mat[1] *= scalar;
        mat[2] *= scalar;
        return *this;
    }

    constexpr Mat& operator/=(value_type scalar) noexcept
    {
        using upcast_type  = Internal::promote_t<value_type>;
        upcast_type inv    = upcast_type(1) / static_cast<upcast_type>(scalar);
        mat[0]            *= inv;
        mat[1]            *= inv;
        mat[2]            *= inv;
        return *this;
    }

    friend constexpr Mat operator*(Mat m, value_type scalar) noexcept
    {
        m *= scalar;
        return m;
    }
    friend constexpr Mat operator*(value_type scalar, Mat m) noexcept
    {
        m *= scalar;
        return m;
    }

    friend constexpr Mat operator/(Mat m, value_type scalar) noexcept
    {
        m /= scalar;
        return m;
    }

    friend constexpr Mat operator+(Mat lhs, const Mat& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Mat operator-(Mat lhs, const Mat& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }
    constexpr auto operator*=(const Mat& other) noexcept
    {
        Mat result{};
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t r = 0; r < 3; ++r)
            {
                result[i][r] = mat[0][r] * other[i][0] + mat[1][r] * other[i][1] + mat[2][r] * other[i][2];
            }
        }
        *this = result;
        return *this;
    }

    friend constexpr Mat operator*(Mat lhs, const Mat& rhs) noexcept
        requires(column_count != row_count)
    {
        lhs *= rhs;
        return lhs;
    }

    friend constexpr Mat operator*(const Mat& A, const Mat& B) noexcept
        requires(column_count == row_count)
    {
        return Mat{
            A[0] * B[0][0] + A[1] * B[0][1] + A[2] * B[0][2],
            A[0] * B[1][0] + A[1] * B[1][1] + A[2] * B[1][2],
            A[0] * B[2][0] + A[1] * B[2][1] + A[2] * B[2][2],
        };
    }

    template <typename Q>
    friend constexpr auto operator*(const Mat& lhs, const Vec<Q, column_count>& v)
    {
        using upcast_type = Internal::promote_t<Q>;
        Vec<upcast_type, row_count> out{};
        for (size_t r = 0; r < row_count; ++r)
        {
            upcast_type sum{};
            for (size_t c = 0; c < column_count; ++c)
            {
                sum += static_cast<upcast_type>(lhs[c][r]) * static_cast<upcast_type>(v[c]);
            }

            out[r] = sum;
        }

        return out;
    }

    template <typename Q>
    friend constexpr auto operator*(Mat lhs, Q s)
    {
        using upcast_type = Internal::promote_t<Q>;
        Mat<Vec<upcast_type, row_count>, column_count> out{};
        for (size_t i = 0; i < row_count; ++i)
        {
            out[0][i] = lhs[0][i] * s;
            out[1][i] = lhs[1][i] * s;
            out[2][i] = lhs[2][i] * s;
        }
        return out;
    }

    template <typename Q>
    friend constexpr auto operator*(Q s, Mat rhs)
    {
        rhs *= s;
        return rhs;
    }

    template <typename Q>
    friend constexpr auto operator/(Mat lhs, Q s) noexcept
    {
        using upcast_type = Internal::promote_t<Q>;
        auto val          = static_cast<upcast_type>(s);
        if (val < static_cast<upcast_type>(1E-6) && val > static_cast<upcast_type>(-1E-6))
        {
            return Mat{};
        }
        upcast_type inv = static_cast<upcast_type>(1) / s;
        return lhs * inv;
    }

    friend constexpr bool operator==(const Mat& a, const Mat& b) noexcept
    {
        return a.mat[0] == b.mat[0] && a.mat[1] == b.mat[1] && a.mat[2] == b.mat[2];
    }

    static constexpr Mat<Vec<value_type, 3>, 3> Identity() noexcept
    {
        return Mat<Vec<value_type, 3>, 3>{Vec<value_type, 3>{1, 0, 0}, Vec<value_type, 3>{0, 1, 0}, Vec<value_type, 3>{0, 0, 1}};
    }

    static constexpr Mat<Vec<value_type, row_count>, column_count> Transpose(const Mat& m) noexcept
    {
        Mat<Vec<value_type, row_count>, column_count> result{};
        for (size_t c = 0; c < column_count; ++c)
        {
            for (size_t r = 0; r < row_count; ++r)
            {
                result[r][c] = m[c][r];
            }
        }
        return result;
    }

    static value_type Determinant(const Mat& m)
        requires(column_count == row_count)
    {
        /*
         [a, b, c]     [00, 01, 02]
         [d, e, f] ==> [10, 11, 12]
         [g, h, i]     [20, 21, 22]
        */

        return +m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
    }

    static Mat Adjugate(const Mat& m)
        requires(column_count == row_count)
    {
        /*
         [a, b, c]     [00, 01, 02]
         [d, e, f] ==> [10, 11, 12]
         [g, h, i]     [20, 21, 22]

        A = (ei - fh)  => 11*22-12*21
        B = -(di - fg) => -10*22+12*20
        C = (dh - eg)  => 10*20-11*20

        D = -(bi - ch) => -01*22+02*21
        E = (ai - cg)  => 00*22-02*20
        F = -(ah - bg) => -00*21+01*20

        G = (bf - ce)  => 01*12-02*11
        H = -(af - cd) => -00*12+02*10
        I = (ae - bd)  => 00*11-01*10
        */
        using mat2           = Mat<Vec<value_type, 2>, 2>;
        value_type const m00 = mat2{m[1][1], m[2][1], m[1][2], m[2][2]}.Determinant();
        value_type const m01 = mat2{m[0][1], m[2][1], m[0][2], m[2][2]}.Determinant();
        value_type const m02 = mat2{m[0][1], m[1][1], m[0][2], m[1][2]}.Determinant();

        value_type const m10 = mat2{m[1][0], m[2][0], m[1][2], m[2][2]}.Determinant();
        value_type const m11 = mat2{m[0][0], m[2][0], m[0][2], m[2][2]}.Determinant();
        value_type const m12 = mat2{m[0][0], m[1][0], m[0][2], m[1][2]}.Determinant();

        value_type const m20 = mat2{m[1][0], m[2][0], m[1][1], m[2][1]}.Determinant();
        value_type const m21 = mat2{m[0][0], m[2][0], m[0][1], m[2][1]}.Determinant();
        value_type const m22 = mat2{m[0][0], m[1][0], m[0][1], m[1][1]}.Determinant();

        return Mat{+m00, -m01, +m02, -m10, +m11, -m12, +m20, -m21, +m22};
    }

    static constexpr auto Inverse(const Mat& m)
        requires(column_count == row_count)
    {
        value_type oneOverDeterminant = static_cast<value_type>(1)
            / (+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

        Mat inverse;
        inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]);
        inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]);
        inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]);
        inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]);
        inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]);
        inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]);
        inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]);
        inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]);
        inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]);

        inverse *= oneOverDeterminant;
        return inverse;
    }
};

export using uchar2x3  = Mat<uchar2, 3>;
export using uchar3x3  = Mat<uchar3, 3>;
export using uchar4x3  = Mat<uchar4, 3>;
export using char2x3   = Mat<char2, 3>;
export using char3x3   = Mat<char3, 3>;
export using char4x3   = Mat<char4, 3>;
export using ushort2x3 = Mat<ushort2, 3>;
export using ushort3x3 = Mat<ushort3, 3>;
export using ushort4x3 = Mat<ushort4, 3>;
export using short2x3  = Mat<short2, 3>;
export using short3x3  = Mat<short3, 3>;
export using short4x3  = Mat<short4, 3>;
export using uint2x3   = Mat<uint2, 3>;
export using uint3x3   = Mat<uint3, 3>;
export using uint4x3   = Mat<uint4, 3>;
export using int2x3    = Mat<int2, 3>;
export using int3x3    = Mat<int3, 3>;
export using int4x3    = Mat<int4, 3>;
export using float2x3  = Mat<float2, 3>;
export using float3x3  = Mat<float3, 3>;
export using float4x3  = Mat<float4, 3>;
export using double2x3 = Mat<double2, 3>;
export using double3x3 = Mat<double3, 3>;
export using double4x3 = Mat<double4, 3>;

template <typename T>
struct Mat<T, 4>
{
    using column_type                    = T;
    using value_type                     = T::value_type; // get scalar type from Vec
    static constexpr size_t column_count = 4;
    static constexpr size_t row_count    = T::element_count;

    column_type mat[column_count]{column_type{0}, column_type{0}, column_type{0}, column_type{0}};

    static constexpr auto RotationAxis(T axis, value_type angle)
    {
        using upcast_type = Internal::promote_t<value_type>;
        value_type c      = std::cos(static_cast<upcast_type>(angle));
        value_type s      = std::sin(static_cast<upcast_type>(angle));
        value_type t      = upcast_type(1) - c;

        using vec_type = Vec<value_type, row_count>;
        return Mat<vec_type, column_count>{vec_type{t * axis.x * axis.x + c, t * axis.x * axis.y - s * axis.z, t * axis.x * axis.z + s * axis.y, 0},
                                           vec_type{t * axis.x * axis.y + s * axis.z, t * axis.y * axis.y + c, t * axis.y * axis.z - s * axis.x, 0},
                                           vec_type{t * axis.x * axis.z - s * axis.y, t * axis.y * axis.z + s * axis.x, t * axis.z * axis.z + c, 0}, vec_type{0, 0, 0, 0}};
    }

    static constexpr Mat FromPosition(const T& v) noexcept
    {
        return Mat{T{1, 0, 0, 0}, T{0, 1, 0, 0}, T{0, 0, 1, 0}, T{v.x, v.y, v.z, 1}};
    }
    static constexpr Mat FromPosition(const Vec<value_type, 3>& v) noexcept
    {
        return Mat{T{1, 0, 0, 0}, T{0, 1, 0, 0}, T{0, 0, 1, 0}, T{v.x, v.y, v.z, 1}};
    }
    constexpr const column_type& operator[](size_t i) const noexcept
    {
        // assert(i < column_count);
        return mat[i];
    }

    constexpr column_type& operator[](size_t i) noexcept
    {
        // assert(i < column_count);
        return mat[i];
    }

    constexpr Mat operator+() const noexcept
    {
        return *this;
    }
    constexpr Mat operator-() const noexcept
    {
        return Mat(-mat[0], -mat[1], -mat[2], -mat[3]);
    }

    constexpr Mat& operator+=(const Mat& rhs) noexcept
    {
        mat[0] += rhs.mat[0];
        mat[1] += rhs.mat[1];
        mat[2] += rhs.mat[2];
        mat[3] += rhs.mat[3];
        return *this;
    }

    constexpr Mat& operator-=(const Mat& rhs) noexcept
    {
        mat[0] -= rhs.mat[0];
        mat[1] -= rhs.mat[1];
        mat[2] -= rhs.mat[2];
        mat[3] -= rhs.mat[3];
        return *this;
    }

    constexpr auto operator*=(const Mat& m) noexcept
    {
        Mat result{};

        for (size_t i = 0; i < column_count; ++i)
        {
            for (size_t k = 0; k < column_count; ++k)
            {
                for (size_t j = 0; j < row_count; ++j)
                {
                    result[i][j] += mat[i][k] * m[k][j];
                }
            }
        }

        *this = result;
        return result;
    }

    friend constexpr Mat operator*(Mat lhs, const Mat& rhs) noexcept
        requires(column_count != row_count)
    {
        lhs *= rhs;
        return lhs;
    }

    friend constexpr column_type operator*(Mat lhs, const column_type& rhs) noexcept
    {
        column_type result{};
        for (size_t i = 0; i < column_count; ++i)
        {
            for (size_t j = 0; j < row_count; ++j)
            {
                result[i] += lhs[i][j] * rhs[j];
            }
        }

        return result;
    }

    friend constexpr Mat<Vec<value_type, 4>, 4> operator*(const Mat<Vec<value_type, 4>, 4>& a, const Mat<Vec<value_type, 4>, 4>& b) noexcept
    {
        return {{a[0] * b[0].x + a[1] * b[0].y + a[2] * b[0].z + a[3] * b[0].w, a[0] * b[1].x + a[1] * b[1].y + a[2] * b[1].z + a[3] * b[1].w,
                 a[0] * b[2].x + a[1] * b[2].y + a[2] * b[2].z + a[3] * b[2].w, a[0] * b[3].x + a[1] * b[3].y + a[2] * b[3].z + a[3] * b[3].w}};
    }

    constexpr Mat& operator*=(value_type scalar) noexcept
    {
        mat[0] *= scalar;
        mat[1] *= scalar;
        mat[2] *= scalar;
        mat[3] *= scalar;
        return *this;
    }

    constexpr Mat& operator/=(value_type scalar) noexcept
    {
        mat[0] /= scalar;
        mat[1] /= scalar;
        mat[2] /= scalar;
        mat[3] /= scalar;
        return *this;
    }

    friend constexpr Mat operator+(Mat lhs, const Mat& rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    friend constexpr Mat operator-(Mat lhs, const Mat& rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    template <typename Q>
    friend constexpr auto operator*(Mat lhs, Q s) noexcept
    {
        using upcast_type = Internal::promote_t<Q>;
        Mat<Vec<upcast_type, row_count>, column_count> out{};
        for (size_t i = 0; i < row_count; ++i)
        {
            out[0][i] = lhs[0][i] * s;
            out[1][i] = lhs[1][i] * s;
            out[2][i] = lhs[2][i] * s;
            out[3][i] = lhs[3][i] * s;
        }
        return out;
    }

    template <typename Q>
    friend constexpr auto operator*(Q s, Mat rhs) noexcept
    {
        rhs *= s;
        return rhs;
    }

    friend constexpr Mat operator*(Mat lhs, value_type scalar) noexcept
    {
        lhs *= scalar;
        return lhs;
    }

    template <typename Q>
    friend constexpr auto operator/(Mat lhs, Q s) noexcept
    {
        using upcast_type = Internal::promote_t<Q>;
        auto val          = static_cast<upcast_type>(s);
        if (val < static_cast<upcast_type>(1E-6) && val > static_cast<upcast_type>(-1E-6))
        {
            return Mat{};
        }
        upcast_type inv = static_cast<upcast_type>(1) / s;
        return lhs * s;
    }

    friend constexpr bool operator==(const Mat& a, const Mat& b) noexcept
    {
        return a.mat[0] == b.mat[0] && a.mat[1] == b.mat[1] && a.mat[2] == b.mat[2] && a.mat[3] == b.mat[3];
    }

    static constexpr auto Identity() noexcept
    {
        return Mat{column_type{1, 0, 0, 0}, column_type{0, 1, 0, 0}, column_type{0, 0, 1, 0}, column_type{0, 0, 0, 1}};
    }

    static Mat<Vec<value_type, column_count>, row_count> Transpose(const Mat& m) noexcept
    {
        Mat<Vec<value_type, column_count>, row_count> result{};
        for (size_t i = 0; i < row_count; ++i)
        {
            result[i][0] = m[0][i];
            result[i][1] = m[1][i];
            result[i][2] = m[2][i];
            result[i][3] = m[3][i];
        }
        return result;
    }

    static auto LookAt(const Vec<value_type, 3>& eye, const Vec<value_type, 3>& center, const Vec<value_type, 3>& up = direction::Up<value_type, 3>()) noexcept
    {
        using vec_type = Vec<value_type, 3>;
        vec_type f     = vec_type::Normalize(center - eye);           // forward
        vec_type s     = vec_type::Normalize(vec_type::Cross(f, up)); // right
        vec_type u     = vec_type::Cross(s, f);                       // true up

        Mat result{};
        result[0] = column_type(s.x, u.x, -f.x, 0);
        result[1] = column_type(s.y, u.y, -f.y, 0);
        result[2] = column_type(s.z, u.z, -f.z, 0);
        result[3] = column_type(-vec_type::Dot(s, eye), -vec_type::Dot(u, eye), vec_type::Dot(f, eye), 1);

        result[0][0] = s.x;
        result[0][1] = -f.x;
        result[0][2] = -f.x;
        result[1][0] = s.y;
        result[1][1] = -f.y;
        result[1][2] = u.y;
        result[2][0] = s.z;
        result[2][1] = -f.z;
        result[2][2] = u.z;
        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(f, eye);
        result[3][2] = dot(u, eye);
        return result;
    }

    static auto Perspective(const float fovYRadians, const float aspectRatio, const float nearPlane, const float farPlane) noexcept
    {
        value_type const tanHalfFovy = tan(fovYRadians / static_cast<value_type>(2));

        Mat result{};
        result[0][0] = static_cast<value_type>(1) / (aspectRatio * tanHalfFovy);
        result[1][1] = static_cast<value_type>(1) / (tanHalfFovy);
        result[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result[2][3] = -static_cast<value_type>(1);
        result[3][2] = -(static_cast<value_type>(2) * farPlane * nearPlane) / (farPlane - nearPlane);
        return result;
    }

    static auto Ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) noexcept
    {
        Mat result(1);
        result[0][0] = static_cast<value_type>(2) / (right - left);
        result[1][1] = static_cast<value_type>(2) / (top - bottom);
        result[2][2] = -static_cast<value_type>(1) / (far - near);
        result[3][0] = -(right + left) / (right - left);
        result[3][1] = -(top + bottom) / (top - bottom);
        result[3][2] = -near / (far - near);
        return result;
    }

    static value_type Determinant(const Mat& m) noexcept
        requires(column_count == row_count)
    {
        value_type subFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
        value_type subFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
        value_type subFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
        value_type subFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
        value_type subFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
        value_type subFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

        Vec<T, 4> detCof(+(m[1][1] * subFactor00 - m[1][2] * subFactor01 + m[1][3] * subFactor02), -(m[1][0] * subFactor00 - m[1][2] * subFactor03 + m[1][3] * subFactor04),
                         +(m[1][0] * subFactor01 - m[1][1] * subFactor03 + m[1][3] * subFactor05), -(m[1][0] * subFactor02 - m[1][1] * subFactor04 + m[1][2] * subFactor05));

        return m[0][0] * detCof[0] + m[0][1] * detCof[1] + m[0][2] * detCof[2] + m[0][3] * detCof[3];
    }

    static Mat Adjugate(const Mat& m) noexcept
        requires(column_count == row_count)
    {
        using mat3           = Mat<Vec<value_type, 3>, 3>;
        value_type const m00 = mat3{m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]}.Determinant();
        value_type const m01 = mat3{m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]}.Determinant();
        value_type const m02 = mat3{m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]}.Determinant();
        value_type const m03 = mat3{m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]}.Determinant();

        value_type const m10 = mat3{m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]}.Determinant();
        value_type const m11 = mat3{m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]}.Determinant();
        value_type const m12 = mat3{m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]}.Determinant();
        value_type const m13 = mat3{m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]}.Determinant();

        value_type const m20 = mat3{m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]}.Determinant();
        value_type const m21 = mat3{m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]}.Determinant();
        value_type const m22 = mat3{m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]}.Determinant();
        value_type const m23 = mat3{m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]}.Determinant();

        value_type const m30 = mat3{m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]}.Determinant();
        value_type const m31 = mat3{m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]}.Determinant();
        value_type const m32 = mat3{m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]}.Determinant();
        value_type const m33 = mat3{m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]}.Determinant();

        return Mat(+m00, -m10, +m20, -m30, -m01, +m11, -m21, +m31, +m02, -m12, +m22, -m32, -m03, +m13, -m23, +m33);
    }

    static auto Inverse(const Mat& m) noexcept
        requires(column_count == row_count)
    {
        value_type coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
        value_type coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
        value_type coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

        value_type coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
        value_type coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
        value_type coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

        value_type coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
        value_type coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
        value_type coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

        value_type coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
        value_type coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
        value_type coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

        value_type coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
        value_type coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
        value_type coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

        value_type coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
        value_type coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
        value_type coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

        column_type fac0{coef00, coef00, coef02, coef03};
        column_type fac1{coef04, coef04, coef06, coef07};
        column_type fac2{coef08, coef08, coef10, coef11};
        column_type fac3{coef12, coef12, coef14, coef15};
        column_type fac4{coef16, coef16, coef18, coef19};
        column_type fac5{coef20, coef20, coef22, coef23};

        column_type Vec0{m[1][0], m[0][0], m[0][0], m[0][0]};
        column_type Vec1{m[1][1], m[0][1], m[0][1], m[0][1]};
        column_type Vec2{m[1][2], m[0][2], m[0][2], m[0][2]};
        column_type Vec3{m[1][3], m[0][3], m[0][3], m[0][3]};

        column_type inv0{Vec1 * fac0 - Vec2 * fac1 + Vec3 * fac2};
        column_type inv1{Vec0 * fac0 - Vec2 * fac3 + Vec3 * fac4};
        column_type inv2{Vec0 * fac1 - Vec1 * fac3 + Vec3 * fac5};
        column_type inv3{Vec0 * fac2 - Vec1 * fac4 + Vec2 * fac5};

        constexpr column_type signA(+1, -1, +1, -1);
        constexpr column_type signB(-1, +1, -1, +1);

        Mat inverse{inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

        column_type row0{inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]};

        column_type dot0{m[0] * row0};
        value_type dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

        value_type oneOverDeterminant = static_cast<value_type>(1) / dot1;

        return inverse * oneOverDeterminant;
    }
};

export using uchar2x4  = Mat<uchar2, 4>;
export using uchar3x4  = Mat<uchar3, 4>;
export using uchar4x4  = Mat<uchar4, 4>;
export using char2x4   = Mat<char2, 4>;
export using char3x4   = Mat<char3, 4>;
export using char4x4   = Mat<char4, 4>;
export using ushort2x4 = Mat<ushort2, 4>;
export using ushort3x4 = Mat<ushort3, 4>;
export using ushort4x4 = Mat<ushort4, 4>;
export using short2x4  = Mat<short2, 4>;
export using short3x4  = Mat<short3, 4>;
export using short4x4  = Mat<short4, 4>;
export using uint2x4   = Mat<uint2, 4>;
export using uint3x4   = Mat<uint3, 4>;
export using uint4x4   = Mat<uint4, 4>;
export using int2x4    = Mat<int2, 4>;
export using int3x4    = Mat<int3, 4>;
export using int4x4    = Mat<int4, 4>;
export using float2x4  = Mat<float2, 4>;
export using float3x4  = Mat<float3, 4>;
export using float4x4  = Mat<float4, 4>;
export using double2x4 = Mat<double2, 4>;
export using double3x4 = Mat<double3, 4>;
export using double4x4 = Mat<double4, 4>;

template <typename T>
struct Quat
{
    T a{};
    T e23{};
    T e31{};
    T e12{};

    /// Returns the identity quaternion (no rotation)
    /// @return Quaternion representing identity (1, 0, 0, 0)
    static constexpr Quat Identity()
    {
        return {T(1), T(0), T(0), T(0)};
    }

    /// Creates a quaternion from axis-angle representation
    /// @param axis Normalized rotation axis vector
    /// @param angle Rotation angle in radians
    /// @return Quaternion representing the rotation
    static constexpr auto FromAxisAngle(const Vec<T, 3>& axis, T angle)
    {
        using upcast_t  = Internal::promote_t<T>;
        upcast_t half   = static_cast<upcast_t>(angle) * upcast_t(0.5);
        upcast_t upcast = std::sin(half);
        return Quat<upcast_t>{std::cos(half), axis.x * upcast, axis.y * upcast, axis.z * upcast};
    }

    /// Creates a quaternion from Euler angles (roll, pitch, yaw)
    /// @param roll Rotation around X-axis in radians
    /// @param pitch Rotation around Y-axis in radians
    /// @param yaw Rotation around Z-axis in radians
    /// @return Quaternion representing the combined rotation
    static constexpr Quat ToQuaternion(double roll, double pitch, double yaw) noexcept
    {
        const T cr = std::cos(roll / T{2});
        const T sr = std::sin(roll / T{2});
        const T cp = std::cos(pitch / T{2});
        const T sp = std::sin(pitch / T{2});
        const T cy = std::cos(yaw / T{2});
        const T sy = std::sin(yaw / T{2});

        return {cr * cp * cy + sr * sp * sy, sr * cp * cy - cr * sp * sy, cr * sp * cy + sr * cp * sy, cr * cp * sy - sr * sp * cy};
    }

    static constexpr Quat ToQuaternion(const Mat<Vec<T, 3>, 3>& m) noexcept
    {
        T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
        T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
        T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
        T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

        int biggestIndex           = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex             = 1;
        }
        if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex             = 2;
        }
        if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex             = 3;
        }

        T biggestVal = std::sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1)) * static_cast<T>(0.5);
        T mult       = static_cast<T>(0.25) / biggestVal;

        switch (biggestIndex)
        {
        case 0: return Quat(biggestVal, (m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult);
        case 1: return Quat((m[1][2] - m[2][1]) * mult, biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult);
        case 2: return Quat((m[2][0] - m[0][2]) * mult, (m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult);
        case 3: return Quat((m[0][1] - m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal);
        default: // Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
            // assert(false);
            return Quat(1, 0, 0, 0);
        }
    }

    /// Creates a quaternion that orients from one point to look at another
    /// @param from Starting position (camera position)
    /// @param to Target position to look at
    /// @param up Up direction (default: Z-up for this coordinate system)
    /// @return Quaternion representing the look-at orientation
    /// @note For Y-forward, Z-up, X-right coordinate system. Forward direction is along +Y axis
    static constexpr Quat LookAt(const Vec<T, 3>& from, const Vec<T, 3>& to, [[maybe_unused]] const Vec<T, 3> up = direction::Up<T, 3>())
    /*{
        Vec<T, 3> direction = to - from;
        constexpr T epsilon{T(1E-6)};
        if (direction.LengthSqr() <= epsilon)
        {
            return Identity();
        }
        direction = Vec<T, 3>::Normalize(direction);
        Mat<Vec<T, 3>, 3> result{};
        result[2] = -direction;

        Vec<T, 3> right = Vec<T, 3>::Cross(up, result[2]);
        result[0] = right * Rsqrt(std::max(T(1E-6), right.LengthSqr()));
        result[1] = Vec<T, 3>::Cross(result[2], result[0]);

        return ToQuaternion(result);
    }*/
    {
        Vec<T, 3> lookAtVector = Vec<T, 3>::Normalize(to - from);
        Vec<T, 3> lookAtXZ     = Vec<T, 3>::Normalize(lookAtVector - direction::Up<T, 3>() * lookAtVector.y);

        Quat quatPitch = Quat::Identity();
        Quat quatYaw   = Quat::Identity();

        // Create yaw rotation
        static constexpr T epsilon{T{1E-6}};
        if (lookAtXZ.x > epsilon || lookAtXZ.x < -epsilon)
        {
            const T halfAngleYaw   = std::acos(Vec<T, 3>::Dot(direction::Forward<T, 3>(), lookAtXZ)) * T(0.5);
            Vec<T, 3> rotationAxis = lookAtVector.x < T(0) ? direction::Up<T, 3>() : direction::Down<T, 3>();
            Vec<T, 3> quatYawImg   = rotationAxis * std::sin(halfAngleYaw);

            quatYaw = Quat(std::cos(halfAngleYaw), quatYawImg.x, quatYawImg.y, quatYawImg.z);
        }
        else if (lookAtXZ.z < T(0))
        {
            quatYaw = Quat(std::cos(std::numbers::pi_v<T> * T(0.5)), T(0), std::sin(std::numbers::pi_v<T> * T(0.5)), T(0));
        }

        // Create pitch rotation
        if (lookAtVector.y > epsilon || lookAtVector.y < -epsilon)
        {
            if (lookAtXZ.LengthSqr() > epsilon)
            {
                const T halfAnglePitch = std::acos(Vec<T, 3>::Dot(lookAtXZ, lookAtVector)) * T(0.5);
                Vec<T, 3> rotationAxis = lookAtVector.y > T(0) ? direction::Right<T, 3>() : direction::Left<T, 3>();
                Vec<T, 3> quatPitchImg = rotationAxis * std::sin(halfAnglePitch);

                quatPitch = Quat(std::cos(halfAnglePitch), quatPitchImg.x, quatPitchImg.y, quatPitchImg.z);
            }
            else
            {
                quatPitch = Quat(std::cos(std::numbers::pi_v<T> * T(0.25)), std::sin(std::numbers::pi_v<T> * T(0.25)), T(0), T(0));
            }
        }

        return quatPitch * quatYaw;
    }

    /// Creates a quaternion representing rotation around the X-axis
    /// @param angle Rotation angle in radians
    /// @return Quaternion representing X-axis rotation
    static constexpr Quat RotateX(T angle)
    {
        T half = angle * T(0.5);
        return {std::cos(half), std::sin(half), T(0), T(0)};
    }

    /// Creates a quaternion representing rotation around the Y-axis
    /// @param angle Rotation angle in radians
    /// @return Quaternion representing Y-axis rotation
    static constexpr Quat RotateY(T angle)
    {
        T half = angle * T(0.5);
        return {std::cos(half), T(0), std::sin(half), T(0)};
    }

    /// Creates a quaternion representing rotation around the Z-axis
    /// @param angle Rotation angle in radians
    /// @return Quaternion representing Z-axis rotation
    static constexpr Quat RotateZ(T angle)
    {
        T half = angle * T(0.5);
        return {std::cos(half), T(0), T(0), std::sin(half)};
    }

    /// Exponential map: converts a bivector (rotation axis * angle) to a quaternion
    /// @param bivector Rotation axis scaled by angle
    /// @return Quaternion representing the rotation
    static constexpr Quat Exp(const Vec<T, 3>& bivector)
    {
        T angle = Vec<T, 3>::Length(bivector);

        static constexpr T epsilon{T(1E-6)};
        if (angle < epsilon)
        {
            return Identity();
        }

        T s = std::sin(angle) / angle;
        return {std::cos(angle), bivector.x * s, bivector.y * s, bivector.z * s};
    }

    /// Convert from engine space (Z-up, Y-forward) to OpenGL space (Y-up, -Z-forward)
    /// @param engineQuat quaternion used internally
    /// @return quaternion used in the renderer
    static constexpr Quat FromEngineSpace(const Quat& engineQuat)
    {
        // Basis change: X->X, Y->-Z, Z->Y
        // This is a 90° rotation around X-axis
        static constexpr T angle = std::numbers::sqrt2_v<T> / T(2);
        static constexpr Quat basisChange{angle, angle, T(0), T(0)};
        return basisChange * engineQuat;
    }

    /// Computes the dot product of two quaternions
    /// @param a First quaternion
    /// @param b Second quaternion
    /// @return Scalar dot product
    static constexpr T Dot(const Quat& a, const Quat& b)
    {
        return a.a * b.a + a.e23 * b.e23 + a.e31 * b.e31 + a.e12 * b.e12;
    }

    /// Computes the length (magnitude) of a quaternion
    /// @param q Input quaternion
    /// @return Length of the quaternion
    static constexpr T Length(const Quat& q)
    {
        return q.Length();
    }

    /// Returns the reverse (conjugate with negated bivector part) of a quaternion
    /// @param q Input quaternion
    /// @return Reversed quaternion
    static constexpr Quat Reverse(const Quat& q)
    {
        return q.Reverse();
    }

    /// Normalizes a quaternion to unit length
    /// @param q Input quaternion
    /// @return Normalized quaternion
    static constexpr Quat Normalize(const Quat& q)
    {
        return q.Normalize();
    }

    /// Multiplies two quaternions (combines rotations)
    /// @param a First quaternion
    /// @param b Second quaternion
    /// @return Product quaternion (applies a, then b)
    static constexpr Quat Multiply(const Quat& a, const Quat& b)
    {
        return a * b;
    }

    /// Rotates a vector by a quaternion
    /// @param q Rotation quaternion
    /// @param v Vector to rotate
    /// @return Rotated vector
    static constexpr Vec<T, 3> RotateVector(const Quat& q, const Vec<T, 3>& v)
    {
        return q * v;
    }

    /// Converts a quaternion to a 3x3 rotation matrix
    /// @param q Input quaternion
    /// @return 3x3 rotation matrix
    static constexpr Mat<Vec<T, 3>, 3> ToMat3(const Quat& q)
    {
        return q.ToMat3();
    }

    /// Computes the angular difference between two quaternions
    /// @param a First quaternion
    /// @param b Second quaternion
    /// @return Angle in radians between the two rotations
    static constexpr T AngleBetween(const Quat& a, const Quat& b)
    {
        T dot = Dot(a, b);
        return T(2) * std::acos(std::min(std::abs(dot), T(1)));
    }

    // ========================================================================
    // Interpolation Methods
    // ========================================================================

    /// Spherical linear interpolation between two quaternions
    /// @param a Start quaternion
    /// @param b End quaternion
    /// @param t Interpolation parameter [0, 1]
    /// @return Interpolated quaternion with constant angular velocity
    static constexpr Quat Slerp(const Quat& a, const Quat& b, T t)
    {
        T dot = Dot(a, b);

        Quat bAdj = dot < T(0) ? Quat{-b.a, -b.e23, -b.e31, -b.e12} : b;
        dot       = dot < T(0) ? -dot : dot;

        static constexpr T delta{T{1} - T{1E-6}};
        if (dot > delta)
        {
            // Linear interpolation for close quaternions
            return Quat{a.a + t * (bAdj.a - a.a), a.e23 + t * (bAdj.e23 - a.e23), a.e31 + t * (bAdj.e31 - a.e31), a.e12 + t * (bAdj.e12 - a.e12)}.Normalize();
        }

        T Theta    = std::acos(dot);
        T SinTheta = std::sin(Theta);
        T Wa       = std::sin((T(1) - t) * Theta) / SinTheta;
        T Wb       = std::sin(t * Theta) / SinTheta;

        return {Wa * a.a + Wb * bAdj.a, Wa * a.e23 + Wb * bAdj.e23, Wa * a.e31 + Wb * bAdj.e31, Wa * a.e12 + Wb * bAdj.e12};
    }

    /// Normalized linear interpolation (faster approximation of Slerp)
    /// @param a Start quaternion
    /// @param b End quaternion
    /// @param t Interpolation parameter [0, 1]
    /// @return Interpolated quaternion (non-constant angular velocity)
    static constexpr Quat Nlerp(const Quat& a, const Quat& b, T t)
    {
        T dot     = a.a * b.a + a.e23 * b.e23 + a.e31 * b.e31 + a.e12 * b.e12;
        Quat bAdj = dot < T(0) ? Quat{-b.a, -b.e23, -b.e31, -b.e12} : b;

        return Quat{a.a + t * (bAdj.a - a.a), a.e23 + t * (bAdj.e23 - a.e23), a.e31 + t * (bAdj.e31 - a.e31), a.e12 + t * (bAdj.e12 - a.e12)}.Normalize();
    }

    // ========================================================================
    // Instance Methods
    // ========================================================================

    /// Returns the reverse (conjugate with negated bivector components)
    /// @return Reversed quaternion
    constexpr Quat Reverse() const
    {
        return Quat{a, -e23, -e31, -e12};
    }

    /// Returns the conjugate (same as Reverse for quaternions)
    /// @return Conjugate quaternion
    constexpr Quat Conjugate() const
    {
        return {a, -e23, -e31, -e12};
    }

    /// Returns the inverse quaternion (reverses the rotation)
    /// @return Inverse quaternion
    /// @note For unit quaternions, this equals the conjugate
    constexpr Quat Inverse() const
    {
        T lenSq = LengthSqr();
        T inv   = T(1) / lenSq;
        return {a * inv, -e23 * inv, -e31 * inv, -e12 * inv};
    }

    /// Computes the squared length of the quaternion
    /// @return Squared magnitude
    constexpr T LengthSqr() const
    {
        return a * a + e23 * e23 + e31 * e31 + e12 * e12;
    }

    /// Computes the length (magnitude) of the quaternion
    /// @return Length
    constexpr T Length() const
    {
        return std::sqrt(LengthSqr());
    }

    /// Returns a normalized (unit length) version of this quaternion
    /// @return Normalized quaternion
    constexpr Quat Normalize() const
    {
        T lenSqr = LengthSqr();
        if (lenSqr == T(0))
        {
            return {T(1), T(0), T(0), T(0)};
        }

        T inv = T(1) / std::sqrt(lenSqr);
        return {a * inv, e23 * inv, e31 * inv, e12 * inv};
    }

    /// Logarithm: converts quaternion (rotor) to bivector (axis * angle)
    /// @return Bivector representing rotation axis scaled by half-angle
    constexpr Vec<T, 3> Log() const
    {
        T lenSq = LengthSqr();

        static constexpr T epsilon{T(1E-6)};
        if (lenSq < epsilon)
        {
            return {T(0), T(0), T(0)};
        }

        T len   = std::sqrt(lenSq);
        T angle = std::atan2(len, a);
        T s     = angle / len;
        return {e23 * s, e31 * s, e12 * s};
    }

    /// Raises the quaternion to a power (scales the rotation angle)
    /// @param exponent Power to raise to
    /// @return Quaternion raised to the given power
    constexpr Quat Pow(T exponent) const
    {
        return Exp(Log() * exponent);
    }

    // ========================================================================
    // Conversion Methods
    // ========================================================================

    /// Converts quaternion to axis-angle representation
    /// @param[out] axis Rotation axis (normalized)
    /// @param[out] angle Rotation angle in radians
    constexpr void ToAxisAngle(Vec<T, 3>& axis, T& angle) const
    {
        T lenSq = LengthSqr();

        static constexpr T epsilon{T(1E-6)};
        if (lenSq < epsilon)
        {
            axis  = {T(1), T(0), T(0)};
            angle = T(0);
            return;
        }

        T invLen = T(1) / std::sqrt(lenSq);
        axis     = {e23 * invLen, e31 * invLen, e12 * invLen};
        angle    = T(2) * std::atan2(std::sqrt(lenSq), a);
    }

    /// Converts quaternion to Euler angles (ZYX intrinsic, or XYZ extrinsic)
    /// @return Vector containing (roll, pitch, yaw) in radians
    constexpr Vec<T, 3> ToEuler() const
    {
        T sinrCosp = T(2) * (a * e23 + e31 * e12);
        T cosrCosp = T(1) - T(2) * (e23 * e23 + e31 * e31);
        T roll     = std::atan2(sinrCosp, cosrCosp);

        T sinp  = T(2) * (a * e31 - e12 * e23);
        T pitch = std::abs(sinp) >= T(1) ? std::copysign(T(std::numbers::pi_v<T>) / T(2), sinp) : std::asin(sinp);

        T sinyCosp = T(2) * (a * e12 + e23 * e31);
        T cosyCosp = T(1) - T(2) * (e31 * e31 + e12 * e12);
        T yaw      = std::atan2(sinyCosp, cosyCosp);

        return {roll, pitch, yaw};
    }

    /// Converts quaternion to a 3x3 rotation matrix
    /// @return 3x3 rotation matrix
    constexpr Mat<Vec<T, 3>, 3> ToMat3() const
    {
        const T xx = e23 * e23;
        const T yy = e31 * e31;
        const T zz = e12 * e12;
        const T xy = e23 * e31;
        const T xz = e23 * e12;
        const T yz = e31 * e12;
        const T wx = a * e23;
        const T wy = a * e31;
        const T wz = a * e12;

        return Mat<Vec<T, 3>, 3>{T{1 - 2 * (yy + zz)}, T{2 * (xy - wz)}, T{2 * (xz + wy)}, T{2 * (xy + wz)},    T{1 - 2 * (xx + zz)},
                                 T{2 * (yz - wx)},     T{2 * (xz - wy)}, T{2 * (yz + wx)}, T{1 - 2 * (xx + yy)}};
    }

    /// Converts quaternion to a 4x4 homogeneous rotation matrix
    /// @return 4x4 rotation matrix with translation component set to zero
    constexpr Mat<Vec<T, 4>, 4> ToMat4() const
    {
        const T xx = e23 * e23;
        const T yy = e31 * e31;
        const T zz = e12 * e12;
        const T xy = e23 * e31;
        const T xz = e23 * e12;
        const T yz = e31 * e12;
        const T wx = a * e23;
        const T wy = a * e31;
        const T wz = a * e12;

        return Mat<Vec<T, 4>, 4>{Vec<T, 4>{1 - 2 * (yy + zz), 2 * (xy + wz), 2 * (xz - wy), 0}, Vec<T, 4>{2 * (xy - wz), 1 - 2 * (xx + zz), 2 * (yz + wx), 0},
                                 Vec<T, 4>{2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (xx + yy), 0}, Vec<T, 4>{0, 0, 0, 1}};
    }

    // ========================================================================
    // Operators
    // ========================================================================

    /// Quaternion multiplication (geometric product)
    /// @param b Right-hand quaternion
    /// @return Product quaternion representing combined rotation
    constexpr Quat operator*(const Quat& b) const
    {
        return {a * b.a - e23 * b.e23 - e31 * b.e31 - e12 * b.e12, a * b.e23 + e23 * b.a + e31 * b.e12 - e12 * b.e31, a * b.e31 - e23 * b.e12 + e31 * b.a + e12 * b.e23,
                a * b.e12 + e23 * b.e31 - e31 * b.e23 + e12 * b.a};
    }

    /// Compound multiplication assignment
    /// @param b Right-hand quaternion
    /// @return Reference to this quaternion after multiplication
    constexpr Quat& operator*=(const Quat& b)
    {
        *this = *this * b;
        return *this;
    }

    /// Rotates a vector by this quaternion
    /// @param v Vector to rotate
    /// @return Rotated vector
    constexpr Vec<T, 3> operator*(const Vec<T, 3>& v) const
    {
        Vec<T, 3> t = T{2} * Vec<T, 3>::Cross({e23, e31, e12}, v);
        return v + a * t + Vec<T, 3>::Cross({e23, e31, e12}, t);
    }
};

export using quatf = Quat<float>;
export using quatd = Quat<double>;

// SIMD Quaternion using Geometric Algebra notation
// Layout: [a, e23, e31, e12] = [scalar, i, j, k]
// Right-handed coordinate system
template <typename>
struct alignas(16) QuatSimd
{
    __m128 data; // a, e23, e31, e12

    // ========================================================================
    // Constructors
    // ========================================================================

    QuatSimd()
        : data(_mm_setr_ps(1.0F, 0.0F, 0.0F, 0.0F))
    {
    }

    QuatSimd(__m128 v)
        : data(v)
    {
    }

    QuatSimd(const float a, const float e23, const float e31, const float e12)
        : data(_mm_setr_ps(a, e23, e31, e12))
    {
    }

    // ========================================================================
    // Static Factory Methods
    // ========================================================================

    static QuatSimd Identity()
    {
        return QuatSimd(_mm_setr_ps(1.0F, 0.0F, 0.0F, 0.0F));
    }

    static QuatSimd FromAxisAngle(const __m128 axis, const float angle)
    {
        const float half = angle * 0.5F;
        const float s    = std::sin(half);
        const float c    = std::cos(half);

        // axis is [x, y, z, _]; scaled is [x*s, y*s, z*s, _*s].
        // We need result = [c, x*s, y*s, z*s], i.e. scaled shifted UP one lane
        // before lane 0 is overwritten with c. Using move_ss to inject the
        // scalar after the shift avoids the lane-alignment bug of blending
        // straight into the unshifted vector.
        const __m128 scaled = _mm_mul_ps(axis, _mm_set1_ps(s));
        const __m128 moved  = _mm_shuffle_ps(scaled, scaled, _MM_SHUFFLE(2, 1, 0, 0)); // [_, x*s, y*s, z*s]
        return QuatSimd(_mm_move_ss(moved, _mm_set_ss(c)));                            // [c, x*s, y*s, z*s]
    }

    static QuatSimd FromEuler(const float roll, const float pitch, const float yaw)
    {
        const float cr{std::cos(roll * 0.5F)};
        const float sr{std::sin(roll * 0.5F)};
        const float cp{std::cos(pitch * 0.5F)};
        const float sp{std::sin(pitch * 0.5F)};
        const float cy{std::cos(yaw * 0.5F)};
        const float sy{std::sin(yaw * 0.5F)};

        // Right-handed ZYX Euler convention
        float a{(cr * cp * cy) + (sr * sp * sy)};
        float e23{(sr * cp * cy) - (cr * sp * sy)};
        float e31{(cr * sp * cy) + (sr * cp * sy)};
        float e12{(cr * cp * sy) - (sr * sp * cy)};

        return QuatSimd(a, e23, e31, e12);
    }

    static QuatSimd FromMatrix(const float* matrix)
    {
        // Shepperd's method for numerical stability.
        // `matrix` is column-major 3x3 (or the top-left 3x3 of a 4x4), matching
        // ToMatrix3x3/ToMatrix4x4 below, i.e. M[row][col] == matrix[col*3+row].
        const float fourXSquaredMinus1{matrix[0] - matrix[4] - matrix[8]};
        const float fourYSquaredMinus1{matrix[4] - matrix[0] - matrix[8]};
        const float fourZSquaredMinus1{matrix[8] - matrix[0] - matrix[4]};
        const float fourWSquaredMinus1{matrix[0] + matrix[4] + matrix[8]};

        int biggestIndex               = 0;
        float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

        if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex             = 1;
        }
        if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex             = 2;
        }
        if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex             = 3;
        }

        // Use a real sqrt here: Rsqrt is a low-precision (~12 bit) approximation
        // and this value seeds every other component via `mult`, so its error
        // would propagate into the whole quaternion.
        const float biggestVal = Sqrt(fourBiggestSquaredMinus1 + 1.0F) * 0.5F;
        const float mult       = 0.25F / biggestVal;

        // Off-diagonal terms below are indexed for the column-major layout
        // M[row][col] == matrix[col*3+row], matching ToMatrix3x3/ToMatrix4x4.
        switch (biggestIndex)
        {
        case 0: // scalar is largest
            return QuatSimd(biggestVal, (matrix[5] - matrix[7]) * mult, (matrix[6] - matrix[2]) * mult, (matrix[1] - matrix[3]) * mult);
        case 1: // e23 (x) is largest
            return QuatSimd((matrix[5] - matrix[7]) * mult, biggestVal, (matrix[1] + matrix[3]) * mult, (matrix[6] + matrix[2]) * mult);
        case 2: // e31 (y) is largest
            return QuatSimd((matrix[6] - matrix[2]) * mult, (matrix[1] + matrix[3]) * mult, biggestVal, (matrix[5] + matrix[7]) * mult);
        case 3: // e12 (z) is largest
            return QuatSimd((matrix[1] - matrix[3]) * mult, (matrix[6] + matrix[2]) * mult, (matrix[5] + matrix[7]) * mult, biggestVal);
        default: return Identity();
        }
    }

    static QuatSimd RotateX(const float angle)
    {
        const float half = angle * 0.5F;
        return QuatSimd(std::cos(half), std::sin(half), 0.0F, 0.0F);
    }

    static QuatSimd RotateY(const float angle)
    {
        const float half = angle * 0.5F;
        return QuatSimd(std::cos(half), 0.0F, std::sin(half), 0.0F);
    }

    static QuatSimd RotateZ(const float angle)
    {
        const float half = angle * 0.5F;
        return QuatSimd(std::cos(half), 0.0F, 0.0F, std::sin(half));
    }

    static QuatSimd Exp(const __m128 bivector)
    {
        // bivector is [x, y, z, _]
        alignas(16) float v[4];
        _mm_store_ps(v, bivector);

        const float angleSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];

        if (angleSq < 1e-12f)
        {
            return Identity();
        }

        const float angle = Sqrt(angleSq);
        const float s     = std::sin(angle) / angle;
        return QuatSimd(_mm_setr_ps(std::cos(angle), v[0] * s, v[1] * s, v[2] * s));
    }

    // ========================================================================
    // Static Utility Methods
    // ========================================================================

    static float Dot(const QuatSimd& a, const QuatSimd& b)
    {
        const __m128 mul = _mm_mul_ps(a.data, b.data);
        // Horizontal add
        __m128 shuf = _mm_movehdup_ps(mul);
        __m128 sums = _mm_add_ps(mul, shuf);
        shuf        = _mm_movehl_ps(shuf, sums);
        sums        = _mm_add_ss(sums, shuf);
        return _mm_cvtss_f32(sums);
    }

    static float Length(const QuatSimd& q)
    {
        const float lenSq = Dot(q, q);
        return Sqrt(lenSq);
    }

#if BALBINO_SIMD_SSE41 || BALBINO_SIMD_SSE42
    static QuatSimd Normalize(QuatSimd q)
    {
        static const __m128 almostZero = _mm_set1_ps(1E-6F);
        __m128 dp                      = _mm_dp_ps(q.data, q.data, 0xFF);
        const __m128 cmp               = _mm_cmpgt_ps(dp, almostZero);
        dp                             = _mm_rsqrt_ps(dp);
        q.data                         = _mm_mul_ps(q.data, _mm_and_ps(dp, cmp));
        return q;
    }

    static QuatSimd Inverse(const QuatSimd& q)
    {
        const __m128 dot  = _mm_dp_ps(q.data, q.data, 0xFF);
        const __m128 sign = _mm_setr_ps(1.0F, -1.0F, -1.0F, -1.0F);
        return QuatSimd(_mm_div_ps(_mm_mul_ps(q.data, sign), dot));
    }
#else // SSE2/SSE3 — no _mm_dp_ps, build the broadcast dot product manually
    static QuatSimd Normalize(QuatSimd q)
    {
        static const __m128 almostZero = _mm_set1_ps(1E-6F);

        __m128 mul  = _mm_mul_ps(q.data, q.data);
        __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(mul, shuf);
        shuf        = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(1, 0, 3, 2));
        __m128 dp   = _mm_add_ps(sums, shuf); // dot, broadcast to all 4 lanes

        const __m128 cmp = _mm_cmpgt_ps(dp, almostZero);
        dp               = _mm_rsqrt_ps(dp);
        q.data           = _mm_mul_ps(q.data, _mm_and_ps(dp, cmp));
        return q;
    }

    static QuatSimd Inverse(const QuatSimd& q)
    {
        __m128 mul  = _mm_mul_ps(q.data, q.data);
        __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(mul, shuf);
        shuf        = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(1, 0, 3, 2));
        __m128 dot  = _mm_add_ps(sums, shuf);

        const __m128 sign = _mm_setr_ps(1.0F, -1.0F, -1.0F, -1.0F);
        return QuatSimd(_mm_div_ps(_mm_mul_ps(q.data, sign), dot));
    }
#endif

    static QuatSimd Reverse(const QuatSimd& q)
    {
        // Reverse negates the bivector part
        const __m128 sign = _mm_setr_ps(1.0F, -1.0F, -1.0F, -1.0F);
        return QuatSimd(_mm_mul_ps(q.data, sign));
    }

    static QuatSimd Conjugate(const QuatSimd& q)
    {
        return Reverse(q);
    }

    // Geometric product (right-handed)
    // q1 * q2 = (a1*a2 - b1·b2) + (a1*b2 + a2*b1 + b1×b2)
    static QuatSimd Multiply(const QuatSimd& q1, const QuatSimd& q2)
    {
        // Broadcast components of q1
        const __m128 a1    = _mm_shuffle_ps(q1.data, q1.data, _MM_SHUFFLE(0, 0, 0, 0));
        const __m128 e23_1 = _mm_shuffle_ps(q1.data, q1.data, _MM_SHUFFLE(1, 1, 1, 1));
        const __m128 e31_1 = _mm_shuffle_ps(q1.data, q1.data, _MM_SHUFFLE(2, 2, 2, 2));
        const __m128 e12_1 = _mm_shuffle_ps(q1.data, q1.data, _MM_SHUFFLE(3, 3, 3, 3));

        // Prepare shuffled versions of q2 for cross products
        // q2 = [a2, e23_2, e31_2, e12_2]
        const __m128 q2_wzyx = _mm_shuffle_ps(q2.data, q2.data, _MM_SHUFFLE(0, 1, 2, 3));
        const __m128 q2_zwxy = _mm_shuffle_ps(q2.data, q2.data, _MM_SHUFFLE(1, 0, 3, 2));
        const __m128 q2_yxwz = _mm_shuffle_ps(q2.data, q2.data, _MM_SHUFFLE(2, 3, 0, 1));

        // Compute each term
        __m128 t0 = _mm_mul_ps(a1, q2.data);    // a1 * q2
        __m128 t1 = _mm_mul_ps(e23_1, q2_wzyx); // e23_1 * [a2, e12_2, e31_2, e23_2]
        __m128 t2 = _mm_mul_ps(e31_1, q2_zwxy); // e31_1 * [e12_2, a2, e23_2, e31_2]
        __m128 t3 = _mm_mul_ps(e12_1, q2_yxwz); // e12_1 * [e31_2, e23_2, a2, e12_2]

        // Apply signs for geometric product
        // Result: [a, e23, e31, e12]
        const __m128 sign0 = _mm_setr_ps(1.0F, 1.0F, 1.0F, 1.0F);
        const __m128 sign1 = _mm_setr_ps(-1.0F, 1.0F, -1.0F, 1.0F);
        const __m128 sign2 = _mm_setr_ps(-1.0F, 1.0F, 1.0F, -1.0F);
        const __m128 sign3 = _mm_setr_ps(-1.0F, -1.0F, 1.0F, 1.0F);

        t0 = _mm_mul_ps(t0, sign0);
        t1 = _mm_mul_ps(t1, sign1);
        t2 = _mm_mul_ps(t2, sign2);
        t3 = _mm_mul_ps(t3, sign3);

        // Sum all terms
        __m128 result = _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3));
        return QuatSimd(result);
    }

    // Rotate vector using sandwich product: q * v * q^(-1)
    // Optimized formula: v' = v + 2*a*(bivector × v) + 2*(bivector × (bivector × v))
    static __m128 RotateVector(const QuatSimd& q, const __m128 v)
    {
        // Extract scalar and bivector parts.
        // q.data = [a, e23, e31, e12]; v = [x, y, z, _].
        // The bivector components live in lanes [1,2,3] of q.data, but v's
        // components live in lanes [0,1,2]. The cross-product shuffles below
        // assume both operands share the same lane layout, so the bivector
        // must be shifted down by one lane to [e23, e31, e12, _] before use
        // (lane 3 is don't-care since v's lane 3 is also unused).
        const __m128 a_broadcast = _mm_shuffle_ps(q.data, q.data, _MM_SHUFFLE(0, 0, 0, 0));
        const __m128 bivector    = _mm_shuffle_ps(q.data, q.data, _MM_SHUFFLE(3, 3, 2, 1)); // [e23, e31, e12, e12]

        // Compute bivector × v (cross product)
        const __m128 v_yzx = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1));
        const __m128 v_zxy = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2));
        const __m128 b_yzx = _mm_shuffle_ps(bivector, bivector, _MM_SHUFFLE(3, 0, 2, 1));
        const __m128 b_zxy = _mm_shuffle_ps(bivector, bivector, _MM_SHUFFLE(3, 1, 0, 2));

        __m128 t = _mm_sub_ps(_mm_mul_ps(b_yzx, v_zxy), _mm_mul_ps(b_zxy, v_yzx));

        // Compute bivector × t
        const __m128 t_yzx = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 2, 1));
        const __m128 t_zxy = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 1, 0, 2));
        const __m128 u     = _mm_sub_ps(_mm_mul_ps(b_yzx, t_zxy), _mm_mul_ps(b_zxy, t_yzx));

        // v' = v + 2*a*t + 2*u
        const __m128 two   = _mm_set1_ps(2.0F);
        const __m128 two_a = _mm_mul_ps(two, a_broadcast);
        const __m128 term1 = _mm_mul_ps(two_a, t);
        const __m128 term2 = _mm_mul_ps(two, u);

        return _mm_add_ps(_mm_add_ps(v, term1), term2);
    }

    static QuatSimd Slerp(const QuatSimd& a, const QuatSimd& b, const float t)
    {
        float dot = Dot(a, b);

        // Ensure we take the shortest path
        QuatSimd b_adj = b;
        if (dot < 0.0F)
        {
            b_adj.data = _mm_mul_ps(b.data, _mm_set1_ps(-1.0F));
            dot        = -dot;
        }

        if (constexpr float threshold{1.0F - 1E-6F}; dot > threshold)
        {
            // Linear interpolation for close quaternions
            const __m128 diff   = _mm_sub_ps(b_adj.data, a.data);
            const __m128 scaled = _mm_mul_ps(diff, _mm_set1_ps(t));
            __m128 result       = _mm_add_ps(a.data, scaled);
            return Normalize(QuatSimd(result));
        }

        // Spherical interpolation
        const float theta    = std::acos(dot);
        const float sinTheta = std::sin(theta);
        const float wa       = std::sin((1.0F - t) * theta) / sinTheta;
        const float wb       = std::sin(t * theta) / sinTheta;

        const __m128 a_scaled = _mm_mul_ps(a.data, _mm_set1_ps(wa));
        const __m128 b_scaled = _mm_mul_ps(b_adj.data, _mm_set1_ps(wb));

        return QuatSimd(_mm_add_ps(a_scaled, b_scaled));
    }

    static QuatSimd Nlerp(const QuatSimd& a, const QuatSimd& b, const float t)
    {
        __m128 b_data = b.data;
        if (Dot(a, b) < 0.0F)
        {
            b_data = _mm_mul_ps(b_data, _mm_set1_ps(-1.0F));
        }

        const __m128 diff   = _mm_sub_ps(b_data, a.data);
        const __m128 scaled = _mm_mul_ps(diff, _mm_set1_ps(t));
        __m128 result       = _mm_add_ps(a.data, scaled);

        return Normalize(QuatSimd(result));
    }

    static float AngleBetween(const QuatSimd& a, const QuatSimd& b)
    {
        const float dot = Dot(a, b);
        return 2.0F * std::acos(std::min(std::abs(dot), 1.0F));
    }

    QuatSimd Reverse() const
    {
        return Reverse(*this);
    }

    QuatSimd Conjugate() const
    {
        return Reverse(*this);
    }

    QuatSimd Inverse() const
    {
        return Inverse(*this);
    }

    float LengthSqr() const
    {
        return Dot(*this, *this);
    }

    float Length() const
    {
        return Length(*this);
    }

    QuatSimd Normalize() const
    {
        return Normalize(*this);
    }

    __m128 Log() const
    {
        // Returns bivector [x, y, z, _]
        alignas(16) float q[4];
        _mm_store_ps(q, data);

        const float bivectorLenSq = (q[1] * q[1]) + (q[2] * q[2]) + (q[3] * q[3]);

        if (bivectorLenSq < 1e-6f)
        {
            return _mm_setzero_ps();
        }

        const float bivectorLen = Sqrt(bivectorLenSq);
        const float angle       = std::atan2(bivectorLen, q[0]);
        const float s           = angle / bivectorLen;

        return _mm_setr_ps(q[1] * s, q[2] * s, q[3] * s, 0.0F);
    }

    QuatSimd Pow(const float exponent) const
    {
        return Exp(_mm_mul_ps(Log(), _mm_set1_ps(exponent)));
    }

    // ========================================================================
    // Conversion Methods
    // ========================================================================

    void ToAxisAngle(__m128& axis, float& angle) const
    {
        alignas(16) float q[4];
        _mm_store_ps(q, data);

        const float bivectorLenSq = (q[1] * q[1]) + (q[2] * q[2]) + (q[3] * q[3]);

        if (bivectorLenSq < 1e-6F)
        {
            axis  = _mm_setr_ps(1.0F, 0.0F, 0.0F, 0.0F);
            angle = 0.0F;
            return;
        }

        const float bivectorLen = Sqrt(bivectorLenSq);
        const float invLen      = 1.0F / bivectorLen;
        axis                    = _mm_setr_ps(q[1] * invLen, q[2] * invLen, q[3] * invLen, 0.0F);
        angle                   = 2.0F * std::atan2(bivectorLen, q[0]);
    }

    __m128 ToEuler() const
    {
        alignas(16) float q[4];
        _mm_store_ps(q, data);

        float a   = q[0];
        float e23 = q[1];
        float e31 = q[2];
        float e12 = q[3];

        // Roll (x-axis rotation)
        const float sinr_cosp = 2.0F * (a * e23 + e31 * e12);
        const float cosr_cosp = 1.0F - (2.0F * (e23 * e23 + e31 * e31));
        const float roll      = std::atan2(sinr_cosp, cosr_cosp);

        // Pitch (y-axis rotation)
        const float sinp  = 2.0F * (a * e31 - e12 * e23);
        const float pitch = std::abs(sinp) >= 1.0F ? std::copysign(std::numbers::pi_v<float> / 2.0F, sinp) : std::asin(sinp);

        // Yaw (z-axis rotation)
        const float siny_cosp = 2.0F * (a * e12 + e23 * e31);
        const float cosy_cosp = 1.0F - (2.0F * (e31 * e31 + e12 * e12));
        const float yaw       = std::atan2(siny_cosp, cosy_cosp);

        return _mm_setr_ps(roll, pitch, yaw, 0.0F);
    }

    void ToMatrix3x3(float* matrix) const
    {
        alignas(16) float q[4];
        _mm_store_ps(q, data);

        float a  = q[0];
        float x  = q[1];
        float y  = q[2];
        float z  = q[3];
        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float ax = a * x;
        float ay = a * y;
        float az = a * z;

        // Column-major order
        matrix[0] = 1.0F - (2.0F * (yy + zz));
        matrix[1] = 2.0F * (xy + az);
        matrix[2] = 2.0F * (xz - ay);

        matrix[3] = 2.0F * (xy - az);
        matrix[4] = 1.0F - (2.0F * (xx + zz));
        matrix[5] = 2.0F * (yz + ax);

        matrix[6] = 2.0F * (xz + ay);
        matrix[7] = 2.0F * (yz - ax);
        matrix[8] = 1.0F - (2.0F * (xx + yy));
    }

    void ToMatrix4x4(float* matrix) const
    {
        alignas(16) float q[4];
        _mm_store_ps(q, data);

        float a  = q[0];
        float x  = q[1];
        float y  = q[2];
        float z  = q[3];
        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float ax = a * x;
        float ay = a * y;
        float az = a * z;

        // Column-major 4x4
        matrix[0] = 1.0F - (2.0F * (yy + zz));
        matrix[1] = 2.0F * (xy + az);
        matrix[2] = 2.0F * (xz - ay);
        matrix[3] = 0.0F;

        matrix[4] = 2.0F * (xy - az);
        matrix[5] = 1.0F - (2.0F * (xx + zz));
        matrix[6] = 2.0F * (yz + ax);
        matrix[7] = 0.0F;

        matrix[8]  = 2.0F * (xz + ay);
        matrix[9]  = 2.0F * (yz - ax);
        matrix[10] = 1.0F - (2.0F * (xx + yy));
        matrix[11] = 0.0F;

        matrix[12] = 0.0F;
        matrix[13] = 0.0F;
        matrix[14] = 0.0F;
        matrix[15] = 1.0F;
    }

    // ========================================================================
    // Operators
    // ========================================================================

    QuatSimd operator*(const QuatSimd& other) const
    {
        return Multiply(*this, other);
    }

    QuatSimd& operator*=(const QuatSimd& other)
    {
        *this = Multiply(*this, other);
        return *this;
    }

    __m128 operator*(const __m128 v) const
    {
        return RotateVector(*this, v);
    }
};
export using quadf_simd = QuatSimd<float>;
} // namespace fawn_algebra

export template <typename T, std::uint8_t N>
struct std::hash<fawn_algebra::Vec<T, N>>
{
    constexpr std::size_t operator()(const fawn_algebra::Vec<T, N>& vec) const noexcept
    {
        std::size_t hash = 0;
        std::hash<T> hasher;
        for (size_t i = 0; i < N; i++)
        {
            hash ^= hasher(vec[i]) << i;
        }
        return hash;
    }
};

export template <typename T, std::uint8_t N>
struct std::hash<fawn_algebra::VecSimd<T, N>>
{
    constexpr std::size_t operator()(const fawn_algebra::VecSimd<T, N>& vec) const noexcept
    {
        std::size_t hash = 0;
        std::hash<T> hasher;
        for (size_t i = 0; i < N; i++)
        {
            hash ^= hasher(vec[i]) << i;
        }
        return hash;
    }
};

export template <typename T, std::uint8_t N>
struct std::hash<fawn_algebra::Mat<T, N>>
{
    constexpr std::size_t operator()(const fawn_algebra::Mat<T, N>& mat) const noexcept
    {
        std::size_t hash = 0;
        std::hash<T> hasher;
        for (size_t i = 0; i < N; i++)
        {
            hash ^= hasher(mat[i]) << i;
        }
        return hash;
    }
};

export template <typename T>
struct std::hash<fawn_algebra::Quat<T>>
{
    constexpr std::size_t operator()(const fawn_algebra::Quat<T>& quat) const noexcept
    {
        std::size_t hash = 0;
        std::hash<T> hasher;
        hash ^= hasher(quat.a) << 0;
        hash ^= hasher(quat.e23) << 1;
        hash ^= hasher(quat.e31) << 2;
        hash ^= hasher(quat.e12) << 3;
        return hash;
    }
};
