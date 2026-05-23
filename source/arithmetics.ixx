//
// Copyright (c) 2025.
// Author: Joran.
//

module;

export module FawnAlgebra:Arithmetics;
import std;

namespace fawn_algebra
{
export template <typename T, std::size_t N>
class Vec
{
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;

    std::array<T, N> data{};

    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        return data[idx];
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        return data[idx];
    }
    constexpr auto operator-() const
    {
        Vec temp{*this};
        for (auto& val : temp.data)
        {
            val = -val;
        }
        return temp;
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] += other.data[i];
        }
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        *this += -other;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] *= other.data[i];
        }
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] /= other.data[i];
        }
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<T>
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] <<= other.data[i];
        }
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<T>
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] <<= other.data[i];
        }
        return *this;
    }
};

template <typename T>
class Vec<T, 2>
{
    static constexpr auto Size{2ULL};

  public:
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;

    T x{};
    T y{};

    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        return idx == 0 ? x : y;
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        return idx == 0 ? x : y;
    }
    constexpr T operator~() noexcept
        requires std::is_integral_v<value_type>
    {
        return {~x, ~y};
    }
    constexpr auto operator-() const -> Vec
    {
        return {-x, -y};
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        x <<= other.x;
        y <<= other.y;
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        x >>= other.x;
        y >>= other.y;
        return *this;
    }
    // Inner Product
    constexpr T operator|(const Vec& other) noexcept
    {
        return x * other.x + y * other.y;
    }

    // Outer Product
    constexpr T operator^(const Vec& other) noexcept
    {
        return x * other.y - y * other.x;
    }

    constexpr Vec operator&=(const Vec& other) noexcept
    {
        *this = (*this | other) * other;
        return *this;
    }
    // Reject Product
    constexpr Vec& operator%=(const Vec& other) noexcept
    {
        *this = *this - (((*this | other) / (other | other)) * other);
        return *this;
    }
};

template <typename T>
class Vec<T, 3>
{
    static constexpr auto Size{3ULL};

  public:
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;
    T x{};
    T y{};
    T z{};

    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        return idx == 0 ? x : idx == 1 ? y : z;
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        return idx == 0 ? x : idx == 1 ? y : z;
    }
    constexpr T operator~() noexcept
        requires std::is_integral_v<value_type>
    {
        return {~x, ~y, ~z};
    }
    constexpr auto operator-() const -> Vec
    {
        return Vec{-x, -y, -z};
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        x <<= other.x;
        y <<= other.y;
        z <<= other.z;
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        x >>= other.x;
        y >>= other.y;
        z >>= other.z;
        return *this;
    }
    // Outer Product
    constexpr Vec operator^=(const Vec& other) noexcept
    {
        *this = {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
        return *this;
    }

    // Projection Product
    constexpr Vec operator&=(const Vec& other) noexcept
    {
        *this = (*this | other) * other;
        return *this;
    }
    // Reject Product
    constexpr Vec& operator%=(const Vec& other) noexcept
    {
        *this = *this - (((*this | other) / (other | other)) * other);
        return *this;
    }
};

template <typename T>
class Vec<T, 4>
{
    static constexpr auto Size{4ULL};

  public:
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;

    T x{};
    T y{};
    T z{};
    T w{};

    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        return idx == 0 ? x : idx == 1 ? y : idx == 2 ? z : w;
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        return idx == 0 ? x : idx == 1 ? y : idx == 2 ? z : w;
    }
    constexpr T operator~() noexcept
        requires std::is_integral_v<value_type>
    {
        return {~x, ~y, ~z};
    }
    constexpr auto operator-() const -> Vec
    {
        return Vec{-x, -y, -z};
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        x <<= other.x;
        y <<= other.y;
        z <<= other.z;
        w <<= other.w;
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        x >>= other.x;
        y >>= other.y;
        z >>= other.z;
        w >>= other.w;
        return *this;
    }
    // Outer Product
    constexpr Vec operator^=(const Vec& other) noexcept
    {
        return {y * other.z - z * other.y, z * other.x - other.z * x, x * other.y - y * other.x};
    }
    constexpr auto operator&=(const Vec& other) noexcept -> Vec&
    {
        *this = (*this | other) * other;
        return *this;
    }
    constexpr auto operator%=(const Vec& other) noexcept -> Vec&
    {
        *this = *this - (((*this | other) / (other | other)) * other);
        return *this;
    }
};

