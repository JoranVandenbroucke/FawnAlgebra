//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

export module FawnAlgebra:Bezier;
import :Arithmetics;

namespace FawnAlgebra
{
export template<typename T, std::size_t N, std::size_t S>
class StaticQuadraticBezier
{
public:
    constexpr StaticQuadraticBezier() = default;

    explicit constexpr StaticQuadraticBezier( std::array<Vec<T, N>, S> pts ) : points { std::move( pts ) }
    {
    }

    constexpr Vec<T, N>& operator[]( std::size_t idx )
    {
        return points[ idx ];
    }

    constexpr const Vec<T, N>& operator[]( std::size_t idx ) const
    {
        return points[ idx ];
    }

    constexpr auto* data() noexcept
    {
        return points.data();
    }

    constexpr const auto* data() const noexcept
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

private:
    std::array<Vec<T, N>, S> points {};
};
export template<typename T, std::size_t N>
class DynamicQuadraticBezier
{
public:
    constexpr DynamicQuadraticBezier() = default;

    explicit constexpr DynamicQuadraticBezier( std::vector<Vec<T, N>> pts ) : points { std::move( pts ) }
    {
    }

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

    constexpr auto back() noexcept -> Vec<T, N>&
    {
        return points.back();
    }

    constexpr auto back() const noexcept -> const Vec<T, N>&
    {
        return points.back();
    }

    constexpr auto push_back( const Vec<T, N>& val ) noexcept -> const Vec<T, N>&
    {
        return points.emplace_back( val );
    }

    constexpr auto emplace_back( const Vec<T, N>& val ) noexcept -> const Vec<T, N>&
    {
        return points.emplace_back( val );
    }

