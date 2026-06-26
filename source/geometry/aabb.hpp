//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

#pragma once

import FawnAlgebra;
import std;
using namespace fawn_algebra;

namespace deer_geometry
{
namespace detail
{
template <typename Type, std::uint8_t Dimension>
consteval Vec<Type, Dimension> CreateVector(const Type value)
{
    Vec<Type, Dimension> result;
    for (std::uint8_t i{}; i < Dimension; ++i)
    {
        result[i] = value;
    }

    return result;
}
} // namespace detail

template <typename Type, std::uint8_t Dimension>
    requires(std::is_arithmetic_v<Type> && Dimension != 0)
struct aabb
{
    Vec<Type, Dimension> minimum = detail::CreateVector<Type, Dimension>(std::numeric_limits<Type>::max());
    Vec<Type, Dimension> maximum = detail::CreateVector<Type, Dimension>(std::numeric_limits<Type>::lowest());

    constexpr void Grow(const Vec<Type, Dimension>& point) noexcept
    {
        minimum = Vec<Type, Dimension>::Min(minimum, point);
        maximum = Vec<Type, Dimension>::Max(maximum, point);
    }

    constexpr void Grow(const aabb& b) noexcept
    {
        if (b.minimum[0] != std::numeric_limits<Type>::lowest()) // Adjust for Dimension dimensions
        {
            Grow(b.minimum);
            Grow(b.maximum);
        }
    }

    [[nodiscard]] constexpr Type Area() const noexcept
    {
        Type result{1};
        for (std::uint8_t i{}; i < Dimension; ++i)
        {
            result *= (maximum[i] - minimum[i]);
        }
        return result;
    }

    [[nodiscard]] constexpr bool Intersect(const aabb& other) const noexcept
    {
        for (std::uint8_t i{}; i < Dimension; ++i)
        {
            if (minimum[i] > other.maximum[i] || maximum[i] < other.minimum[i])
            {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] constexpr bool Contains(const Vec<Type, Dimension>& point) const noexcept
    {
        for (std::uint8_t i{}; i < Dimension; ++i)
        {
            if (point[i] < minimum[i] || point[i] > maximum[i])
            {
                return false;
            }
        }
        return true;
    }

    static constexpr bool Intersect(const aabb& a, const aabb& b) noexcept
    {
        return a.Intersect(b);
    }
};

template <typename Type>
using aabb2d = aabb<Type, 2>;
template <typename Type>
using aabb3d = aabb<Type, 3>;
template <typename Type>
using aabb4d = aabb<Type, 4>;

using aabb2d_int8     = aabb2d<std::int8_t>;
using aabb2d_int16    = aabb2d<std::int16_t>;
using aabb2d_int32    = aabb2d<std::int32_t>;
using aabb2d_int64    = aabb2d<std::int64_t>;
using aabb2d_uint8    = aabb2d<std::uint8_t>;
using aabb2d_uint16   = aabb2d<std::uint16_t>;
using aabb2d_uint32   = aabb2d<std::uint32_t>;
using aabb2d_uint64   = aabb2d<std::uint64_t>;
using aabb2d_float32  = aabb2d<float>;
using aabb2d_float64  = aabb2d<double>;
using aabb2d_float128 = aabb2d<long double>;

using aabb3d_int8     = aabb3d<std::int8_t>;
using aabb3d_int16    = aabb3d<std::int16_t>;
using aabb3d_int32    = aabb3d<std::int32_t>;
using aabb3d_int64    = aabb3d<std::int64_t>;
using aabb3d_uint8    = aabb3d<std::uint8_t>;
using aabb3d_uint16   = aabb3d<std::uint16_t>;
using aabb3d_uint32   = aabb3d<std::uint32_t>;
using aabb3d_uint64   = aabb3d<std::uint64_t>;
using aabb3d_float32  = aabb3d<float>;
using aabb3d_float64  = aabb3d<double>;
using aabb3d_float128 = aabb3d<long double>;

using aabb4d_int8     = aabb4d<std::int8_t>;
using aabb4d_int16    = aabb4d<std::int16_t>;
using aabb4d_int32    = aabb4d<std::int32_t>;
using aabb4d_int64    = aabb4d<std::int64_t>;
using aabb4d_uint8    = aabb4d<std::uint8_t>;
using aabb4d_uint16   = aabb4d<std::uint16_t>;
using aabb4d_uint32   = aabb4d<std::uint32_t>;
using aabb4d_uint64   = aabb4d<std::uint64_t>;
using aabb4d_float32  = aabb4d<float>;
using aabb4d_float64  = aabb4d<double>;
using aabb4d_float128 = aabb4d<long double>;
} // namespace deer_geometry
