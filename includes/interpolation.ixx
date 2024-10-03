//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

module;
#include <cmath>
#include <cstdint>
#include <type_traits>


export module FawnAlgebra.Interpolation;

import FawnAlgebra.Constants;
import FawnAlgebra.Trigonometric;

namespace FawnAlgebra
{
    export template<typename T>
    constexpr T toRadians( const T& degrees ) noexcept
    {
        return degrees * ( FawnAlgebra::pi<T> / static_cast<T>(180) );
    }

    export template<typename T>
    constexpr T toDegree( const T& rad ) noexcept
    {
        return rad * ( static_cast<T>(180) / FawnAlgebra::pi<T> );
    }

    export template<typename T>
    constexpr T fade( const T& t ) noexcept
    {
        return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
    }

    export template<typename T>
    constexpr T grad( const uint32_t hash, const T& x, const T& y, const T& z ) noexcept
    {
        const uint32_t h = hash & 15U;// CONVERT LO 4 BITS OF HASH CODE
        const T& u = h < 8U ? x : y;  // INTO 12 GRADIENT DIRECTIONS.
        const T& v = h < 4U ? y : h == 12 || h == 14 ? x : z;
        return ( ( h & 1U ) == 0U ? u : -u ) + ( ( h & 2U ) == 0U ? v : -v );
    }

    export template<typename V, typename T>
    constexpr T lerp( const V& a, const V& b, const T& t ) noexcept
    {
        return std::lerp( a, b, t );
    }

    export template<typename V, typename T = V>
    constexpr T ilerp( const V& a, const V& b, const V& v ) noexcept
    {
        return static_cast<T>(v - a) / static_cast<T>(b - a);
    }

