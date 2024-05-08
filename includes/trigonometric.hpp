//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

#pragma once
#include <cmath>

#include "arithmetic.hpp"

namespace FawnAlgebra
{
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T sin( const T& x ) noexcept
    {
        return static_cast<T>(std::sin( static_cast<double>(x) ));
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T cos( const T& x ) noexcept
    {
        return static_cast<T>(std::cos( static_cast<double>(x) ));
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T tan( const T& x ) noexcept
    {
        return static_cast<T>(std::tan( static_cast<double>(x) ));
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T asin( const T& x ) noexcept
    {
        return static_cast<T>(std::asin( static_cast<double>(x) ));
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T acos( const T& x ) noexcept
    {
        return static_cast<T>(std::acos( static_cast<double>(x) ));
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T atan( const T& x ) noexcept
    {
        return static_cast<T>(std::atan( static_cast<double>(x) ));
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T atan2( const T& y, const T& x ) noexcept
    {
        return static_cast<T>(std::atan2( static_cast<double>(y), static_cast<double>(x) ));
    }

    constexpr float2 sin( const float2& x ) noexcept
    {
        return {std::sin( x.x ), std::sin( x.y )};
    }

    constexpr float2 cos( const float2& x ) noexcept
    {
        return {std::cos( x.x ), std::cos( x.y )};
    }

    constexpr float2 tan( const float2& x ) noexcept
    {
        return {std::tan( x.y ), std::tan( x.y )};
    }

    constexpr float2 asin( const float2& x ) noexcept
    {
        return {std::asin( x.x ), std::asin( x.y )};
    }

    constexpr float2 acos( const float2& x ) noexcept
    {
        return {std::acos( x.x ), std::acos( x.y )};
    }

    constexpr float2 atan( const float2& x ) noexcept
    {
        return {std::atan( x.x ), std::atan( x.y )};
    }

    inline float atan2( const float2& v ) noexcept
    {
        return std::atan2( v.y, v.x );
    }

    inline float2 atan2( const float2& y, const float2& x ) noexcept
    {
        return {std::atan2( y.x, x.x ), std::atan2( y.y, x.y )};
    }

    constexpr float3 sin( const float3& x ) noexcept
    {
        return {std::sin( x.x ), std::sin( x.y ), std::sin( x.z )};
    }

    constexpr float3 cos( const float3& x ) noexcept
    {
        return {std::cos( x.x ), std::cos( x.y ), std::cos( x.z )};
    }

    constexpr float3 tan( const float3& x ) noexcept
    {
        return {std::tan( x.y ), std::tan( x.y ), std::tan( x.z )};
    }

    constexpr float3 asin( const float3& x ) noexcept
    {
        return {std::asin( x.x ), std::asin( x.y ), std::asin( x.z )};
    }

    constexpr float3 acos( const float3& x ) noexcept
    {
        return {std::acos( x.x ), std::acos( x.y ), std::acos( x.z )};
    }

    constexpr float3 atan( const float3& x ) noexcept
    {
        return {std::atan( x.x ), std::atan( x.y ), std::atan( x.z )};
    }

    inline float3 atan2( const float3& v ) noexcept
    {
        return {std::atan2( v.x, v.y ), std::atan2( v.y, v.z ), std::atan2( v.z, v.x )};
    }

    inline float3 atan2( const float3& y, const float3& x ) noexcept
    {
        return {std::atan2( y.x, x.x ), std::atan2( y.y, x.y ), std::atan2( y.z, x.z )};
    }

    constexpr float4 sin( const float4& x ) noexcept
    {
        return {std::sin( x.x ), std::sin( x.y ), std::sin( x.z ), std::sin( x.w )};
    }

    constexpr float4 cos( const float4& x ) noexcept
    {
        return {std::cos( x.x ), std::cos( x.y ), std::cos( x.z ), std::cos( x.w )};
    }

    constexpr float4 tan( const float4& x ) noexcept
    {
        return {std::tan( x.y ), std::tan( x.y ), std::tan( x.z ), std::tan( x.w )};
    }

    constexpr float4 asin( const float4& x ) noexcept
    {
        return {std::asin( x.x ), std::asin( x.y ), std::asin( x.z ), std::asin( x.w )};
    }

    constexpr float4 acos( const float4& x ) noexcept
    {
        return {std::acos( x.x ), std::acos( x.y ), std::acos( x.z ), std::acos( x.w )};
    }

    constexpr float4 atan( const float4& x ) noexcept
    {
        return {std::atan( x.x ), std::atan( x.y ), std::atan( x.z ), std::atan( x.w )};
    }

    inline float4 atan2( const float4& v ) noexcept
    {
        return {std::atan2( v.x, v.y ), std::atan2( v.y, v.z ), std::atan2( v.z, v.w ), std::atan2( v.w, v.x )};
    }

    inline float4 atan2( const float4& y, const float4& x ) noexcept
    {
        return {std::atan2( y.x, x.x ), std::atan2( y.y, x.y ), std::atan2( y.z, x.z ), std::atan2( y.w, x.w )};
    }
}// namespace FawnAlgebra