export template <typename T, std::size_t N>
constexpr auto operator+(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs += rhs;
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator-(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs -= rhs;
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator*(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs *= rhs;
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator/(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs /= rhs;
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator+(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs += Vec<T, N>(rhs);
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator-(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs -= Vec<T, N>(rhs);
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator*(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs *= Vec<T, N>(rhs);
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator/(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs /= Vec<T, N>(rhs);
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator+(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp += rhs;
    return temp;
}

export template <typename T, std::size_t N>
constexpr auto operator-(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp -= rhs;
    return temp;
}

export template <typename T, std::size_t N>
constexpr auto operator*(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp *= rhs;
    return temp;
}
export template <typename T, std::size_t N>
constexpr auto operator/(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp /= rhs;
    return temp;
}

export template <typename T, std::size_t N>
constexpr auto operator<<(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
    requires std::is_integral_v<T>
{
    lhs <<= rhs;
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator>>(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
    requires std::is_integral_v<T>
{
    lhs >>= rhs;
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator==(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept -> bool
{
    return lhs.data == rhs.data;
}
export template <typename T>
constexpr auto operator==(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs) noexcept -> bool
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
export template <typename T>
constexpr auto operator==(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs) noexcept -> bool
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
export template <typename T>
constexpr auto operator==(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs) noexcept -> bool
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

export template <typename T, std::size_t N>
constexpr auto operator<=>(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept -> bool
{
    return lhs.data <=> rhs.data;
}
export template <typename T>
constexpr auto operator<=>(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs) noexcept
{
    if (const auto value{lhs.x <=> rhs.x}; value != 0)
    {
        return value;
    }
    return lhs.y <=> rhs.y;
}
export template <typename T>
constexpr auto operator<=>(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs) noexcept
{
    if (const auto value{lhs.x <=> rhs.x}; value != 0)
    {
        return value;
    }
    if (const auto value{lhs.y <=> rhs.y}; value != 0)
    {
        return value;
    }
    return lhs.z <=> rhs.z;
}
export template <typename T>
constexpr auto operator<=>(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs) noexcept -> bool
{
    if (const auto value{lhs.x <=> rhs.x}; value != 0)
    {
        return value;
    }
    if (const auto value{lhs.y <=> rhs.y}; value != 0)
    {
        return value;
    }
    if (const auto value{lhs.z <=> rhs.z}; value != 0)
    {
        return value;
    }
    return lhs.w <=> rhs.w;
}
// Inner Product
export template <typename T, std::size_t N>
constexpr auto operator|(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> T
{
    T dot{};
    for (int i{}; i < N; ++i)
    {
        dot += lhs.data[i] * rhs.data[i];
    }
    return dot;
}

export template <typename T>
constexpr auto operator|(Vec<T, 2> lhs, const Vec<T, 2>& rhs) noexcept -> T
{

    return lhs.x * rhs.x + lhs.y * rhs.y;
}

export template <typename T>
constexpr auto operator|(Vec<T, 3> lhs, const Vec<T, 3>& rhs) noexcept -> T
{

    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

export template <typename T>
constexpr auto operator|(Vec<T, 4> lhs, const Vec<T, 4>& rhs) noexcept -> T
{

    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

// Outer Product
export template <typename T, std::size_t N>
constexpr auto operator^(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs ^= rhs;
    return lhs;
}

export template <typename T>
constexpr auto operator^(Vec<T, 2> lhs, const Vec<T, 2>& rhs) noexcept -> T
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
// todo : find a valid aternative for Vec<T, 4>
export template <typename T>
constexpr auto operator^([[maybe_unused]] Vec<T, 4> lhs, [[maybe_unused]] const Vec<T, 4>& rhs) noexcept -> T
{
    return 0;
}

// Projection Product
export template <typename T, std::size_t N>
constexpr auto operator&(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs &= rhs;
    return lhs;
}

// Reject Product
export template <typename T, std::size_t N>
constexpr auto operator%(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs %= rhs;
    return lhs;
}

#define OVERLOAD_FUNC_VEC(func)                                                                                                                                                    \
    export template <typename T, std::size_t N>                                                                                                                                    \
    constexpr Vec<T, N> func(const Vec<T, N>& v)                                                                                                                                   \
    {                                                                                                                                                                              \
        Vec<T, N> result;                                                                                                                                                          \
        for (std::size_t i = 0; i < N; ++i)                                                                                                                                        \
        {                                                                                                                                                                          \
            result[i] = std::func(v[i]);                                                                                                                                           \
        }                                                                                                                                                                          \
        return result;                                                                                                                                                             \
    }

#define OVERLOAD_FUNC_VEC_VEC(func)                                                                                                                                                \
    export template <typename T, std::size_t N>                                                                                                                                    \
    constexpr Vec<T, N> func(const Vec<T, N>& v, const Vec<T, N>& w)                                                                                                               \
    {                                                                                                                                                                              \
        Vec<T, N> result;                                                                                                                                                          \
        for (std::size_t i{}; i < N; ++i)                                                                                                                                          \
        {                                                                                                                                                                          \
            result[i] = std::func(v[i], w[i]);                                                                                                                                     \
        }                                                                                                                                                                          \
        return result;                                                                                                                                                             \
    }

OVERLOAD_FUNC_VEC(abs)
OVERLOAD_FUNC_VEC(exp)
OVERLOAD_FUNC_VEC(exp2)
OVERLOAD_FUNC_VEC(expm1)
OVERLOAD_FUNC_VEC(log)
OVERLOAD_FUNC_VEC(log10)
OVERLOAD_FUNC_VEC(log2)
OVERLOAD_FUNC_VEC(log1p)
OVERLOAD_FUNC_VEC(sqrt)
OVERLOAD_FUNC_VEC(cbrt)
OVERLOAD_FUNC_VEC(sin)
OVERLOAD_FUNC_VEC(cos)
OVERLOAD_FUNC_VEC(tan)
OVERLOAD_FUNC_VEC(asin)
OVERLOAD_FUNC_VEC(acos)
OVERLOAD_FUNC_VEC(atan)
OVERLOAD_FUNC_VEC(sinh)
OVERLOAD_FUNC_VEC(cosh)
OVERLOAD_FUNC_VEC(tanh)
OVERLOAD_FUNC_VEC(asinh)
OVERLOAD_FUNC_VEC(acosh)
OVERLOAD_FUNC_VEC(atanh)
OVERLOAD_FUNC_VEC(erf)
OVERLOAD_FUNC_VEC(erfc)
OVERLOAD_FUNC_VEC(tgamma)
OVERLOAD_FUNC_VEC(lgamma)
OVERLOAD_FUNC_VEC(ceil)
OVERLOAD_FUNC_VEC(floor)
OVERLOAD_FUNC_VEC(trunc)
OVERLOAD_FUNC_VEC(round)
OVERLOAD_FUNC_VEC(lround)
OVERLOAD_FUNC_VEC(llround)
OVERLOAD_FUNC_VEC(nearbyint)
OVERLOAD_FUNC_VEC(rint)
OVERLOAD_FUNC_VEC(lrint)
OVERLOAD_FUNC_VEC(llrint)
OVERLOAD_FUNC_VEC(frexp)
OVERLOAD_FUNC_VEC(ldexp)
OVERLOAD_FUNC_VEC(modf)
OVERLOAD_FUNC_VEC(scalbn)
OVERLOAD_FUNC_VEC(scalbln)
OVERLOAD_FUNC_VEC(ilogb)
OVERLOAD_FUNC_VEC(logb)
OVERLOAD_FUNC_VEC(nextafter)
OVERLOAD_FUNC_VEC(nexttoward)
OVERLOAD_FUNC_VEC(copysign)

OVERLOAD_FUNC_VEC_VEC(min)
OVERLOAD_FUNC_VEC_VEC(max)
OVERLOAD_FUNC_VEC_VEC(fdim)
OVERLOAD_FUNC_VEC_VEC(fmod)
OVERLOAD_FUNC_VEC_VEC(remainder)
OVERLOAD_FUNC_VEC_VEC(hypot)

export template <typename T, std::size_t N>
constexpr Vec<T, N> pow(const Vec<T, N>& v, T exponent)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::pow(v.data[i], exponent);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> fma(const Vec<T, N>& v, const Vec<T, N>& y, const Vec<T, N>& z)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::fma(v.data[i], y.data[i], z.data[i]);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> nan(const Vec<T, N>& v)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::nan(v.data[i]);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> remquo(const Vec<T, N>& a, const Vec<T, N>& b, Vec<int, N>& quo)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::remquo(a.data[i], b.data[i], &quo.data[i]);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> lerp(const Vec<T, N>& a, const Vec<T, N>& b, T t)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::lerp(a.data[i], b.data[i], t);
    }
    return result;
}

// Aliases for different types
export using int2 = Vec<std::int32_t, 2>;
export using int3 = Vec<std::int32_t, 3>;
export using int4 = Vec<std::int32_t, 4>;

export using uint2 = Vec<std::uint32_t, 2>;
export using uint3 = Vec<std::uint32_t, 3>;
export using uint4 = Vec<std::uint32_t, 4>;

export using float2 = Vec<float, 2>;
export using float3 = Vec<float, 3>;
export using float4 = Vec<float, 4>;

export using double2 = Vec<double, 2>;
export using double3 = Vec<double, 3>;
export using double4 = Vec<double, 4>;

export using char2 = Vec<std::int8_t, 2>;
export using char3 = Vec<std::int8_t, 3>;
export using char4 = Vec<std::int8_t, 4>;

export using uchar2 = Vec<std::uint8_t, 2>;
export using uchar3 = Vec<std::uint8_t, 3>;
export using uchar4 = Vec<std::uint8_t, 4>;

export using short2 = Vec<std::int16_t, 2>;
export using short3 = Vec<std::int16_t, 3>;
export using short4 = Vec<std::int16_t, 4>;

export using ushort2 = Vec<std::uint16_t, 2>;
export using ushort3 = Vec<std::uint16_t, 3>;
export using ushort4 = Vec<std::uint16_t, 4>;

export using int2 = Vec<std::int32_t, 2>;
export using int3 = Vec<std::int32_t, 3>;
export using int4 = Vec<std::int32_t, 4>;

export using uint2 = Vec<std::uint32_t, 2>;
export using uint3 = Vec<std::uint32_t, 3>;
export using uint4 = Vec<std::uint32_t, 4>;

export using float2 = Vec<float, 2>;
export using float3 = Vec<float, 3>;
export using float4 = Vec<float, 4>;

export using double2 = Vec<double, 2>;
export using double3 = Vec<double, 3>;
export using double4 = Vec<double, 4>;
} // namespace fawn_algebra