    export template<typename T>
    constexpr T remap( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
    {
        const T& t = ilerp( iMin, iMax, v );
        return lerp( oMin, oMax, t );
    }

    export template<typename V, typename T>
    constexpr T lerpAngleEuler( const V& a, const V& b, const T& t ) noexcept
    {
        if ( std::abs( 180 - std::abs( b - a ) ) <= static_cast<T>(1.0e-6) )// avoid rotating counter-clockwise if a difference angle is 180 degree (180 vs -180, it otherwise has a preference for -180)
        {
            return a + 180 * t;
        }
        const V& shortestAngle = static_cast<T>(std::fmod( static_cast<T>(std::fmod( b - a, 360 )) + 540, 360 )) - 180;
        return a + shortestAngle * t;
    }

    export template<typename V, typename T = V>
    constexpr T ilerpAngleEuler( const V& a, const V& b, const V& v ) noexcept
    {
        if ( std::abs( 180 - std::abs( b - a ) ) <= static_cast<T>(1.0e-6) )// avoid rotating counter-clockwise if a difference angle is 180 degree (180 vs -180, it otherwise has a preference for -180)
        {
            return static_cast<T>(( v - a ) / 180);
        }
        const V& shortestAngle = static_cast<T>(std::fmod( static_cast<T>(std::fmod( a - b, 360 )) + 540, 360 )) - 180;
        return static_cast<T>(( v - a ) / shortestAngle);
    }

    export template<typename T>
    constexpr T remapAngleToValueEuler( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
    {
        const T& t = ilerpAngleEuler( iMin, iMax, v );
        return lerp( oMin, oMax, t );
    }

    export template<typename T>
    constexpr T remapAngleFromValueEuler( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
    {
        const T& t = ilerp( iMin, iMax, v );
        return lerpAngleEuler( oMin, oMax, t );
    }

    export template<typename T>
    constexpr T remapAngleEuler( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
    {
        const T& t = ilerpAngleEuler( iMin, iMax, v );
        return lerpAngleEuler( oMin, oMax, t );
    }

    export template<typename V, typename T>
    constexpr T lerpAngleRad( const V& a, const V& b, const T& t ) noexcept
    {
        if ( std::abs( pi<T> - std::abs( b - a ) ) <= static_cast<T>(1.0e-6) )// avoid rotating counter-clockwise if a difference angle is 180 degree (180 vs -180, it otherwise has a preference for -180)
        {
            return a + pi<T> * t;
        }
        const V& shortestAngle = std::fmod( std::fmod( a - b, FawnAlgebra::two_pi<T> ) + 3 * FawnAlgebra::pi<T>, FawnAlgebra::two_pi<T> ) - FawnAlgebra::pi<T>;
        return a + shortestAngle * t;
    }

    export template<typename V, typename T = float>
    constexpr T ilerpAngleRad( const V& a, const V& b, const V& v ) noexcept
    {
        if ( std::abs( pi<T> - std::abs( b - a ) ) <= static_cast<T>(1.0e-6) )// avoid rotating counter-clockwise if a difference angle is 180 degree (180 vs -180, it otherwise has a preference for -180)
        {
            return static_cast<T>(( v - a ) / pi<T>);
        }
        const V& shortestAngle = std::fmod( std::fmod( a - b, FawnAlgebra::pi<T> ) + 3 * FawnAlgebra::pi<T>, FawnAlgebra::two_pi<T> ) - FawnAlgebra::pi<T>;
        return static_cast<T>(( v - a ) / shortestAngle);
    }

    export template<typename T>
    constexpr T remapAngleToValueRad( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
    {
        const T& t = ilerpAngleRad( iMin, iMax, v );
        return lerp( oMin, oMax, t );
    }

    export template<typename T>
    constexpr T remapAngleFromValueRad( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
    {
        const T& t = ilerp( iMin, iMax, v );
        return lerpAngleRad( oMin, oMax, t );
    }

    export template<typename T>
    constexpr T remapAngleRad( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
    {
        const T& t = ilerpAngleRad( iMin, iMax, v );
        return lerpAngleRad( oMin, oMax, t );
    }

    // lerp with correct corelation to time :p
    export template<typename T>
    constexpr T exponentialDecay( const T& a, const T& b, const T& decay, const T& dt ) noexcept
    {
        return b + ( a - b ) * std::exp( -decay * dt );
    }

    // https://easings.net/
    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T easeInSine( const T& v, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true ) noexcept
    {
        return 1 - FawnAlgebra::cos( v * FawnAlgebra::pi<T> / 2 );
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T easeOutSine( const T& v ) noexcept
    {
        return FawnAlgebra::sin( v * FawnAlgebra::pi<T> / 2 );
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    constexpr T easeInOutSine( const T& v ) noexcept
    {
        return -( FawnAlgebra::cos( FawnAlgebra::pi<T> * v ) - 1 ) / 2;
    }

    export template<typename T>
    constexpr T easeInQuad( const T& v ) noexcept
    {
        return v * v;
    }

    export template<typename T>
    constexpr T easeOutQuad( const T& v ) noexcept
    {
        const T& t = 1 - v;
        return 1 - t * t;
    }

    export template<typename T>
    constexpr T easeInOutQuad( const T& v ) noexcept
    {
        const T& t = -2 * v + 2;
        return v < static_cast<T>(0.5) ? 2 * v * v : 1 - t * t / 2;
    }

    export template<typename T>
    constexpr T easeInCube( const T& v ) noexcept
    {
        return v * v * v;
    }

    export template<typename T>
    constexpr T easeOutCube( const T& v ) noexcept
    {
        const T& t = 1 - v;
        return 1 - t * t * t;
    }

    export template<typename T>
    constexpr T easeInOutCube( const T& v ) noexcept
    {
        const T& t = -2 * v + 2;
        return v < static_cast<T>(0.5) ? 4 * v * v * v : 1 - t * t * t / 2;
    }

    export template<typename T>
    constexpr T easeInQuart( const T& v ) noexcept
    {
        return v * v * v * v;
    }

    export template<typename T>
    constexpr T easeOutQuart( const T& v ) noexcept
    {
        const T& t = 1 - v;
        return 1 - t * t * t * t;
    }

    export template<typename T>
    constexpr T easeInOutQuart( const T& v ) noexcept
    {
        const T& t = -2 * v + 2;
        return v < static_cast<T>(0.5) ? 8 * v * v * v * v : 1 - t * t * t * t / 2;
    }

    export template<typename T>
    constexpr T easeInQuint( const T& v ) noexcept
    {
        return v * v * v * v * v;
    }

    export template<typename T>
    constexpr T easeOutQuint( const T& v ) noexcept
    {
        const T& t = 1 - v;
        return 1 - t * t * t * t * t;
    }

    export template<typename T>
    constexpr T easeInOutQuint( const T& v ) noexcept
    {
        const T& t = -2 * v + 2;
        return v < static_cast<T>(0.5) ? 16 * v * v * v * v * v : 1 - t * t * t * t * t / 2;
    }

    export template<typename T>
    constexpr T easeInExpo( const T& v ) noexcept
    {
        return v == 0 ? 0 : static_cast<T>(std::pow( 2, 10 * v - 10 ));
    }

    export template<typename T>
    constexpr T easeOutExpo( const T& v ) noexcept
    {
        return v == 1 ? 1 : 1 - static_cast<T>(std::pow( 2, -10 * v ));
    }

    export template<typename T>
    constexpr T easeInOutExpo( const T& v ) noexcept
    {
        return v == 0 ? 0 : v == 1 ? 1 : v < static_cast<T>(0.5) ? static_cast<T>(std::pow( 2, 20 * v - 10 )) / 2 : ( 2 - static_cast<T>(std::pow( 2, -20 * v + 10 )) ) / 2;
    }

    export template<typename T>
    constexpr T easeInCirc( const T& v ) noexcept
    {
        return 1 - std::sqrt( 1 - v * v );
    }

    export template<typename T>
    constexpr T easeOutCirc( const T& v ) noexcept
    {
        return std::sqrt( 1 - ( v - 1 ) * ( v - 1 ) );
    }

    export template<typename T>
    constexpr T easeInOutCirc( const T& v ) noexcept
    {
        return v < static_cast<T>(0.5) ? ( 1 - static_cast<T>(std::sqrt( 1 - static_cast<T>(std::pow( 2 * v, 2 )) )) ) / 2 : ( static_cast<T>(std::sqrt( 1 - static_cast<T>(std::pow( -2 * v + 2, 2 )) )) + 1 ) / 2;
    }

    export template<typename T>
    constexpr T easeInBack( const T& v ) noexcept
    {
        constexpr T c1 = static_cast<T>(1.70158);
        constexpr T c2 = c1 + 1;

        return c2 * v * v * v - c1 * v * v;
    }

    export template<typename T>
    constexpr T easeOutBack( const T& v ) noexcept
    {
        constexpr T c1 = static_cast<T>(1.70158);
        constexpr T c2 = c1 + 1;

        const T a = v - 1;
        const T a1 = a * a;
        const T a2 = a * a * a;

        return 1 + c2 * a2 + c1 * a1;
    }

    export template<typename T>
    constexpr T easeInOutBack( const T& v ) noexcept
    {
        constexpr T c1 = static_cast<T>(1.70158);
        constexpr T c2 = c1 + 1;

        return v < 0.5 ? static_cast<T>(std::pow( 2 * v, 2 )) * ( ( c2 + 1 ) * 2 * v - c2 ) / 2 : ( static_cast<T>(std::pow( 2 * v - 2, 2 )) * ( ( c2 + 1 ) * ( v * 2 - 2 ) + c2 ) + 2 ) / 2;
    }

    export template<typename T>
    constexpr T easeInElastic( const T& v ) noexcept
    {
        constexpr T c1 = 2 * FawnAlgebra::pi<T> / 3;

        return v == 0 ? 0 : v == 1 ? 1 : static_cast<T>(-std::pow( 2, 10 * v - 10 )) * FawnAlgebra::sin( ( v * 10 - static_cast<T>(10.75) ) * c1 );
    }

    export template<typename T>
    constexpr T easeOutElastic( const T& v ) noexcept
    {
        constexpr T c1 = 2 * FawnAlgebra::pi<T> / 3;
        return v == 0 ? 0 : v == 1 ? 1 : static_cast<T>(std::pow( 2, -10 * v )) * FawnAlgebra::sin( ( v * 10 - static_cast<T>(0.75) ) * c1 ) + 1;
    }

    export template<typename T>
    constexpr T easeInOutElastic( const T& v ) noexcept
    {
        constexpr T c1 = 2 * FawnAlgebra::pi<T> / static_cast<T>(4.5);

        return v == 0
                ? 0
                : v == 1
                ? 1
                : v < 0.5
                ? -( static_cast<T>(std::pow( 2, 20 * v - 10 )) * FawnAlgebra::sin( ( 20 * v - static_cast<T>(11.125) ) * c1 ) ) / 2
                : static_cast<T>(std::pow( 2, -20 * v + 10 )) * FawnAlgebra::sin( ( 20 * v - static_cast<T>(11.125) ) * c1 ) / 2 + 1;
    }

    export template<typename T>
    constexpr T easeOutBounce( const T& v ) noexcept
    {
        constexpr T n1{static_cast<T>(7.5625)};
        constexpr T d1{static_cast<T>(2.75)};

        if ( v < 1 / d1 )
        {
            return n1 * v * v;
        }
        if ( v < 2 / d1 )
        {
            const T& a{v - static_cast<T>(1.5) / d1};
            return n1 * a * a + static_cast<T>(0.75);
        }
        if ( v < 2.5 / d1 )
        {
            const T& a{v - static_cast<T>(2.25) / d1};
            return n1 * a * a + static_cast<T>(0.9375);
        }
        const T& a{v - static_cast<T>(2.625) / d1};
        return n1 * a * a + static_cast<T>(0.984375);
    }

    export template<typename T>
    constexpr T easeInBounce( const T& v ) noexcept
    {
        return 1 - easeOutBounce( 1 - v );
    }

    export template<typename T>
    constexpr T easeInOutBounce( const T& v ) noexcept
    {
        return v < 0.5 ? ( 1 - easeOutBounce( 1 - 2 * v ) ) / 2 : ( 1 + easeOutBounce( 2 * v - 1 ) ) / 2;
    }
}// namespace FawnAlgebra
