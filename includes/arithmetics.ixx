//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ostream>

export module FawnAlgebra.Arithmetics;

namespace FawnAlgebra
{
export template<typename T, std::size_t N>
struct Vec
{
private:
    std::array<T, N> data;

public:
    // Default constructor
    Vec()
        : data{}
    {
    }

    // Constructor to initialize all elements
    explicit constexpr Vec( const T& val ) noexcept
        : data{}
    {
        data.fill( val );
    }

    constexpr Vec( std::initializer_list<T> args ) noexcept
        : data{}
    {
        // static_assert(args.size() == N, "argument list is incorrect size");
        std::ranges::copy( args, data.begin() );
    }

    constexpr Vec( const Vec& other )
        : data{}
    {
        std::ranges::copy( other.data, data.begin() );
    }

    constexpr Vec( Vec&& other ) noexcept
        : data( std::move( other.data ) )
    {
    }

    constexpr auto operator=( const Vec& other ) -> Vec&
    {
        if ( this == &other )
        {
            return *this;
        }
        std::copy( other.data.cbegin(), other.data.cend(), data.begin() );
        return *this;
    }

    constexpr auto operator=( Vec&& other ) noexcept -> Vec&
    {
        if ( this == &other )
        {
            return *this;
        }
        return *this;
        data = std::move( other.data );
        return *this;
    }

    explicit constexpr operator std::array<T, N>() const noexcept
    {
        return data;
    }

    template<std::size_t M>
        requires( M < N )
    explicit constexpr Vec( const Vec<T, M>& other )
    {
        std::size_t i = 0;
        for ( ; i < M; ++i )
        {
            data[ i ] = other.data[ i ];// Copy existing elements
        }
        for ( ; i < N; ++i )
        {
            data[ i ] = 0;// Fill remaining elements with zero
        }
    }

    // Indexing operator
    [[nodiscard]] constexpr T& operator[]( const std::size_t idx )
    {
        return data[ idx ];
    }

    [[nodiscard]] constexpr const T& operator[]( const std::size_t idx ) const
    {
        return data[ idx ];
    }

    [[nodiscard]] constexpr const std::array<T, N>& get() const noexcept
    {
        return data;
    }

    #pragma region 2D Swizzle
    template<std::size_t M = N>
        requires( M >= 1 )
    constexpr T& x() noexcept
    {
        return data[ 0 ];
    }

