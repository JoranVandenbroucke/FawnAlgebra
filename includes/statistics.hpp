//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>
#include <set>
#include <type_traits>

//correlation coefficient.

namespace FawnAlgebra
{
    template<typename T, size_t N, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    constexpr T mean( T ( &a )[ N ] ) noexcept
    {
        return std::accumulate( std::begin( a ), std::end( a ), T( 0 ) ) / N;
    }

    template<typename T, size_t N, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    constexpr T median( T ( &a )[ N ] ) noexcept
    {
        return ( N & 2 ) != 0 ? a[ N / 2 ] : ( a[ ( N - 1 ) / 2 ] + a[ N / 2 ] ) / 2;
    }

    template<typename T, size_t N, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    constexpr T mode( T ( &a )[ N ] ) noexcept
    {
        T array[ N ]       = std::ranges::sort( a );
        T value            = a[ 0 ];
        T mode             = value;
        uint32_t count     = 1u;
        uint32_t countMode = 1u;

        for ( size_t i { 1 }; i < N; i++ )
        {
            if ( array[ i ] == value )
            {
                ++count;
            }
            else
            {
                if ( count > countMode )
                {
                    countMode = count;
                    mode      = value;
                }
                count = 1;
                value = array[ i ];
            }
        }
        return mode;
    }

    template<typename T, size_t N, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    constexpr T variance( T ( &a )[ N ] ) noexcept
    {
        if constexpr ( N <= 1 )
        {
            return (T) 0;
        }

        constexpr T m                = mean( a );
        constexpr size_t size        = N;
        constexpr auto variance_func = [ &m, &size ]( T accumulator, const T& val ) constexpr -> T
        {
            return accumulator + ( ( val - m ) * ( val - m ) / ( size - 1 ) );
        };

        return std::accumulate( std::begin( a ), std::end( a ), (T) 0, variance_func );
    }

    template<typename T, size_t N, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    constexpr T standarDiv( T ( &a )[ N ] ) noexcept
    {
        if constexpr ( N <= 1 )
        {
            return (T) 0;
        }

        constexpr T m                              = mean( a );
        constexpr auto standard_deviation_function = [ &m ]( T accumulator, const T& val ) constexpr -> T
        {
            return accumulator + ( val - m ) * ( val - m );
        };

        const T sum = std::accumulate( std::begin( a ), std::end( a ), (T) 0, standard_deviation_function );

        return (T) std::sqrt( ( sum * sum ) / (T) N );
    }

    template<typename T, size_t N, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    constexpr float correlationCoefficient( T ( &a )[ N ], T ( &b )[ N ] ) noexcept
    {
        T sumX        = (T) 0;
        T sumY        = (T) 0;
        T sumXY       = (T) 0;
        T squareSumX  = (T) 0;
        T squareSumY  = (T) 0;

        for ( int i = 0; i < N; i++ )
        {
            sumX  = sumX + a[ i ];
            sumY  = sumY + b[ i ];

            sumXY = sumXY + a[ i ] * b[ i ];

            squareSumX  = squareSumX + a[ i ] * a[ i ];
            squareSumY  = squareSumY + b[ i ] * b[ i ];
        }

        // use formula for calculating correlation coefficient.
        float corr = (float) ( N * sumXY - sumX * sumY ) / std::sqrtf( ( N * squareSumX - sumX * sumX ) * ( N * squareSumY - sumY * sumY ) );

        return corr;
    }
}// namespace FawnAlgebra
