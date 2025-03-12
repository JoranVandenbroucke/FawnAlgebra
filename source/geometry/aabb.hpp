//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

import FawnAlgebra;
using namespace FawnAlgebra;

namespace DeerGeometry
{
template <typename Type, std::uint8_t Dimension>
    requires(std::is_arithmetic_v<Type> && Dimension != 0)
struct aabb
{
    Vec<Type, Dimension> bMin = Vec<Type, Dimension>(std::numeric_limits<Type>::max());
    Vec<Type, Dimension> bMax = Vec<Type, Dimension>(std::numeric_limits<Type>::min());

    constexpr void Grow(const Vec<Type, Dimension>& point) noexcept
    {
        bMin = min(bMin, point);
        bMax = max(bMax, point);
    }

    constexpr void Grow(const aabb& b) noexcept
    {
        if (b.bMin[0] != std::numeric_limits<Type>::min()) // Adjust for Dimension dimensions
        {
            Grow(b.bMin);
            Grow(b.bMax);
        }
    }

    [[nodiscard]] constexpr Type Area() const noexcept
    {
        Type result{1};
        for (std::uint8_t i{}; i < Dimension; ++i)
        {
            result *= (bMax[i] - bMin[i]);
        }
        return result;
    }

    [[nodiscard]] constexpr bool Intersect(const aabb& other) const noexcept
    {
        for (std::uint8_t i{}; i < Dimension; ++i)
        {
            if (bMin[i] > other.bMax[i] || bMax[i] < other.bMin[i])
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
            if (point[i] < bMin[i] || point[i] > bMax[i])
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

using aabb2d_int8     = aabb2d<int8_t>;
using aabb2d_int16    = aabb2d<int16_t>;
using aabb2d_int32    = aabb2d<int32_t>;
using aabb2d_int64    = aabb2d<int64_t>;
using aabb2d_uint8    = aabb2d<uint8_t>;
using aabb2d_uint16   = aabb2d<uint16_t>;
using aabb2d_uint32   = aabb2d<uint32_t>;
using aabb2d_uint64   = aabb2d<uint64_t>;
using aabb2d_float32  = aabb2d<float>;
using aabb2d_float64  = aabb2d<double>;
using aabb2d_float128 = aabb2d<long double>;

using aabb3d_int8     = aabb3d<int8_t>;
using aabb3d_int16    = aabb3d<int16_t>;
using aabb3d_int32    = aabb3d<int32_t>;
using aabb3d_int64    = aabb3d<int64_t>;
using aabb3d_uint8    = aabb3d<uint8_t>;
using aabb3d_uint16   = aabb3d<uint16_t>;
using aabb3d_uint32   = aabb3d<uint32_t>;
using aabb3d_uint64   = aabb3d<uint64_t>;
using aabb3d_float32  = aabb3d<float>;
using aabb3d_float64  = aabb3d<double>;
using aabb3d_float128 = aabb3d<long double>;

using aabb4d_int8     = aabb4d<int8_t>;
using aabb4d_int16    = aabb4d<int16_t>;
using aabb4d_int32    = aabb4d<int32_t>;
using aabb4d_int64    = aabb4d<int64_t>;
using aabb4d_uint8    = aabb4d<uint8_t>;
using aabb4d_uint16   = aabb4d<uint16_t>;
using aabb4d_uint32   = aabb4d<uint32_t>;
using aabb4d_uint64   = aabb4d<uint64_t>;
using aabb4d_float32  = aabb4d<float>;
using aabb4d_float64  = aabb4d<double>;
using aabb4d_float128 = aabb4d<long double>;
} // namespace DeerGeometry
