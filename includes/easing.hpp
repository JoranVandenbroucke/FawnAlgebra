//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include <cmath>
#include <numbers>
#include <type_traits>

namespace FawnAlgebra
{
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T toRadians( T degrees ) noexcept
    {
        return degrees * ( std::numbers::pi_v<T> / 180 );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T toDegree( T rad ) noexcept
    {
        return rad * ( 180 / std::numbers::pi_v<T> );
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T fade( T t ) noexcept
    {
        return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T grad( uint32_t hash, T x, T y, T z ) noexcept
    {
        const uint32_t h = hash & 15;    // CONVERT LO 4 BITS OF HASH CODE
        const T u        = h < 8 ? x : y;// INTO 12 GRADIENT DIRECTIONS.
        const T v        = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ( ( h & 1 ) == 0 ? u : -u ) + ( ( h & 2 ) == 0 ? v : -v );
    }

    template<typename V, typename T, typename = typename std::enable_if<std::is_arithmetic<V>::value, V>::type, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr V lerp( V a, V b, T t ) noexcept
    {
        return std::lerp( a, b, t );
    }
    template<typename V, typename T = float, typename = typename std::enable_if<std::is_arithmetic<V>::value, V>::type, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T ilerp( V a, V b, V v ) noexcept
    {
        return (T) ( ( v - a ) / ( b - a ) );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T remap( T iMin, T iMax, T oMin, T oMax, T v ) noexcept
    {
        T t = ilerp( iMin, iMax, v );
        return lerp( oMin, oMax, t, v );
    }

    template<typename V, typename T, typename = typename std::enable_if<std::is_arithmetic<V>::value, V>::type, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr V lerp_angle_euler( V a, V b, T t ) noexcept
    {
        const V shortest_angle = std::fmod( std::fmod( ( a - b ), 360 ) + 540, 360 ) - 180;
        return a + shortest_angle * t;
    }
    template<typename V, typename T = float, typename = typename std::enable_if<std::is_arithmetic<V>::value, V>::type, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T ilerp_angle_euler( V a, V b, V v ) noexcept
    {
        const V shortest_angle = std::fmod( std::fmod( ( a - b ), 360 ) + 540, 360 ) - 180;
        return (T) ( ( v - a ) / shortest_angle );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T remap_angle_to_value_euler( T iMin, T iMax, T oMin, T oMax, T v ) noexcept
    {
        const T t = ilerp_angle_euler( iMin, iMax, v );
        return lerp( oMin, oMax, t, v );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T remap_angle_from_value_euler( T iMin, T iMax, T oMin, T oMax, T v ) noexcept
    {
        const T t = ilerp( iMin, iMax, v );
        return lerp_angle_euler( oMin, oMax, t, v );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T remap_angle_euler( T iMin, T iMax, T oMin, T oMax, T v ) noexcept
    {
        const T t = ilerp_angle_euler( iMin, iMax, v );
        return lerp_angle_euler( oMin, oMax, t, v );
    }

    template<typename V, typename T, typename = typename std::enable_if<std::is_arithmetic<V>::value, V>::type, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr V lerp_angle_rad( V a, V b, T t ) noexcept
    {
        const V shortest_angle = std::fmod( std::fmod( ( a - b ), 2 * std::numbers::pi_v<T> ) + 3 * std::numbers::pi_v<T>, 2 * std::numbers::pi_v<T> ) - std::numbers::pi_v<T>;
        return a + shortest_angle * t;
    }
    template<typename V, typename T = float, typename = typename std::enable_if<std::is_arithmetic<V>::value, V>::type, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T ilerp_angle_rad( V a, V b, V v ) noexcept
    {
        const V shortest_angle = std::fmod( std::fmod( ( a - b ), 2 * std::numbers::pi_v<T> ) + 3 * std::numbers::pi_v<T>, 2 * std::numbers::pi_v<T> ) - std::numbers::pi_v<T>;
        return (T) ( ( v - a ) / shortest_angle );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T remap_angle_to_value_rad( T iMin, T iMax, T oMin, T oMax, T v ) noexcept
    {
        const T t = ilerp_angle_rad( iMin, iMax, v );
        return lerp( oMin, oMax, t, v );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T remap_angle_from_value_rad( T iMin, T iMax, T oMin, T oMax, T v ) noexcept
    {
        const T t = ilerp( iMin, iMax, v );
        return lerp_angle_rad( oMin, oMax, t, v );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T remap_angle_rad( T iMin, T iMax, T oMin, T oMax, T v ) noexcept
    {
        const T t = ilerp_angle_rad( iMin, iMax, v );
        return lerp_angle_rad( oMin, oMax, t, v );
    }

    // https://easings.net/
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInSine( T v ) noexcept
    {
        return 1 - std::cos( (v * std::numbers::pi_v<T>) / (T) 2.0 );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutSine( T v ) noexcept
    {
        return std::sin( (v * std::numbers::pi_v<T>) / (T) 2.0 );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutSine( T v ) noexcept
    {
        return -( std::cos( std::numbers::pi_v<T> * v ) - 1 ) / (T) 2.0;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInQuad( T v ) noexcept
    {
        return v * v;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutQuad( T v ) noexcept
    {
        T t = 1 - v;
        return 1 - t * t;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutQuad( T v ) noexcept
    {
        T t = -2 * v + 2;
        return v < 0.5 ? 2 * v * v : 1 - ( t * t ) / 2;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInCube( T v ) noexcept
    {
        return v * v * v;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutCube( T v ) noexcept
    {
        T t = 1 - v;
        return 1 - t * t * t;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutCube( T v ) noexcept
    {
        T t = -2 * v + 2;
        return v < 0.5 ? 4 * v * v * v : 1 - ( t * t * t ) / 2;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInQuart( T v ) noexcept
    {
        return v * v * v * v;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutQueart( T v ) noexcept
    {
        T t = 1 - v;
        return 1 - t * t * t * t;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutQuart( T v ) noexcept
    {
        T t = -2 * v + 2;
        return v < 0.5 ? 8 * v * v * v * v : 1 - ( t * t * t * t ) / 2;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInQuint( T v ) noexcept
    {
        return v * v * v * v * v;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutQuint( T v ) noexcept
    {
        T t = 1 - v;
        return 1 - t * t * t * t * t;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutQuint( T v ) noexcept
    {
        T t = -2 * v + 2;
        return v < 0.5 ? 16 * v * v * v * v * v : 1 - ( t * t * t * t * t ) / 2;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInExpo( T v ) noexcept
    {
        return v == 0 ? 0 : std::pow( 2, 10 * v - 10 );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutExpo( T v ) noexcept
    {
        return v == 1 ? 1 : 1 - std::pow( 2, -10 * v );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutExpo( T v ) noexcept
    {
        return v == 0 ? 0 : v == 1 ? 1 : v < 0.5 ? std::pow( 2, 20 * v - 10 ) / 2 : ( 2 - std::pow( 2, -20 * v + 10 ) ) / 2;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInCirc( T v ) noexcept
    {
        return 1 - std::sqrt( 1 - std::pow( v, 2 ) );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutCirc( T v ) noexcept
    {
        return std::sqrt( 1 - std::pow( v - 1, 2 ) );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutCirc( T v ) noexcept
    {
        return v < 0.5 ? ( 1 - std::sqrt( 1 - std::pow( 2 * v, 2 ) ) ) / 2 : ( std::sqrt( 1 - std::pow( -2 * v + 2, 2 ) ) + 1 ) / 2;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInBack( T v ) noexcept
    {
        constexpr T c1 = 1.70158;
        constexpr T c2 = c1 + 1;

        return c2 * v * v * v - c1 * v * v;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutBack( T v ) noexcept
    {
        constexpr T c1 = 1.70158;
        constexpr T c2 = c1 + 1;

        return 1 + c2 * std::pow( v - 1, 3 ) + c1 * std::pow( v - 1, 2 );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutBack( T v ) noexcept
    {
        constexpr T c1 = 1.70158;
        constexpr T c2 = c1 + 1;

        return v < 0.5 ? ( std::pow( 2 * v, 2 ) * ( ( c2 + 1 ) * 2 * v - c2 ) ) / 2 : ( std::pow( 2 * v - 2, 2 ) * ( ( c2 + 1 ) * ( v * 2 - 2 ) + c2 ) + 2 ) / 2;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInElastic( T v ) noexcept
    {
        constexpr T c1 = (2 * std::numbers::pi_v<T>) / 3;

        return v == 0 ? 0 : v == 1 ? 1 : -std::pow( 2, 10 * v - 10 ) * std::sin( ( v * 10 - 10.75 ) * c1 );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutElastic( T v ) noexcept
    {
        constexpr T c1 = (2 * std::numbers::pi_v<T>) / 3;
        return v == 0 ? 0 : v == 1 ? 1 : std::pow( 2, -10 * v ) * std::sin( ( v * 10 - 0.75 ) * c1 ) + 1;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutElastic( T v ) noexcept
    {
        constexpr T c1 = (2 * std::numbers::pi_v<T>) / 4.5;

        return v == 0 ? 0 : v == 1 ? 1 : v < 0.5 ? -( std::pow( 2, 20 * v - 10 ) * std::sin( ( 20 * v - 11.125 ) * c1 ) ) / 2 : ( std::pow( 2, -20 * v + 10 ) * std::sin( ( 20 * v - 11.125 ) * c1 ) ) / 2 + 1;
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInBounce( T v ) noexcept
    {
        return 1 - easeInBounce( 1 - v );
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeOutBounce( T v ) noexcept
    {
        constexpr T n1 = 7.5625;
        constexpr T d1 = 2.75;

        if ( v < 1 / d1 )
        {
            return n1 * v * v;
        }
        if ( v < 2 / d1 )
        {
            return n1 * ( v -= 1.5 / d1 ) * v + 0.75;
        }
        if ( v < 2.5 / d1 )
        {
            return n1 * ( v -= 2.25 / d1 ) * v + 0.9375;
        }
        return n1 * ( v -= 2.625 / d1 ) * v + 0.984375;
    }
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    constexpr T easeInOutBounce( T v ) noexcept
    {
        return v < 0.5 ? ( 1 - easeOutBounce( 1 - 2 * v ) ) / 2 : ( 1 + easeOutBounce( 2 * v - 1 ) ) / 2;
    }
}// namespace FawnAlgebra
