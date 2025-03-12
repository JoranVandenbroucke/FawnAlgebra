//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

module;
#include <cmath>

export module FawnAlgebra:Trigonometric;

namespace FawnAlgebra
{
// no need to make thing inline as it's implicit
// can't make them constexpr as arithmatics can't be calculated at compile time (yet)
export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Sin( const T& x ) noexcept
{
    return static_cast<T>( std::sin( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Cos( const T& x ) noexcept
{
    return static_cast<T>( std::cos( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Tan( const T& x ) noexcept
{
    return static_cast<T>( std::tan( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Cotan( const T& x ) noexcept
{
    return static_cast<T>( 1 / tan( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Secant( const T& x ) noexcept
{
    return static_cast<T>( 1 / cos( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Cosecant( const T& x ) noexcept
{
    return static_cast<T>( 1 / sin( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Arcsin( const T& x ) noexcept
{
    return static_cast<T>( std::asin( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Arccos( const T& x ) noexcept
{
    return static_cast<T>( std::acos( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Arctan( const T& x ) noexcept
{
    return static_cast<T>( std::atan( static_cast<double>( x ) ) );
}

export template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Arctan2( const T& y, const T& x ) noexcept
{
    return static_cast<T>( std::atan2( static_cast<double>( y ), static_cast<double>( x ) ) );
}
template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Sinh( const T& x ) noexcept
{
    return static_cast<T>( std::sinh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Cosh( const T& x ) noexcept
{
    return static_cast<T>( std::cosh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Tanh( const T& x ) noexcept
{
    return static_cast<T>( std::tanh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Coth( const T& x ) noexcept
{
    return static_cast<T>( 1 / tanh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Sech( const T& x ) noexcept
{
    return static_cast<T>( 1 / cosh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Cosech( const T& x ) noexcept
{
    return static_cast<T>( 1 / sinh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Arsinh( const T& x ) noexcept
{
    return static_cast<T>( std::asinh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Arcosh( const T& x ) noexcept
{
    return static_cast<T>( std::acosh( static_cast<double>( x ) ) );
}

template<typename T>
    requires std::is_arithmetic_v<T>
constexpr T Artanh( const T& x ) noexcept
{
    return static_cast<T>( std::atanh( static_cast<double>( x ) ) );
}
}// namespace FawnAlgebra