    template<std::size_t M = N>
        requires( M >= 1 )
    constexpr const T& x() const noexcept
    {
        return data[ 0 ];
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr T& y() noexcept
    {
        return data[ 1 ];
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr const T& y() const noexcept
    {
        return data[ 1 ];
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 2> xx() const noexcept
    {
        return Vec<T, 2>{data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 2> xy() const noexcept
    {
        return Vec<T, 2>{data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 2> yy() const noexcept
    {
        return Vec<T, 2>{data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 2> yx() const noexcept
    {
        return Vec<T, 2>{data[ 1 ], data[ 0 ]};
    }

    #pragma endregion 2D Swizzle
    #pragma region 3D Swizzle
    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr T& z() noexcept
    {
        return data[ 2 ];
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr const T& z() const noexcept
    {
        return data[ 2 ];
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 2> xz() const noexcept
    {
        return {data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 2> zx() const noexcept
    {
        return {data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 2> yz() const noexcept
    {
        return {data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 2> zy() const noexcept
    {
        return {data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xxx() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xxy() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xyx() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xyy() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yxx() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yxy() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yyx() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yyy() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yyz() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yzy() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yzz() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zyy() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zyz() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zzy() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zzz() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zzx() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zxz() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zxx() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xzz() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xzx() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xxz() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xyz() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> xzy() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yxz() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> yzx() const noexcept
    {
        return Vec<T, 3>{data[ 1 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zxy() const noexcept
    {
        return Vec<T, 3>{data[ 2 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 3> zyx() const noexcept
    {
        return Vec<T, 3>{data[ 0 ], data[ 1 ], data[ 0 ]};
    }
    #pragma endregion 3D Swizzle
    #pragma region 4D Swizzle
    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr T& w() noexcept
    {
        return data[ 3 ];
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr const T& w() const noexcept
    {
        return data[ 3 ];
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 2> ww() const noexcept
    {
        return {data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 2> wx() const noexcept
    {
        return {data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 2> wy() const noexcept
    {
        return {data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 2> wz() const noexcept
    {
        return {data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> www() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wwx() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wwy() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wwz() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wxw() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wxx() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wxy() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wxz() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wyw() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wyx() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wyy() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wyz() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wzw() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wzx() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wzy() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> wzz() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> xww() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> xwx() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> xwy() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> xwz() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> xxw() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> xyw() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> xzw() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> yww() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> ywx() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> ywy() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> ywz() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> yxw() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> yyw() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> yzw() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> zww() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> zwx() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> zwy() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> zwz() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> zxw() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> zyw() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 3> zzw() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 2> xw() const noexcept
    {
        return {data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwww() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwwx() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwwy() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwwz() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwxw() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwxx() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwxy() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwxz() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwyw() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwyx() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwyy() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwyz() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwzw() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwzx() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwzy() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wwzz() const noexcept
    {
        return {data[ 3 ], data[ 3 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxww() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxwx() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxwy() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxwz() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxxw() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxxx() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxxy() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxxz() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxyw() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxyx() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxyy() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxyz() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxzw() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxzx() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxzy() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wxzz() const noexcept
    {
        return {data[ 3 ], data[ 0 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyww() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wywx() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wywy() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wywz() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyxw() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyxx() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyxy() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyxz() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyyw() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyyx() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyyy() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyyz() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyzw() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyzx() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyzy() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wyzz() const noexcept
    {
        return {data[ 3 ], data[ 1 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzww() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzwx() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzwy() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzwz() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzxw() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzxx() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzxy() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzxz() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzyw() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzyx() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzyy() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzyz() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzzw() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzzx() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzzy() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> wzzz() const noexcept
    {
        return {data[ 3 ], data[ 2 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwww() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwwx() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwwy() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwwz() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwxw() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwxx() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwxy() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwxz() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwyw() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwyx() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwyy() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwyz() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwzw() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwzx() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwzy() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xwzz() const noexcept
    {
        return {data[ 0 ], data[ 3 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xxww() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xxwx() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xxwy() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xxwz() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xxxw() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 1 )
    constexpr Vec<T, 4> xxxx() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> xxxy() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xxxz() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xxyw() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> xxyx() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> xxyy() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xxyz() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xxzw() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xxzx() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xxzy() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xxzz() const noexcept
    {
        return {data[ 0 ], data[ 0 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xyww() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xywx() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xywy() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xywz() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xyxw() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> xyxx() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> xyxy() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xyxz() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xyyw() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> xyyx() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> xyyy() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xyyz() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xyzw() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xyzx() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xyzy() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xyzz() const noexcept
    {
        return {data[ 0 ], data[ 1 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xzww() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xzwx() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xzwy() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xzwz() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xzxw() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzxx() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzxy() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzxz() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xzyw() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzyx() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzyy() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzyz() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> xzzw() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzzx() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzzy() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> xzzz() const noexcept
    {
        return {data[ 0 ], data[ 2 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywww() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywwx() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywwy() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywwz() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywxw() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywxx() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywxy() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywxz() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywyw() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywyx() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywyy() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywyz() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywzw() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywzx() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywzy() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> ywzz() const noexcept
    {
        return {data[ 1 ], data[ 3 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yxww() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yxwx() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yxwy() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yxwz() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yxxw() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yxxx() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yxxy() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yxxz() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yxyw() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yxyx() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yxyy() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yxyz() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yxzw() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yxzx() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yxzy() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yxzz() const noexcept
    {
        return {data[ 1 ], data[ 0 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yyww() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yywx() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yywy() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yywz() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yyxw() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yyxx() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yyxy() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yyxz() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yyyw() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yyyx() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 2 )
    constexpr Vec<T, 4> yyyy() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yyyz() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yyzw() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yyzx() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yyzy() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yyzz() const noexcept
    {
        return {data[ 1 ], data[ 1 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yzww() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yzwx() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yzwy() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yzwz() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yzxw() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzxx() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzxy() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzxz() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yzyw() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzyx() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzyy() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzyz() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> yzzw() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzzx() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzzy() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> yzzz() const noexcept
    {
        return {data[ 1 ], data[ 2 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwww() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwwx() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwwy() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwwz() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwxw() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwxx() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwxy() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwxz() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwyw() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwyx() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwyy() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwyz() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwzw() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwzx() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwzy() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zwzz() const noexcept
    {
        return {data[ 2 ], data[ 3 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zxww() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zxwx() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zxwy() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zxwz() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zxxw() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxxx() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxxy() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxxz() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zxyw() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxyx() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxyy() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxyz() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zxzw() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxzx() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxzy() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zxzz() const noexcept
    {
        return {data[ 2 ], data[ 0 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zyww() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zywx() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zywy() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zywz() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zyxw() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyxx() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyxy() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyxz() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zyyw() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyyx() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyyy() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyyz() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zyzw() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyzx() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyzy() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zyzz() const noexcept
    {
        return {data[ 2 ], data[ 1 ], data[ 2 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zzww() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 3 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zzwx() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 3 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zzwy() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 3 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zzwz() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 3 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zzxw() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 0 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzxx() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 0 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzxy() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 0 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzxz() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 0 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zzyw() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 1 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzyx() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 1 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzyy() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 1 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzyz() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 1 ], data[ 2 ]};
    }

    template<std::size_t M = N>
        requires( M >= 4 )
    constexpr Vec<T, 4> zzzw() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 2 ], data[ 3 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzzx() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 2 ], data[ 0 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzzy() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 2 ], data[ 1 ]};
    }

    template<std::size_t M = N>
        requires( M >= 3 )
    constexpr Vec<T, 4> zzzz() const noexcept
    {
        return {data[ 2 ], data[ 2 ], data[ 2 ], data[ 2 ]};
    }
    #pragma endregion 4D Swizzle
    #pragma region Operators
    template<typename TT, std::size_t NN>
    constexpr bool operator==( const Vec<TT, NN>& other ) const noexcept
    {
        if constexpr ( N != NN )
        {
            return true;
        }
        for ( std::size_t i{}; i < N; ++i )
        {
            if ( this->data[ i ] != other.data[ i ] )
            {
                return false;
            }
        }
        return true;
    }

    template<typename TT, std::size_t NN>
    constexpr bool operator!=( const Vec<TT, NN>& other ) const noexcept
    {
        return !( *this == other );
    }

    constexpr Vec operator+=( const Vec& other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] += other.data[ i ];
        }
        return *this;
    }

    constexpr Vec operator-=( const Vec& other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] -= other.data[ i ];
        }
        return *this;
    }

    constexpr Vec operator*=( const Vec& other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] *= other.data[ i ];
        }
        return *this;
    }

    constexpr Vec operator/=( const Vec& other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] /= other.data[ i ];
        }
        return *this;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    constexpr Vec operator+=( const TT other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] += static_cast<T>(other);
        }
        return *this;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    constexpr Vec operator-=( const TT other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] -= static_cast<T>(other);
        }
        return *this;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    constexpr Vec operator*=( const TT other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] *= static_cast<T>(other);
        }
        return *this;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    constexpr Vec operator/=( const TT other ) noexcept
    {
        for ( std::size_t i{}; i < N; ++i )
        {
            data[ i ] /= static_cast<T>(other);
        }
        return *this;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator+( Vec lhs, const TT rhs ) noexcept
    {
        return lhs += rhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator-( Vec lhs, const TT& rhs ) noexcept
    {
        return lhs -= rhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator*( Vec lhs, const TT& rhs ) noexcept
    {
        return lhs *= rhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator/( Vec lhs, const TT& rhs ) noexcept
    {
        return lhs /= rhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator+( const TT lhs, const Vec& rhs ) noexcept
    {
        return rhs + lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator-( const TT lhs, const Vec& rhs ) noexcept
    {
        return rhs - lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator*( const TT lhs, const Vec& rhs ) noexcept
    {
        return rhs * lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Vec operator/( const TT lhs, const Vec& rhs ) noexcept
    {
        return rhs / lhs;
    }

    friend constexpr Vec operator+( Vec lhs, const Vec& rhs ) noexcept
    {
        return lhs += rhs;
    }

    friend constexpr Vec operator-( Vec lhs, const Vec& rhs ) noexcept
    {
        return lhs -= rhs;
    }

    friend constexpr Vec operator*( Vec lhs, const Vec& rhs ) noexcept
    {
        return lhs *= rhs;
    }

    friend constexpr Vec operator/( Vec lhs, const Vec& rhs ) noexcept
    {
        return lhs /= rhs;
    }

    friend constexpr T operator|( Vec lhs, const Vec& rhs ) noexcept
    {
        T result{};
        for ( std::size_t i{}; i < N; ++i )
        {
            result += lhs.data[ i ] * rhs.data[ i ];
        }
        return result;
    }

    friend constexpr std::ostream& operator<<( std::ostream& os, const Vec& obj )
    {
        for ( std::size_t i{}; i < N - 1; ++i )
        {
            os << obj.data[ i ] << ", ";
        }
        os << obj.data[ N - 1 ];
        return os;
    }

    template<std::size_t M = N>
        requires( M == 2 )
    constexpr T operator^=( const Vec& other )
    {
        return x() * other.y() - y() * other.x();
    }

    template<std::size_t M = N>
        requires( M == 3 )
    constexpr Vec operator^=( const Vec& other )
    {
        return {y() * other.z() - z() * other.y(), z() * other.x() - x() * other.z(), x() * other.y() - y() * other.x()};
    }

    template<std::size_t M = N>
        requires( M == 4 )
    constexpr Vec operator^=( const Vec& other )
    {
        return {y() * other.z() - z() * other.y(), z() * other.w() - w() * other.z(), w() * other.x() - x() * other.w(), x() * other.y() - y() * other.x()};
    }

    template<std::size_t M = N>
        requires( M == 2 )
    friend constexpr T operator^( Vec lhs, const Vec& rhs )
    {
        return lhs ^= rhs;
    }

    template<std::size_t M = N>
        requires( M == 3 )
    friend constexpr Vec operator^( Vec lhs, const Vec& rhs )
    {
        return lhs ^= rhs;
    }

    template<std::size_t M = N>
        requires( M == 4 )
    friend constexpr Vec operator^( Vec lhs, const Vec& rhs )
    {
        return lhs ^= rhs;
    }
    #pragma endregion Operators
};

#define OVERLOAD_FUNC_VEC( func )                                                                                                                                                                                                                        \
    export template<typename T, std::size_t N>                                                                                                                                                                                                           \
    constexpr Vec<T, N> func( const Vec<T, N>& v )                                                                                                                                                                                                       \
    {                                                                                                                                                                                                                                                    \
        Vec<T, N> result;                                                                                                                                                                                                                                \
        for ( std::size_t i = 0; i < N; ++i )                                                                                                                                                                                                            \
        {                                                                                                                                                                                                                                                \
            result[ i ] = std::func( v[ i ] );                                                                                                                                                                                                      \
        }                                                                                                                                                                                                                                                \
        return result;                                                                                                                                                                                                                                   \
    }

#define OVERLOAD_FUNC_VEC_VEC( func )                                                                                                                                                                                                                    \
    export template<typename T, std::size_t N>                                                                                                                                                                                                           \
    constexpr Vec<T, N> func( const Vec<T, N>& v, const Vec<T, N>& w )                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
        Vec<T, N> result;                                                                                                                                                                                                                                \
        for ( std::size_t i {}; i < N; ++i )                                                                                                                                                                                                             \
        {                                                                                                                                                                                                                                                \
            result[ i ] = std::func( v[ i ], w[ i ] );                                                                                                                                                                                              \
        }                                                                                                                                                                                                                                                \
        return result;                                                                                                                                                                                                                                   \
    }

OVERLOAD_FUNC_VEC( abs )
OVERLOAD_FUNC_VEC( exp )
OVERLOAD_FUNC_VEC( exp2 )
OVERLOAD_FUNC_VEC( expm1 )
OVERLOAD_FUNC_VEC( log )
OVERLOAD_FUNC_VEC( log10 )
OVERLOAD_FUNC_VEC( log2 )
OVERLOAD_FUNC_VEC( log1p )
OVERLOAD_FUNC_VEC( sqrt )
OVERLOAD_FUNC_VEC( cbrt )
OVERLOAD_FUNC_VEC( sin )
OVERLOAD_FUNC_VEC( cos )
OVERLOAD_FUNC_VEC( tan )
OVERLOAD_FUNC_VEC( asin )
OVERLOAD_FUNC_VEC( acos )
OVERLOAD_FUNC_VEC( atan )
OVERLOAD_FUNC_VEC( sinh )
OVERLOAD_FUNC_VEC( cosh )
OVERLOAD_FUNC_VEC( tanh )
OVERLOAD_FUNC_VEC( asinh )
OVERLOAD_FUNC_VEC( acosh )
OVERLOAD_FUNC_VEC( atanh )
OVERLOAD_FUNC_VEC( erf )
OVERLOAD_FUNC_VEC( erfc )
OVERLOAD_FUNC_VEC( tgamma )
OVERLOAD_FUNC_VEC( lgamma )
OVERLOAD_FUNC_VEC( ceil )
OVERLOAD_FUNC_VEC( floor )
OVERLOAD_FUNC_VEC( trunc )
OVERLOAD_FUNC_VEC( round )
OVERLOAD_FUNC_VEC( lround )
OVERLOAD_FUNC_VEC( llround )
OVERLOAD_FUNC_VEC( nearbyint )
OVERLOAD_FUNC_VEC( rint )
OVERLOAD_FUNC_VEC( lrint )
OVERLOAD_FUNC_VEC( llrint )
OVERLOAD_FUNC_VEC( frexp )
OVERLOAD_FUNC_VEC( ldexp )
OVERLOAD_FUNC_VEC( modf )
OVERLOAD_FUNC_VEC( scalbn )
OVERLOAD_FUNC_VEC( scalbln )
OVERLOAD_FUNC_VEC( ilogb )
OVERLOAD_FUNC_VEC( logb )
OVERLOAD_FUNC_VEC( nextafter )
OVERLOAD_FUNC_VEC( nexttoward )
OVERLOAD_FUNC_VEC( copysign )

OVERLOAD_FUNC_VEC_VEC( min )
OVERLOAD_FUNC_VEC_VEC( max )
OVERLOAD_FUNC_VEC_VEC( fdim )
OVERLOAD_FUNC_VEC_VEC( fmod )
OVERLOAD_FUNC_VEC_VEC( remainder )
OVERLOAD_FUNC_VEC_VEC( hypot )

export template<typename T, std::size_t N>
constexpr Vec<T, N> pow( const Vec<T, N>& v, T exponent )
{
    Vec<T, N> result;
    for ( std::size_t i{}; i < N; ++i )
    {
        result.data[ i ] = std::pow( v.data[ i ], exponent );
    }
    return result;
}

export template<typename T, std::size_t N>
constexpr Vec<T, N> fma( const Vec<T, N>& v, const Vec<T, N>& y, const Vec<T, N>& z )
{
    Vec<T, N> result;
    for ( std::size_t i{}; i < N; ++i )
    {
        result.data[ i ] = std::fma( v.data[ i ], y.data[ i ], z.data[ i ] );
    }
    return result;
}

export template<typename T, std::size_t N>
constexpr Vec<T, N> nan( const Vec<T, N>& v )
{
    Vec<T, N> result;
    for ( std::size_t i{}; i < N; ++i )
    {
        result.data[ i ] = std::nan( v.data[ i ] );
    }
    return result;
}

export template<typename T, std::size_t N>
constexpr Vec<T, N> remquo( const Vec<T, N>& a, const Vec<T, N>& b, Vec<int, N>& quo )
{
    Vec<T, N> result;
    for ( std::size_t i{}; i < N; ++i )
    {
        result.data[ i ] = std::remquo( a.data[ i ], b.data[ i ], &quo.data[ i ] );
    }
    return result;
}

export template<typename T, std::size_t N>
constexpr Vec<T, N> Lerp( const Vec<T, N>& a, const Vec<T, N>& b, T t )
{
    Vec<T, N> result;
    for ( std::size_t i{}; i < N; ++i )
    {
        result.data[ i ] = std::lerp( a.data[ i ], b.data[ i ], t );
    }
    return result;
}

// Aliases for different types
export using int8 = int8_t;
export using int16 = int16_t;
export using int32 = int32_t;
export using int64 = int64_t;
export using uint8 = uint8_t;
export using uint16 = uint16_t;
export using uint32 = uint32_t;
export using uint = uint32_t;
export using uint64 = uint64_t;
export using float32 = float;
export using float64 = double;

export using char2 = Vec<int8, 2>;
export using char3 = Vec<int8, 3>;
export using char4 = Vec<int8, 4>;

export using uchar2 = Vec<uint8, 2>;
export using uchar3 = Vec<uint8, 3>;
export using uchar4 = Vec<uint8, 4>;

export using short2 = Vec<int16, 2>;
export using short3 = Vec<int16, 3>;
export using short4 = Vec<int16, 4>;

export using ushort2 = Vec<uint16, 2>;
export using ushort3 = Vec<uint16, 3>;
export using ushort4 = Vec<uint16, 4>;

export using int2 = Vec<int32, 2>;
export using int3 = Vec<int32, 3>;
export using int4 = Vec<int32, 4>;

export using uint2 = Vec<uint32, 2>;
export using uint3 = Vec<uint32, 3>;
export using uint4 = Vec<uint32, 4>;

export using float2 = Vec<float32, 2>;
export using float3 = Vec<float32, 3>;
export using float4 = Vec<float32, 4>;

export using double2 = Vec<float64, 2>;
export using double3 = Vec<float64, 3>;
export using double4 = Vec<float64, 4>;
}// namespace FawnAlgebra
