//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

module;

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <random>
#include <vector>

export module FawnAlgebra.Statistics;
import FawnAlgebra.Arithmetics;
import FawnAlgebra.Constants;


namespace FawnAlgebra
{
    /**
     * @brief Calculates the sum of elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose elements are to be summed.
     * @return The sum of the elements.
     */
    template<typename Container, typename T = typename Container::value_type>
    constexpr auto sum( const Container& a ) -> T
    {
        return static_cast<T>(std::accumulate( std::cbegin( a ), std::cend( a ), 0 ));
    }

    #pragma region measures of frequency
    /**
     * @brief Generates a frequency table for the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose frequency table is to be generated.
     * @return A vector of pairs where each pair contains an element and its frequency.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto frequencyTable( Container a ) noexcept
    {
        static_assert( !std::empty( a ), "An empty array doesn't have a frequency table." );

        std::vector<std::pair<T, std::size_t>> table;
        for ( std::size_t i = 0; i < std::size( a ); i++ )
        {
            const std::size_t firstIndex = std::ranges::lower_bound( a, a[ i ] ) - std::cbegin( a );
            const std::size_t lastIndex = std::ranges::upper_bound( a, a[ i ] ) - std::cbegin( a ) - 1;
            i = lastIndex;
            table.emplace_back( std::make_pair( a[ firstIndex ], lastIndex - firstIndex + 1 ) );
        }
        return table;
    }

    /**
     * @brief Calculates the first quartile (Q1) of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose first quartile is to be calculated.
     * @return The first quartile (Q1).
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto quartilesQ1( const Container& a ) noexcept
    {
        const double idx{std::size( a ) / 4.0};
        return static_cast<T>(std::size( a ) & 3) != 0 ? a[ static_cast<std::size_t>(std::ceil( idx )) ] : ( a[ static_cast<std::size_t>(idx) ] + a[ static_cast<std::size_t>(idx + 1) ] ) / 2;
    }

    /**
     * @brief Calculates the second quartile (Q2) or median of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose second quartile (Q2) or median is to be calculated.
     * @return The second quartile (Q2) or median.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto quartilesQ2( const Container& a ) noexcept
    {
        const double idx{std::size( a ) / 2.0};
        return static_cast<T>(std::size( a ) & 1) != 0 ? a[ static_cast<std::size_t>(std::ceil( idx )) ] : ( a[ static_cast<std::size_t>(idx) ] + a[ static_cast<std::size_t>(idx + 1) ] ) / 2;
    }

    /**
     * @brief Calculates the third quartile (Q3) of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose third quartile (Q3) is to be calculated.
     * @return The third quartile (Q3).
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto quartilesQ3( const Container& a ) noexcept
    {
        const double idx{3 * std::size( a ) / 4.0};
        return static_cast<T>(std::size( a ) & 3) != 0 ? a[ static_cast<std::size_t>(std::ceil( idx )) ] : ( a[ static_cast<std::size_t>(idx) ] + a[ static_cast<std::size_t>(idx + 1) ] ) / 2;
    }
    #pragma endregion
    #pragma region central tendency
    /**
     * @brief Calculates the mean of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose mean is to be calculated.
     * @return The mean of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto mean( const Container& a ) noexcept -> T
    {
        return static_cast<T>(sum( a ) / std::size( a ));
    }

    /**
     * @brief Calculates the median of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose median is to be calculated.
     * @return The median of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto median( Container a ) noexcept -> T
    {
        static_assert( !std::empty( a ), "Can not calculate median from empty array" );

        constexpr std::size_t size{std::size( a )};
        return static_cast<T>(( size & 2 ) != 0 ? a[ size / 2 ] : ( a[ ( size - 1 ) / 2 ] + a[ size / 2 ] ) / 2);
    }

    /**
     * @brief Calculates the mode of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose mode is to be calculated.
     * @return The mode of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto mode( Container a ) noexcept -> T
    {
        static_assert( !std::empty( a ), "Can not calculate mod from empty array" );

        constexpr std::size_t size{std::size( a )};
        T value{a[ 0 ]};
        T mode{value};
        uint32 count{1u};
        uint32 countMode{1u};

        for ( std::size_t i{1U}; i < size; i++ )
        {
            if ( a[ i ] == value )
            {
                ++count;
            }
            else
            {
                if ( count > countMode )
                {
                    countMode = count;
                    mode = value;
                }
                count = 1;
                value = a[ i ];
            }
        }
        return mode;
    }

    /**
     * @brief Calculates the geometric median of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose geometric median is to be calculated.
     * @return The geometric median of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto gemetricMedian( Container a ) noexcept -> T
    {
        static_assert( !std::empty( a ), "Can not calculate median from empty array" );

        return static_cast<T>(std::pow( std::accumulate( std::cbegin( a ),
                                                         std::cend( a ),
                                                         0,
                                                         []( const T& accumulation, const T& value )
                                                         {
                                                             return accumulation * value;
                                                         } ),
                                        1.0 / std::size( a ) ));
    }

    #pragma endregion
    #pragma region measures of variability
    /**
    * @brief Calculates the range of the elements in a container.
    *
    * @tparam Container The type of the container.
    * @tparam T The type of the elements in the container.
    * @param a The container whose range is to be calculated.
    * @return The range of the elements.
    */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto range( const Container& a ) noexcept
    {
        T max{std::numeric_limits<T>::min()};
        T min{std::numeric_limits<T>::max()};
        std::ranges::for_each( a,
                               [&min,&max]( const T& v )
                               {
                                   min = std::min( min, v );
                                   max = std::max( max, v );
                               } );
        return max - min;
    }

