//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

module;
#include <cmath>

export module FawnAlgebra.Interpolation;

import FawnAlgebra.Arithmetics;
import FawnAlgebra.Constants;
import FawnAlgebra.Trigonometric;

namespace FawnAlgebra
{
export template<typename T>
constexpr T ToRadians( const T& degrees ) noexcept
{
    return degrees * ( FawnAlgebra::pi<T> / static_cast<T>(180) );
}

export template<typename T>
constexpr T ToDegree( const T& rad ) noexcept
{
    return rad * ( static_cast<T>(180) / FawnAlgebra::pi<T> );
}

export template<typename T>
constexpr T Fade( const T& t ) noexcept
{
    return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
}

export template<typename T>
constexpr T Grad( const uint32 hash, const T& x, const T& y, const T& z ) noexcept
{
    const uint32 h{hash & 15U};// CONVERT LO 4 BITS OF HASH CODE
    const T& u{h < 8U ? x : y};// INTO 12 GRADIENT DIRECTIONS.
    const T& v{h < 4U ? y : h == 12 || h == 14 ? x : z};
    return ( ( h & 1U ) == 0U ? u : -u ) + ( ( h & 2U ) == 0U ? v : -v );
}

export template<typename V, typename T>
constexpr T Lerp( const V& a, const V& b, const T& t ) noexcept
{
    return std::lerp( a, b, t );
}

export template<typename V, typename T = V>
constexpr T Ilerp( const V& a, const V& b, const V& v ) noexcept
{
    return static_cast<T>(v - a) / static_cast<T>(b - a);
}

export template<typename T>
constexpr T Remap( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
{
    return Lerp( oMin, oMax, Ilerp( iMin, iMax, v ) );
}

export template<typename V, typename T>
constexpr T LerpAngleEuler( const V& a, const V& b, const T& t ) noexcept
{
    // Define a small epsilon for floating-point comparisons
    constexpr T epsilon{static_cast<T>(1e-6)};

    // Calculate the difference between the angles
    V difference{b - a};

    // Check if the angle difference is approximately 180 degrees
    if ( std::abs( 180.0 - std::abs( difference ) ) <= epsilon )
    {
        return static_cast<T>(a + 180.0 * t);
    }

    // Calculate the shortest angle using the provided logic
    V shortestAngle{static_cast<V>(std::fmod( std::fmod( difference, 360 ) + 540.0, 360.0 ) - 180.0)};
    if ( std::abs( shortestAngle ) <= static_cast<T>(1e-6) )
    {
        return static_cast<T>(a + 360.0 * t);
    }

    return static_cast<T>(a + shortestAngle * t);
}

export template<typename V, typename T = V>
constexpr T IlerpAngleEuler( const V& a, const V& b, const V& v ) noexcept
{
    // Define a small epsilon for floating-point comparisons
    constexpr T epsilon{static_cast<T>(1e-6)};

    // Calculate the difference between the angles
    T difference{b - a};

    // Check if the angle difference is approximately 180 degrees
    if ( std::abs( 180.0 - std::abs( difference ) ) <= epsilon )
    {
        return static_cast<T>(( v - a ) / 180.0);
    }

    // Calculate the shortest angle using the provided logic
    V shortestAngle{static_cast<V>(std::fmod( std::fmod( difference, 360 ) + 540.0, 360.0 ) - 180.0)};

    // Check if the shortest angle is nearly zero
    if ( std::abs( shortestAngle ) <= epsilon )
    {
        return static_cast<T>(( v - a ) / 360.0);
    }

    return static_cast<T>(( v - a ) / shortestAngle);
}

export template<typename T>
constexpr T RemapAngleToValueEuler( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
{
    const T& t{IlerpAngleEuler( iMin, iMax, v )};
    return Lerp( oMin, oMax, t );
}

export template<typename T>
constexpr T RemapAngleFromValueEuler( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
{
    const T& t{Ilerp( iMin, iMax, v )};
    return LerpAngleEuler( oMin, oMax, t );
}

export template<typename T>
constexpr T RemapAngleEuler( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
{
    const T& t{IlerpAngleEuler( iMin, iMax, v )};
    return LerpAngleEuler( oMin, oMax, t );
}

export template<typename V, typename T>
constexpr T LerpAngleRad( const V& a, const V& b, const T& t ) noexcept
{
    // Define a small epsilon for floating-point comparisons
    constexpr T epsilon{static_cast<T>(1e-6)};

    // Calculate the difference between the angles
    T difference{b - a};

    // Check if the angle difference is approximately 180 degrees
    if ( std::abs( FawnAlgebra::pi<V> - std::abs( difference ) ) <= epsilon )
    {
        return a + FawnAlgebra::pi<V> * t;
    }

    // Calculate the shortest angle using the provided logic
    V shortestAngle{static_cast<V>(std::fmod( std::fmod( difference, FawnAlgebra::two_pi<V> ) + 3 * FawnAlgebra::pi<V>, FawnAlgebra::two_pi<V> ) - FawnAlgebra::pi<V>)};

    if ( std::abs( shortestAngle ) <= static_cast<T>(1e-6) )
    {
        return a + FawnAlgebra::two_pi<V> * t;
    }
    return a + shortestAngle * t;
}

export template<typename V, typename T = float>
constexpr T IlerpAngleRad( const V& a, const V& b, const V& v ) noexcept
{
    // Define a small epsilon for floating-point comparisons
    constexpr T epsilon{static_cast<T>(1e-6)};

    // Calculate the difference between the angles
    T difference{b - a};

    // Check if the angle difference is approximately 180 degrees
    if ( std::abs( FawnAlgebra::pi<V> - std::abs( difference ) ) <= epsilon )
    {
        return static_cast<T>(( v - a ) / FawnAlgebra::pi<V>);
    }

    // Calculate the shortest angle using the provided logic
    const V shortestAngle{static_cast<V>(std::fmod( std::fmod( difference, FawnAlgebra::two_pi<V> ) + 3 * FawnAlgebra::pi<V>, FawnAlgebra::two_pi<V> ) - FawnAlgebra::pi<V>)};

    // if shortest angle is 0, return a
    if ( std::abs( shortestAngle ) <= static_cast<T>(1e-6) )
    {
        return static_cast<T>(( v - a ) / FawnAlgebra::two_pi<V>);
    }
    return static_cast<T>(( v - a ) / shortestAngle);
}

export template<typename T>
constexpr T RemapAngleToValueRad( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
{
    const T& t{IlerpAngleRad( iMin, iMax, v )};
    return Lerp( oMin, oMax, t );
}

export template<typename T>
constexpr T RemapAngleFromValueRad( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
{
    const T& t{Ilerp( iMin, iMax, v )};
    return LerpAngleRad( oMin, oMax, t );
}

export template<typename T>
constexpr T RemapAngleRad( const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& v ) noexcept
{
    const T& t{IlerpAngleRad( iMin, iMax, v )};
    return LerpAngleRad( oMin, oMax, t );
}

// lerp with correct correlation to time :p
export template<typename T>
constexpr T ExponentialDecay( const T& a, const T& b, const T& decay, const T& dt ) noexcept
{
    return b + ( a - b ) * std::exp( -decay * dt );
}

// https://easings.net/
export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T EaseInSine( const T& v ) noexcept
{
    return 1 - FawnAlgebra::cos( v * FawnAlgebra::pi<T> / 2 );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T EaseOutSine( const T& v ) noexcept
{
    return FawnAlgebra::sin( v * FawnAlgebra::pi<T> / 2 );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T EaseInOutSine( const T& v ) noexcept
{
    return -( FawnAlgebra::cos( FawnAlgebra::pi<T> * v ) - 1 ) / 2;
}

export template<typename T>
constexpr T EaseInQuad( const T& v ) noexcept
{
    return v * v;
}

export template<typename T>
constexpr T EaseOutQuad( const T& v ) noexcept
{
    const T& t{1 - v};
    return 1 - t * t;
}

export template<typename T>
constexpr T EaseInOutQuad( const T& v ) noexcept
{
    const T& t{-2 * v + 2};
    return v < static_cast<T>(0.5) ? 2 * v * v : 1 - t * t / 2;
}

export template<typename T>
constexpr T EaseInCube( const T& v ) noexcept
{
    return v * v * v;
}

export template<typename T>
constexpr T EaseOutCube( const T& v ) noexcept
{
    const T& t{1 - v};
    return 1 - t * t * t;
}

export template<typename T>
constexpr T EaseInOutCube( const T& v ) noexcept
{
    const T& t{-2 * v + 2};
    return v < static_cast<T>(0.5) ? 4 * v * v * v : 1 - t * t * t / 2;
}

export template<typename T>
constexpr T EaseInQuart( const T& v ) noexcept
{
    return v * v * v * v;
}

export template<typename T>
constexpr T EaseOutQuart( const T& v ) noexcept
{
    const T& t{1 - v};
    return 1 - t * t * t * t;
}

export template<typename T>
constexpr T EaseInOutQuart( const T& v ) noexcept
{
    const T& t{-2 * v + 2};
    return v < static_cast<T>(0.5) ? 8 * v * v * v * v : 1 - t * t * t * t / 2;
}

export template<typename T>
constexpr T EaseInQuint( const T& v ) noexcept
{
    return v * v * v * v * v;
}

export template<typename T>
constexpr T EaseOutQuint( const T& v ) noexcept
{
    const T& t{1 - v};
    return 1 - t * t * t * t * t;
}

export template<typename T>
constexpr T EaseInOutQuint( const T& v ) noexcept
{
    const T& t{-2 * v + 2};
    return v < static_cast<T>(0.5) ? 16 * v * v * v * v * v : 1 - t * t * t * t * t / 2;
}

export template<typename T>
constexpr T EaseInExpo( const T& v ) noexcept
{
    return v == 0 ? 0 : static_cast<T>(std::pow( 2, 10 * v - 10 ));
}

export template<typename T>
constexpr T EaseOutExpo( const T& v ) noexcept
{
    return v == 1 ? 1 : 1 - static_cast<T>(std::pow( 2, -10 * v ));
}

export template<typename T>
constexpr T EaseInOutExpo( const T& v ) noexcept
{
    return v == 0 ? 0 : v == 1 ? 1 : v < static_cast<T>(0.5) ? static_cast<T>(std::pow( 2, 20 * v - 10 )) / 2 : ( 2 - static_cast<T>(std::pow( 2, -20 * v + 10 )) ) / 2;
}

export template<typename T>
constexpr T EaseInCirc( const T& v ) noexcept
{
    return 1 - std::sqrt( 1 - v * v );
}

export template<typename T>
constexpr T EaseOutCirc( const T& v ) noexcept
{
    return std::sqrt( 1 - ( v - 1 ) * ( v - 1 ) );
}

export template<typename T>
constexpr T EaseInOutCirc( const T& v ) noexcept
{
    return v < static_cast<T>(0.5) ? ( 1 - static_cast<T>(std::sqrt( 1 - static_cast<T>(std::pow( 2 * v, 2 )) )) ) / 2 : ( static_cast<T>(std::sqrt( 1 - static_cast<T>(std::pow( -2 * v + 2, 2 )) )) + 1 ) / 2;
}

export template<typename T>
constexpr T EaseInBack( const T& v ) noexcept
{
    constexpr T c1{static_cast<T>(1.70158)};
    constexpr T c2{c1 + 1};

    return c2 * v * v * v - c1 * v * v;
}

export template<typename T>
constexpr T EaseOutBack( const T& v ) noexcept
{
    constexpr T c1{static_cast<T>(1.70158)};
    constexpr T c2{c1 + 1};

    const T a{v - 1};
    const T a1{a * a};
    const T a2{a * a * a};

    return 1 + c2 * a2 + c1 * a1;
}

export template<typename T>
constexpr T EaseInOutBack( const T& v ) noexcept
{
    constexpr T c1{static_cast<T>(1.70158)};
    constexpr T c2{c1 + 1};

    return v < 0.5 ? static_cast<T>(std::pow( 2 * v, 2 )) * ( ( c2 + 1 ) * 2 * v - c2 ) / 2 : ( static_cast<T>(std::pow( 2 * v - 2, 2 )) * ( ( c2 + 1 ) * ( v * 2 - 2 ) + c2 ) + 2 ) / 2;
}

export template<typename T>
constexpr T EaseInElastic( const T& v ) noexcept
{
    constexpr T c1{2 * FawnAlgebra::pi<T> / 3};

    return v == 0 ? 0 : v == 1 ? 1 : static_cast<T>(-std::pow( 2, 10 * v - 10 )) * FawnAlgebra::sin( ( v * 10 - static_cast<T>(10.75) ) * c1 );
}

export template<typename T>
constexpr T EaseOutElastic( const T& v ) noexcept
{
    constexpr T c1{2 * FawnAlgebra::pi<T> / 3};
    return v == 0 ? 0 : v == 1 ? 1 : static_cast<T>(std::pow( 2, -10 * v )) * FawnAlgebra::sin( ( v * 10 - static_cast<T>(0.75) ) * c1 ) + 1;
}

export template<typename T>
constexpr T EaseInOutElastic( const T& v ) noexcept
{
    constexpr T c1{2 * FawnAlgebra::pi<T> / static_cast<T>(4.5)};

    return v == 0
        ? 0
        : v == 1
        ? 1
        : v < 0.5
        ? -( static_cast<T>(std::pow( 2, 20 * v - 10 )) * FawnAlgebra::sin( ( 20 * v - static_cast<T>(11.125) ) * c1 ) ) / 2
        : static_cast<T>(std::pow( 2, -20 * v + 10 )) * FawnAlgebra::sin( ( 20 * v - static_cast<T>(11.125) ) * c1 ) / 2 + 1;
}

export template<typename T>
constexpr T EaseOutBounce( const T& v ) noexcept
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
constexpr T EaseInBounce( const T& v ) noexcept
{
    return 1 - EaseOutBounce( 1 - v );
}

export template<typename T>
constexpr T EaseInOutBounce( const T& v ) noexcept
{
    return v < 0.5 ? ( 1 - EaseOutBounce( 1 - 2 * v ) ) / 2 : ( 1 + EaseOutBounce( 2 * v - 1 ) ) / 2;
}
}// namespace FawnAlgebra
