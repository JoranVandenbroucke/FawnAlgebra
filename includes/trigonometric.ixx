//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

module;
#include <cmath>

export module FawnAlgebra.Trigonometric;

namespace FawnAlgebra
{
    // no need to make thing inline as it's implicit
    // can't make them constexpr as arithmatics can't be calculated at compile time (yet)
    export template<typename T>
        requires std::is_arithmetic_v<T>
    T sin( const T& x ) noexcept
    {
        return static_cast<T>( std::sin( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T cos( const T& x ) noexcept
    {
        return static_cast<T>( std::cos( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T tan( const T& x ) noexcept
    {
        return static_cast<T>( std::tan( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T cotan( const T& x ) noexcept
    {
        return static_cast<T>( 1 / tan( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T secant( const T& x ) noexcept
    {
        return static_cast<T>( 1 / cos( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T cosecant( const T& x ) noexcept
    {
        return static_cast<T>( 1 / sin( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T arcsin( const T& x ) noexcept
    {
        return static_cast<T>( std::asin( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T arccos( const T& x ) noexcept
    {
        return static_cast<T>( std::acos( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T arctan( const T& x ) noexcept
    {
        return static_cast<T>( std::atan( static_cast<double>( x ) ) );
    }

    export template<typename T>
        requires std::is_arithmetic_v<T>
    T arctan2( const T& y, const T& x ) noexcept
    {
        return static_cast<T>( std::atan2( static_cast<double>( y ), static_cast<double>( x ) ) );
    }
    template<typename T>
        requires std::is_arithmetic_v<T>
    T sinh( const T& x ) noexcept
    {
        return static_cast<T>( std::sinh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T cosh( const T& x ) noexcept
    {
        return static_cast<T>( std::cosh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T tanh( const T& x ) noexcept
    {
        return static_cast<T>( std::tanh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T coth( const T& x ) noexcept
    {
        return static_cast<T>( 1 / tanh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T sech( const T& x ) noexcept
    {
        return static_cast<T>( 1 / cosh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T csch( const T& x ) noexcept
    {
        return static_cast<T>( 1 / sinh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T arsinh( const T& x ) noexcept
    {
        return static_cast<T>( std::asinh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T arcosh( const T& x ) noexcept
    {
        return static_cast<T>( std::acosh( static_cast<double>( x ) ) );
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    T artanh( const T& x ) noexcept
    {
        return static_cast<T>( std::atanh( static_cast<double>( x ) ) );
    }
}// namespace FawnAlgebra