    /**
     * @brief Calculates the interquartile range (IQR) of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose interquartile range is to be calculated.
     * @return The interquartile range (IQR).
     */
    export template<typename Container, typename = typename Container::value_type>
    constexpr auto interquartileRange( Container a ) noexcept
    {
        return quartilesQ3( a ) - quartilesQ1( a );
    }

    /**
     * @brief Calculates the variance of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose variance is to be calculated.
     * @return The variance of the elements.
     */
    template<typename Container, typename T = typename Container::value_type>
    constexpr auto variance( const Container& a ) noexcept -> double
    {
        if ( std::size( a ) <= 1 )
        {
            return static_cast<T>(0);
        }

        const T m{mean( a )};
        const auto func{[ &m ]( T accumulator, const T& val ) constexpr -> T
                {
                    return accumulator + ( val - m ) * ( val - m );
                }
        };

        return std::sqrt( std::accumulate( std::cbegin( a ), std::cend( a ), T( 0 ), func ) );
    }

    /**
     * @brief Calculates the population variance of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose population variance is to be calculated.
     * @return The population variance of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto variancePopulation( const Container& a ) noexcept -> T
    {
        return static_cast<T>(variance( a ) / std::size( a ));
    }

    /**
     * @brief Calculates the sample variance of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose sample variance is to be calculated.
     * @return The sample variance of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto varianceSample( const Container& a ) noexcept -> T
    {
        return static_cast<T>(variance( a ) / ( std::size( a ) - 1 ));
    }

    /**
     * @brief Calculates the population standard deviation of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose population standard deviation is to be calculated.
     * @return The population standard deviation of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto standardDeviationPopulation( const Container& a ) noexcept -> T
    {
        return static_cast<T>(std::sqrt( variancePopulation( a ) ));
    }

    /**
     * @brief Calculates the sample standard deviation of the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose sample standard deviation is to be calculated.
     * @return The sample standard deviation of the elements.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto standardDeviationSample( const Container& a ) noexcept -> T
    {
        return static_cast<T>(std::sqrt( varianceSample( a ) ));
    }
    #pragma endregion
    #pragma region normal distribution
    /**
     * @brief Calculates the normal probability density for a value.
     *
     * @tparam T The type of the value.
     * @param a The value.
     * @param m The mean.
     * @param v The variance.
     * @param s The standard deviation.
     * @return The normal probability density.
     */
    export template<typename T>
    constexpr auto normalProbabilityDensity( const T& a, const T& m, const T& v, const T& s )
    {
        return 1 / ( s * std::sqrt( FawnAlgebra::two_pi<double> ) ) * std::exp( -( ( a - m ) * ( a - m ) ) / ( 2 * v ) );
    }

    /**
     * @brief Calculates the normal probability density for the elements in a container.
     *
     * @tparam Container The type of the container.
     * @tparam T The type of the elements in the container.
     * @param a The container whose normal probability density is to be calculated.
     * @return A vector of pairs where each pair contains an element and its normal probability density.
     */
    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto normalProbabilityDensity( const Container& a )
    {
        const T m{mean( a )};
        const T v{variancePopulation( a )};
        const T s{standardDeviationPopulation( a )};

        std::vector<std::pair<T, double>> p( std::size( a ) );// an std::vector because size is not always know on compile time.
        for ( std::size_t i{}; i < std::size( a ); ++i )
        {
            p[ i ] = std::make_pair( a[ i ], normalProbabilityDensity( a[ i ], m, v, s ) );
        }
        return p;
    }

    /**
     * @brief Calculates the z-score for a value.
     *
     * @tparam T The type of the value.
     * @param a The value.
     * @param m The mean.
     * @param s The standard deviation.
     * @return The z-score.
     */
    export template<typename T>
    constexpr auto zScore( const T& a, const T& m, const T& s )
    {
        return ( a - m ) / s;
    }

    /**
     * @brief Calculates the Poisson distribution for a given number of occurrences and rate.
     *
     * @tparam T The type of the number of occurrences.
     * @tparam C The type of the rate.
     * @param k The number of times something occurred.
     * @param l The rate of occurrence.
     * @return The Poisson distribution.
     */
    export template<typename T = int, typename C = int>
    constexpr auto poissonDistribution( const T& k, const C& l )
    {
        uint32 factorial{1};
        for ( T i{2}; i <= k; ++i )
        {
            factorial *= i;
        }
        return std::exp( -l ) * std::pow( l, k ) / factorial;
    }

    export template<typename Container, typename T = typename Container::value_type>
    constexpr auto chiSquareTest( const Container& O, const Container& E )
    {
        // static_assert( !std::is_empty( O ), "Chi-Square requires both containers to be the same size." );
        // static_assert( !std::is_empty( E ), "Chi-Square requires both containers to be the same size." );
        // static_assert( std::size( O ) == std::size( E ), "Chi-Square requires both containers to be the same size." );

        double sum{};
        for ( std::size_t i{}; i < std::size( O ); ++i )
        {
            T oe{O[ i ] - E[ i ]};
            if ( E[ i ] != 0 )
            {
                sum += static_cast<double>(oe * oe) / static_cast<double>(E[ i ]);
            }
        }
        return static_cast<T>(sum);
    }

    // todo : chiSquareTable
    // todo : tDistribution
    // todo : tTable
    #pragma endregion
    #pragma region statistical tests
    // todo : skewness
    // todo : kurtosis
    // todo : correlation
    // todo : regression
    // todo : t test
    // todo : ANOVAs
    // todo : Chi-Quare
    // todo : Effect size
    // todo : Model 
    #pragma endregion
}// namespace FawnAlgebra
