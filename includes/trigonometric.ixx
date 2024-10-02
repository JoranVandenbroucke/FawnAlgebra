//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

module;
#include <cmath>

#include "config/todo.hpp"

export module FawnAlgebra.Trigonometric;


namespace FawnAlgebra
{
    // no need to make thing inline as it's implicit
    // can't make them constexpr as arithmatics can't be calculated at compile time (yet)
    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T sin( const T& x ) noexcept
    {
        return static_cast<T>(std::sin( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T cos( const T& x ) noexcept
    {
        return static_cast<T>(std::cos( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T tan( const T& x ) noexcept
    {
        return static_cast<T>(std::tan( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T cotan( const T& x ) noexcept
    {
        return static_cast<T>(1 / tan( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T secant( const T& x ) noexcept
    {
        return static_cast<T>(1 / cos( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T cosecant( const T& x ) noexcept
    {
        return static_cast<T>(1 / sin( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T arcsin( const T& x ) noexcept
    {
        return static_cast<T>(std::asin( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T arccos( const T& x ) noexcept
    {
        return static_cast<T>(std::acos( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T arctan( const T& x ) noexcept
    {
        return static_cast<T>(std::atan( static_cast<double>(x) ));
    }

    export template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
    T arctan2( const T& y, const T& x ) noexcept
    {
        return static_cast<T>(std::atan2( static_cast<double>(y), static_cast<double>(x) ));
    }

    #pragma todo("Implement the Hyperbolic versions of sin, cos, tan ..."
}// namespace FawnAlgebra
