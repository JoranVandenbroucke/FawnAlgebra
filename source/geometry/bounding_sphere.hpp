#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <type_traits>

import FawnAlgebra;
using namespace FawnAlgebra;

namespace DeerGeometry
{
template <std::uint8_t Pow>
struct pi_pow
{
    static constexpr double value = std::numbers::pi * pi_pow<Pow - 1>::value;
};
template <>
struct pi_pow<1>
{
    static constexpr double value = std::numbers::pi;
};
template <>
struct pi_pow<0>
{
    static constexpr double value = 1;
};

template <std::uint8_t Fac>
struct factorial
{
    static constexpr uint64_t value = Fac * factorial<Fac - 1>::value;
};
template <>
struct factorial<2>
{
    static constexpr double value = 2;
};
template <>
struct factorial<1>
{
    static constexpr double value = 1;
};

template <typename Type>
constexpr Type static_pow(Type value, uint8_t pow)
{
    return pow == 1 ? value : value * static_pow(value, pow - 1);
}

template <typename Type, std::uint8_t Dimension>
    requires(std::is_arithmetic_v<Type> && Dimension != 0)
struct bounding_sphere
{
    Vec<Type, Dimension> origine = Vec<Type, Dimension>(std::numeric_limits<Type>::max());
    Type                 radius{std::numeric_limits<Type>::max()};

    constexpr void Grow(const Vec<Type, Dimension>& point) noexcept
    {
        if (radius == std::numeric_limits<Type>::max())
        {
            origine = point;
            radius  = Type{0};
            return;
        }

        const Vec<Type, Dimension> direction{point - origine};
        const Type                 distSq{direction | direction};

        if (distSq > radius * radius)
        {
            const Type dist{std::sqrt(distSq)};
            const Type newRadius{(2 * radius + dist) / Type{2}};
            origine = origine + (direction * ((newRadius - radius) / dist));
            radius  = newRadius;
        }
    }

    constexpr void Grow(const bounding_sphere& other) noexcept
    {
        const Vec<Type, Dimension> direction{other.origine - origine};
        const Type                 distSq{direction | direction};
        const Type                 dist{std::sqrt(distSq)};
        if (distSq + other.radius < radius)
        {
            return;
        }
        if (distSq + radius < other.radius)
        {
            *this = other;
            return;
        }

        const Type newRadius{(dist + radius + other.radius) / Type{2}};
        origine = origine + (direction * ((newRadius - radius) / dist));
        radius  = newRadius;
    }

    [[nodiscard]] constexpr Type Area() const noexcept
    {
        if constexpr (Dimension == 1)
        {
            return Type{2} * radius;
        }
        if constexpr (Dimension == 2)
        {
            return static_cast<Type>(std::numbers::pi * static_cast<double>(static_pow(radius, 2)));
        }
        if constexpr (Dimension == 3)
        {
            return static_cast<Type>((4.0 * std::numbers::pi / 3.0) * static_cast<double>(static_pow(radius, 3)));
        }
        if constexpr (Dimension == 4)
        {
            return static_cast<Type>((pi_pow<2>::value / 2.0) * static_cast<double>(static_pow(radius, 4)));
        }
        if constexpr (Dimension == 5)
        {
            return static_cast<Type>((8.0 * pi_pow<2>::value / 15.0) * static_cast<double>(static_pow(radius, 5)));
        }
        if constexpr (Dimension == 6)
        {
            return static_cast<Type>((pi_pow<3>::value / 6.0) * static_cast<double>(static_pow(radius, 6)));
        }
        if constexpr (Dimension == 7)
        {
            return static_cast<Type>((16.0 * pi_pow<3>::value / 105.0) * static_cast<double>(static_pow(radius, 7)));
        }
        if constexpr (Dimension == 8)
        {
            return static_cast<Type>((pi_pow<4>::value / 24.0) * static_cast<double>(static_pow(radius, 8)));
        }
        if constexpr (Dimension == 9)
        {
            return static_cast<Type>((32.0 * pi_pow<4>::value / 945.0) * static_cast<double>(static_pow(radius, 9)));
        }
        if constexpr (Dimension == 10)
        {
            return static_cast<Type>((pi_pow<5>::value / 120.0) * static_cast<double>(static_pow(radius, 10)));
        }
        if constexpr (Dimension == 11)
        {
            return static_cast<Type>((64.0 * pi_pow<5>::value / 10395.0) * static_cast<double>(static_pow(radius, 11)));
        }
        if constexpr (Dimension == 12)
        {
            return static_cast<Type>((pi_pow<6>::value / 270.0) * static_cast<double>(static_pow(radius, 12)));
        }
        if constexpr (Dimension == 13)
        {
            return static_cast<Type>((128.0 * pi_pow<6>::value / 135135.0) * static_cast<double>(static_pow(radius, 13)));
        }
        if constexpr (Dimension == 14)
        {
            return static_cast<Type>((pi_pow<7>::value / 5040.0) * static_cast<double>(static_pow(radius, 14)));
        }
        if constexpr (Dimension == 15)
        {
            return static_cast<Type>((256 * pi_pow<7>::value / 2027025.0) * static_cast<double>(static_pow(radius, 15)));
        }
        return {static_cast<Type>((pi_pow<Dimension / 2>::value / static_cast<double>(factorial<Dimension / 2 + 1>::value))
                                  * static_cast<double>(static_pow(radius, 15)))};
    }