    constexpr void pop_back() noexcept
    {
        points.pop_back();
    }

private:
    std::vector<Vec<T, N>> points {};
};

export template<typename T, std::size_t N, std::size_t S, typename V = T>
    requires std::is_floating_point_v<V>
constexpr auto Evaluate( const StaticQuadraticBezier<T, N, S>& bezier, const V& t ) -> Vec<T, N>
{
    Vec<T, N> result {};
    const std::size_t numSegments { bezier.size() / 2ULL };
    const std::size_t from { static_cast<std::size_t>( std::floor( t ) ) % numSegments };
    if ( from == t )
    {
        return bezier[ from * 2 ];
    }

    const Vec<T, N>& p0 { bezier[ from * 2 ] };
    const Vec<T, N>& p1 { bezier[ from * 2 + 1 ] };
    const Vec<T, N>& p2 { bezier[ ( from * 2 + 2 ) % bezier.size() ] };

    const T u { 1 - t };
    result += p0 * ( u * u ) + p1 * ( 2 * u * t ) + p2 * ( t * t );

    return result;
}

export template<typename T, std::size_t N, typename V>
    requires std::is_floating_point_v<V>
constexpr auto Evaluate( const DynamicQuadraticBezier<T, N>& bezier, const V& t ) -> Vec<T, N>
{
    Vec<T, N> result {};
    const std::size_t numSegments { bezier.size() / 2ULL };
    const std::size_t from { static_cast<std::size_t>( std::floor( t ) ) % numSegments };
    if ( from == t )
    {
        return bezier[ from * 2 ];
    }

    const Vec<T, N>& p0 { bezier[ from * 2 ] };
    const Vec<T, N>& p1 { bezier[ from * 2 + 1 ] };
    const Vec<T, N>& p2 { bezier[ ( from * 2 + 2 ) % bezier.size() ] };

    const T u { 1 - t };
    result += p0 * ( u * u ) + p1 * ( 2 * u * t ) + p2 * ( t * t );

    return result;
}

export template<typename T, std::size_t N, std::size_t S>
constexpr auto IsClosed( const StaticQuadraticBezier<T, N, S>& ) -> bool
{
    return ( S & 1ULL ) == 0ULL;
}

export template<typename T, std::size_t N>
constexpr auto IsClosed( const DynamicQuadraticBezier<T, N>& bezier ) -> bool
{
    return ( bezier.size() & 1ULL ) == 0ULL;
}

export template<typename T, std::size_t N>
constexpr void Close( DynamicQuadraticBezier<T, N>& bezier )
{
    // if points are odd and not empty
    if ( ( bezier.size() & 1 ) && !bezier.empty() )
    {
        const Vec<T, N>& lastPoint { bezier.back() };
        const Vec<T, N>& controlPoint { ( lastPoint + bezier[ 0 ] ) / 2 };
        bezier.emplace_back( controlPoint );
    }
}

export template<typename T, std::size_t N>
constexpr void Open( DynamicQuadraticBezier<T, N>& bezier )
{
    // if points are even and not empty
    if ( !( bezier.size() & 1ULL ) && !bezier.empty() )
    {
        bezier.pop_back();
    }
}

export template<typename T, std::size_t N>
constexpr void AddPoint( DynamicQuadraticBezier<T, N>& bezier, const Vec<T, N>& newPoint )
{
    // if points are odd
    if ( bezier.size() & 1ULL )
    {
        const Vec<T, N> lastPoint { bezier.back() };
        const Vec<T, N> controlPoint { ( lastPoint + newPoint ) / Vec<T, N> { 2 } };
        bezier.emplace_back( controlPoint );
        bezier.emplace_back( newPoint );
    }
    else
    {
        bezier.emplace_back( newPoint );
    }
}

export template<typename T, std::size_t N>
constexpr void AddControlPoint( DynamicQuadraticBezier<T, N>& bezier, const Vec<T, N>& newPoint )
{
    if ( bezier.empty() )
    {
        return;
    }
    // if points are odd
    if ( bezier.size() & 1ULL )
    {
        bezier.emplace_back( newPoint );
    }
    else
    {
        const Vec<T, N>& lastPoint { bezier.back() };
        const Vec<T, N> controlPoint { ( lastPoint + newPoint ) / Vec<T, N> { 2 } };
        bezier.emplace_back( controlPoint );
        bezier.emplace_back( newPoint );
    }
}

export template<typename T, std::size_t N>
constexpr void RemovePoint( DynamicQuadraticBezier<T, N>& bezier )
{
    if ( bezier.empty() )
    {
        return;
    }
    if ( bezier.size() > 1 )
    {
        bezier.pop_back();// Remove the control point before the last point
    }
    bezier.pop_back();// Remove the last point
}

export template<typename T, std::size_t N>
constexpr void AddPoints( DynamicQuadraticBezier<T, N>& bezier, const std::vector<Vec<T, N>>& newPoints )
{
    for ( std::size_t i {}; i < newPoints.size(); ++i )
    {
        bezier.push_back( newPoints[ i ] );
    }
}

export template<typename T, std::size_t N>
constexpr ui32 SegmentCount( const DynamicQuadraticBezier<T, N>& bezier )
{
    return static_cast<ui32>( bezier.size() > 2ULL ? ( bezier.size() - 1ULL ) / 2ULL : 0ULL );
}

export using dynamic_bezierI2 = DynamicQuadraticBezier<i32, 2>;
export using dynamic_bezierI3 = DynamicQuadraticBezier<i32, 3>;
export using dynamic_bezierI4 = DynamicQuadraticBezier<i32, 4>;

export using dynamic_bezierUI2 = DynamicQuadraticBezier<ui32, 2>;
export using dynamic_bezierUI3 = DynamicQuadraticBezier<ui32, 3>;
export using dynamic_bezierUI4 = DynamicQuadraticBezier<ui32, 4>;

export using dynamic_bezierF2 = DynamicQuadraticBezier<f32, 2>;
export using dynamic_bezierF3 = DynamicQuadraticBezier<f32, 3>;
export using dynamic_bezierF4 = DynamicQuadraticBezier<f32, 4>;

export using dynamic_bezierD2 = DynamicQuadraticBezier<f64, 2>;
export using dynamic_bezierD3 = DynamicQuadraticBezier<f64, 3>;
export using dynamic_bezierD4 = DynamicQuadraticBezier<f64, 4>;

export using static_bezierI2 = DynamicQuadraticBezier<i32, 2>;
export using static_bezierI3 = DynamicQuadraticBezier<i32, 3>;
export using static_bezierI4 = DynamicQuadraticBezier<i32, 4>;

export using static_bezierUI2 = DynamicQuadraticBezier<ui32, 2>;
export using static_bezierUI3 = DynamicQuadraticBezier<ui32, 3>;
export using static_bezierUI4 = DynamicQuadraticBezier<ui32, 4>;

export using static_bezierF2 = DynamicQuadraticBezier<f32, 2>;
export using static_bezierF3 = DynamicQuadraticBezier<f32, 3>;
export using static_bezierF4 = DynamicQuadraticBezier<f32, 4>;

export using static_bezierD2 = DynamicQuadraticBezier<f64, 2>;
export using static_bezierD3 = DynamicQuadraticBezier<f64, 3>;
export using static_bezierD4 = DynamicQuadraticBezier<f64, 4>;
}// namespace FawnAlgebra
