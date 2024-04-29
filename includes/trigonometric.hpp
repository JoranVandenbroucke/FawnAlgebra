//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include <cmath>

namespace FawnAlgebra
{
    namespace Float
    {
        float sin( float x ) noexcept
        {
            return std::sinf( x );
        }
        float cos( float x ) noexcept
        {
            return std::cosf( x );
        }
        float tan( float x ) noexcept
        {
            return std::tanf( x );
        }
        float asin( float x ) noexcept
        {
            return std::asinf( x );
        }
        float acos( float x ) noexcept
        {
            return std::acosf( x );
        }
        float atan( float x ) noexcept
        {
            return std::atanf( x );
        }
        float atan2( float y, float x ) noexcept
        {
            return std::atan2f( y, x );
        }
    }// namespace Float
    namespace Double
    {
        double sin( double x ) noexcept
        {
            return std::sin( x );
        }
        double cos( double x ) noexcept
        {
            return std::cos( x );
        }
        double tan( double x ) noexcept
        {
            return std::tan( x );
        }
        double asin( double x ) noexcept
        {
            return std::asin( x );
        }
        double acos( double x ) noexcept
        {
            return std::acos( x );
        }
        double atan( double x ) noexcept
        {
            return std::atan( x );
        }
        double atan2( double y, double x ) noexcept
        {
            return std::atan2( y, x );
        }
    }// namespace Double
    namespace LongDouble
    {
        long double sin( long double x ) noexcept
        {
            return std::sinl( x );
        }
        long double cos( long double x ) noexcept
        {
            return std::cosl( x );
        }
        long double tan( long double x ) noexcept
        {
            return std::tanl( x );
        }
        long double asin( long double x ) noexcept
        {
            return std::asinl( x );
        }
        long double acos( long double x ) noexcept
        {
            return std::acosl( x );
        }
        long double atan( long double x ) noexcept
        {
            return std::atanl( x );
        }
        long double atan2( long double y, long double x ) noexcept
        {
            return std::atan2l( y, x );
        }
    }// namespace LongDouble
}// namespace FawnAlgebra