    [[nodiscard]] constexpr bool Intersect(const bounding_sphere& other) const noexcept
    {
        const Vec<Type, Dimension> direction{other.origine - origine};
        const Type                 maxRadius{radius + other.radius};
        const Type                 distSq{direction | direction};

        return distSq < maxRadius * maxRadius;
    }

    [[nodiscard]] constexpr bool Contains(const Vec<Type, Dimension>& point) const noexcept
    {
        const Vec<Type, Dimension> direction{point - origine};
        const Type                 distSq{direction | direction};

        return distSq < radius * radius;
    }

    static constexpr bool Intersect(const bounding_sphere& a, const bounding_sphere& b) noexcept
    {
        return a.Intersect(b);
    }
};

template <typename Type>
using bounding_sphere2d = bounding_sphere<Type, 2>;
template <typename Type>
using bounding_sphere3d = bounding_sphere<Type, 3>;
template <typename Type>
using bounding_sphere4d = bounding_sphere<Type, 4>;

using bounding_sphere2d_int8     = bounding_sphere2d<int8_t>;
using bounding_sphere2d_int16    = bounding_sphere2d<int16_t>;
using bounding_sphere2d_int32    = bounding_sphere2d<int32_t>;
using bounding_sphere2d_int64    = bounding_sphere2d<int64_t>;
using bounding_sphere2d_uint8    = bounding_sphere2d<uint8_t>;
using bounding_sphere2d_uint16   = bounding_sphere2d<uint16_t>;
using bounding_sphere2d_uint32   = bounding_sphere2d<uint32_t>;
using bounding_sphere2d_uint64   = bounding_sphere2d<uint64_t>;
using bounding_sphere2d_float32  = bounding_sphere2d<float>;
using bounding_sphere2d_float64  = bounding_sphere2d<double>;
using bounding_sphere2d_float128 = bounding_sphere2d<long double>;

using bounding_sphere3d_int8     = bounding_sphere3d<int8_t>;
using bounding_sphere3d_int16    = bounding_sphere3d<int16_t>;
using bounding_sphere3d_int32    = bounding_sphere3d<int32_t>;
using bounding_sphere3d_int64    = bounding_sphere3d<int64_t>;
using bounding_sphere3d_uint8    = bounding_sphere3d<uint8_t>;
using bounding_sphere3d_uint16   = bounding_sphere3d<uint16_t>;
using bounding_sphere3d_uint32   = bounding_sphere3d<uint32_t>;
using bounding_sphere3d_uint64   = bounding_sphere3d<uint64_t>;
using bounding_sphere3d_float32  = bounding_sphere3d<float>;
using bounding_sphere3d_float64  = bounding_sphere3d<double>;
using bounding_sphere3d_float128 = bounding_sphere3d<long double>;

using bounding_sphere4d_int8     = bounding_sphere4d<int8_t>;
using bounding_sphere4d_int16    = bounding_sphere4d<int16_t>;
using bounding_sphere4d_int32    = bounding_sphere4d<int32_t>;
using bounding_sphere4d_int64    = bounding_sphere4d<int64_t>;
using bounding_sphere4d_uint8    = bounding_sphere4d<uint8_t>;
using bounding_sphere4d_uint16   = bounding_sphere4d<uint16_t>;
using bounding_sphere4d_uint32   = bounding_sphere4d<uint32_t>;
using bounding_sphere4d_uint64   = bounding_sphere4d<uint64_t>;
using bounding_sphere4d_float32  = bounding_sphere4d<float>;
using bounding_sphere4d_float64  = bounding_sphere4d<double>;
using bounding_sphere4d_float128 = bounding_sphere4d<long double>;

} // namespace DeerGeometry
