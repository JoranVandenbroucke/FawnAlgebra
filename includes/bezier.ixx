//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include <cmath>
#include <cstddef>
#include <vector>

export module FawnAlgebra.Bezier;
import FawnAlgebra.Arithmetics;

namespace FawnAlgebra
{
    export template<typename T, std::size_t N>
    struct QuadraticBezier
    {
        constexpr QuadraticBezier() = default;

        explicit constexpr QuadraticBezier( std::vector<Vec<T, N>> pts ) : points( std::move( pts ) )
        {
        }

        std::vector<Vec<T, N>> points;

        constexpr Vec<T, N>& operator[]( std::size_t idx )
        {
            return points[ idx ];
        }

        constexpr const Vec<T, N>& operator[]( std::size_t idx ) const
        {
            return points[ idx ];
        }

        constexpr auto data() noexcept
        {
            return points.data();
        }

        constexpr auto data() const noexcept
        {
            return points.data();
        }

        constexpr auto begin() noexcept
        {
            return points.begin();
        }

        constexpr auto begin() const noexcept
        {
            return points.begin();
        }

        constexpr auto cbegin() noexcept
        {
            return points.cbegin();
        }

        constexpr auto cbegin() const noexcept
        {
            return points.cbegin();
        }

        constexpr auto end() noexcept
        {
            return points.end();
        }

        constexpr auto end() const noexcept
        {
            return points.end();
        }

        constexpr auto cend() noexcept
        {
            return points.cend();
        }

        constexpr auto cend() const noexcept
        {
            return points.cend();
        }

        constexpr auto rbegin() noexcept
        {
            return points.rbegin();
        }

        constexpr auto rbegin() const noexcept
        {
            return points.rbegin();
        }

        constexpr auto crbegin() noexcept
        {
            return points.crbegin();
        }

        constexpr auto crbegin() const noexcept
        {
            return points.crbegin();
        }

        constexpr auto rend() noexcept
        {
            return points.rend();
        }

        constexpr auto rend() const noexcept
        {
            return points.rend();
        }

        constexpr auto crend() noexcept
        {
            return points.crend();
        }

        constexpr auto crend() const noexcept
        {
            return points.crend();
        }

        constexpr auto size() const noexcept
        {
            return points.size();
        }

        constexpr auto empty() const noexcept
        {
            return points.empty();
        }

        constexpr void clear() noexcept
        {
            return points.clear();
        }
    };

    export template<typename T, std::size_t N, typename V>
        requires std::is_floating_point_v<V>
    constexpr Vec<T, N> evaluate( const QuadraticBezier<T, N>& bezier, const V& t )
    {
        Vec<T, N> result {};
        const std::size_t numSegments { bezier.size() / 2ULL };
        const std::size_t from { static_cast<std::size_t>( std::floor( t ) ) % numSegments };
        if ( from == t )
        {
            return bezier.points[ from * 2 ];
        }

        const Vec<T, N>& p0 { bezier.points[ from * 2 ] };
        const Vec<T, N>& p1 { bezier.points[ from * 2 + 1 ] };
        const Vec<T, N>& p2 { bezier.points[ ( from * 2 + 2 ) % bezier.size() ] };

        const T u { 1 - t };
        result += p0 * ( u * u ) + p1 * ( 2 * u * t ) + p2 * ( t * t );

        return result;
    }

    export template<typename T, std::size_t N>
    constexpr bool isClosed( const QuadraticBezier<T, N>& bezier )
    {
        return ( bezier.size() & 1ULL ) == 0ULL;
    }

    export template<typename T, std::size_t N>
    constexpr void close( QuadraticBezier<T, N>& bezier )
    {
        if ( !( bezier.size() & 1 ) && !bezier.empty() )
        {
            const Vec<T, N>& lastPoint { bezier.points.back() };
            const Vec<T, N>& controlPoint { ( lastPoint + bezier.points[ 0 ] ) / 2 };
            bezier.points.emplace_back( controlPoint );
        }
    }

    export template<typename T, std::size_t N>
    constexpr void open( QuadraticBezier<T, N>& bezier )
    {
        if ( bezier.size() & 1 && !bezier.empty() )
        {
            bezier.points.pop_back();
        }
    }

    export template<typename T, std::size_t N>
    constexpr void addPoint( QuadraticBezier<T, N>& bezier, const Vec<T, N>& newPoint )
    {
        if ( bezier.points.size() & 1ULL )
        {
            const Vec<T, N> lastPoint { bezier.points.back() };
            const Vec<T, N> controlPoint { ( lastPoint + newPoint ) / 2 };
            bezier.points.emplace_back( controlPoint );
            bezier.points.emplace_back( newPoint );
        }
        else
        {
            bezier.points.emplace_back( newPoint );
        }
    }

    export template<typename T, std::size_t N>
    constexpr void addControlPoint( QuadraticBezier<T, N>& bezier, const Vec<T, N>& newPoint )
    {
        if ( bezier.points.empty() )
        {
            return;
        }
        if ( bezier.points.size() & 1ULL )
        {
            bezier.points.emplace_back( newPoint );
        }
        else
        {
            const Vec<T, N>& lastPoint { bezier.points.back() };
            const Vec<T, N>& controlPoint { ( lastPoint + newPoint ) / 2 };
            bezier.points.emplace_back( controlPoint );
            bezier.points.emplace_back( newPoint );
        }
    }

    export template<typename T, std::size_t N>
    constexpr void removePoint( QuadraticBezier<T, N>& bezier )
    {
        if ( bezier.points.empty() )
        {
            return;
        }
        if ( bezier.size() & 1ULL )
        {
            bezier.points.pop_back();
        }
        else
        {
            bezier.points.pop_back();// Remove the control point before the last point
            bezier.points.pop_back();// Remove the last point
        }
    }

    export template<typename T, std::size_t N>
    constexpr void addPoints( QuadraticBezier<T, N>& bezier, const std::vector<Vec<T, N>>& newPoints )
    {
        for ( std::size_t i {}; i < newPoints.size(); ++i )
        {
            bezier.points.push_back( newPoints[ i ] );
        }
    }

    export template<typename T, std::size_t N>
    constexpr uint32 segmentCount( const QuadraticBezier<T, N>& bezier )
    {
        return static_cast<uint32>( bezier.size() > 2ULL ? ( bezier.size() - 1ULL ) / 2ULL : 0ULL );
    }

    export using bezierI2 = QuadraticBezier<int32, 2>;
    export using bezierI3 = QuadraticBezier<int32, 3>;
    export using bezierI4 = QuadraticBezier<int32, 4>;

    export using bezierUI2 = QuadraticBezier<uint32, 2>;
    export using bezierUI3 = QuadraticBezier<uint32, 3>;
    export using bezierUI4 = QuadraticBezier<uint32, 4>;

    export using bezierF2 = QuadraticBezier<float32, 2>;
    export using bezierF3 = QuadraticBezier<float32, 3>;
    export using bezierF4 = QuadraticBezier<float32, 4>;

    export using bezierD2 = QuadraticBezier<float64, 2>;
    export using bezierD3 = QuadraticBezier<float64, 3>;
    export using bezierD4 = QuadraticBezier<float64, 4>;
}// namespace FawnAlgebra
