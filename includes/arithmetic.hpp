//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

#pragma once
#include <cmath>
#include <cstdint>

namespace FawnAlgebra
{
    using uint = uint32_t;

    struct alignas( 8 ) int2
    {
        int2() = default;

        constexpr int2( const int32_t a, const int32_t b )
            : x{a}
            , y{b}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        explicit constexpr int2( const T a )
            : x{static_cast<int>(a)}
            , y{static_cast<int>(a)}
        {
        }

        int32_t x{};
        int32_t y{};

        friend bool operator==( const int2& lhs, const int2& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y;
        }

        friend bool operator!=( const int2& lhs, const int2& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr int32_t& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range{"int2 operator[]; index >= 2"};
            }
        }

        constexpr const int32_t& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range{"int2 operator[]; index >= 2"};
            }
        }

        constexpr int2 operator-() const noexcept
        {
            return {-x, -y};
        }

        constexpr int2 operator+=( const int2& a ) noexcept
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr int2 operator-=( const int2& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr int2 operator*=( const int2& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr int2 operator/=( const int2& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr friend int2 operator+( int2 a, const int2& b )
        {
            return a += b;
        }

        constexpr friend int2 operator-( int2 a, const int2& b )
        {
            return a -= b;
        }

        constexpr friend int2 operator*( int2 a, const int2& b )
        {
            return a *= b;
        }

        constexpr friend int2 operator/( int2 a, const int2& b )
        {
            return a /= b;
        }

        constexpr int2& operator>>( const int a ) noexcept
        {
            x >>= a;
            y >>= a;
            return *this;
        }

        constexpr int2& operator<<( const int a ) noexcept
        {
            x <<= a;
            y <<= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int2 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int2 operator-=( T a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int2 operator*=( T a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int2 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0 / a};
                return *this * s;
            }
            x /= a;
            y /= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator+( int2 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator+( T b, int2 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator-( int2 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator-( T b, int2 a )
        {
            return {b - a.x, b - a.y};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator*( int2 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator*( T a, int2 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator/( int2 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int2 operator/( T a, const int2 b )
        {
            return int2{a} / b;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr int2& operator>>( const T a ) noexcept
        {
            x >>= a;
            y >>= a;
            return *this;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr int2& operator<<( const T a ) noexcept
        {
            x <<= a;
            y <<= a;
            return *this;
        }
    };

    struct alignas( 8 ) uint2
    {
        uint2() = default;

        constexpr uint2( const uint a, const uint b )
            : x{a}
            , y{b}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr explicit uint2( const T a )
            : x{static_cast<uint>(a)}
            , y{static_cast<uint>(a)}
        {
        }

        uint x{};
        uint y{};

        constexpr uint& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const uint& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const uint2& lhs, const uint2& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y;
        }

        friend bool operator!=( const uint2& lhs, const uint2& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr uint2 operator+=( const uint2& a ) noexcept
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr uint2 operator-=( const uint2& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr uint2 operator*=( const uint2& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr uint2 operator/=( const uint2& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr friend uint2 operator+( uint2 a, const uint2& b )
        {
            return a += b;
        }

        constexpr friend uint2 operator-( uint2 a, const uint2& b )
        {
            return a -= b;
        }

        constexpr friend uint2 operator*( uint2 a, const uint2& b )
        {
            return a *= b;
        }

        constexpr friend uint2 operator/( uint2 a, const uint2& b )
        {
            return a /= b;
        }

        constexpr uint2& operator>>( const uint2& a ) noexcept
        {
            x >>= a.x;
            y >>= a.x;
            return *this;
        }

        constexpr uint2& operator<<( const uint2& a ) noexcept
        {
            x <<= a.x;
            y <<= a.x;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint2 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint2 operator-=( T a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint2 operator*=( T a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint2 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0 / a};
                return *this * s;
            }
            x /= a;
            y /= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator+( uint2 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator+( T b, uint2 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator-( uint2 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator-( T b, uint2 a )
        {
            return {b - a.x, b - a.y};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator*( uint2 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator*( T a, uint2 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator/( uint2 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint2 operator/( T a, const uint2 b )
        {
            return uint2{a} / b;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr uint2& operator>>( const T a ) noexcept
        {
            x >>= a;
            y >>= a;
            return *this;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr uint2& operator<<( const T a ) noexcept
        {
            x <<= a;
            y <<= a;
            return *this;
        }
    };

    struct alignas( 8 ) float2
    {
        float2() = default;

        constexpr float2( const float a, const float b )
            : x{a}
            , y{b}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr explicit float2( const T a )
            : x{static_cast<float>(a)}
            , y{static_cast<float>(a)}
        {
        }

        float x{};
        float y{};

        constexpr float& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const float& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const float2& lhs, const float2& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y;
        }

        friend bool operator!=( const float2& lhs, const float2& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr float2 operator-() const noexcept
        {
            return {-x, -y};
        }

        constexpr float2 operator+=( const float2& a ) noexcept
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr float2 operator-=( const float2& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr float2 operator*=( const float2& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr float2 operator/=( const float2& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr friend float2 operator+( float2 a, const float2& b )
        {
            return a += b;
        }

        constexpr friend float2 operator-( float2 a, const float2& b )
        {
            return a -= b;
        }

        constexpr friend float2 operator*( float2 a, const float2& b )
        {
            return a *= b;
        }

        constexpr friend float2 operator/( float2 a, const float2& b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float2 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float2 operator-=( T a ) noexcept
        {
            x -= a;
            y -= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float2 operator*=( T a ) noexcept
        {
            x *= a;
            y *= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float2 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0f / a};
                return *this * s;
            }
            x /= static_cast<float>(a);
            y /= static_cast<float>(a);
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator+( float2 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator+( T b, float2 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator-( float2 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator-( T b, float2 a )
        {
            return {b - a.x, b - a.y};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator*( float2 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator*( T a, float2 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator/( float2 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float2 operator/( T a, const float2 b )
        {
            return float2{a} / b;
        }
    };

    struct int3;

    struct alignas( 16 ) int4
    {
        int4() = default;

        constexpr int4( const int32_t a, const int32_t b, const int32_t c, const int32_t d )
            : x{a}
            , y{b}
            , z{c}
            , w{d}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        explicit constexpr int4( const T a )
            : x{static_cast<int>(a)}
            , y{static_cast<int>(a)}
            , z{static_cast<int>(a)}
            , w{static_cast<int>(a)}
        {
        }

        int4( const int3& a, int d );

        int32_t x{};
        int32_t y{};
        int32_t z{};
        int32_t w{};

        constexpr int32_t& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                case 4: return w;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const int32_t& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                case 4: return w;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const int4& lhs, const int4& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y
                    && lhs.z == rhs.z
                    && lhs.w == rhs.w;
        }

        friend bool operator!=( const int4& lhs, const int4& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr int4 operator-() const noexcept
        {
            return {-x, -y, -z, -w};
        }

        constexpr int4 operator+=( const int4& a ) noexcept
        {
            x += a.x;
            y += a.y;
            z += a.z;
            w += a.w;
            return *this;
        }

        constexpr int4 operator-=( const int4& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            w -= a.w;
            return *this;
        }

        constexpr int4 operator*=( const int4& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            w *= a.w;
            return *this;
        }

        constexpr int4 operator/=( const int4& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            w /= a.w;
            return *this;
        }

        constexpr friend int4 operator+( int4 a, const int4& b )
        {
            return a += b;
        }

        constexpr friend int4 operator-( int4 a, const int4& b )
        {
            return a -= b;
        }

        constexpr friend int4 operator*( int4 a, const int4& b )
        {
            return a *= b;
        }

        constexpr friend int4 operator/( int4 a, const int4& b )
        {
            return a /= b;
        }

        constexpr int4& operator>>( const int a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            w >>= a;
            return *this;
        }

        constexpr int4& operator<<( const int a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            w <<= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int4 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            z += a;
            w += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int4 operator-=( T a ) noexcept
        {
            x -= a;
            y -= a;
            z -= a;
            w -= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int4 operator*=( T a ) noexcept
        {
            x *= a;
            y *= a;
            z *= a;
            w *= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int4 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0 / a};
                return *this * s;
            }
            x /= a;
            y /= a;
            z /= a;
            w /= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator+( int4 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator+( T b, int4 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator-( int4 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator-( T b, int4 a )
        {
            return {b - a.x, b - a.y, b - a.z, b - a.w};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator*( int4 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator*( T a, int4 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator/( int4 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int4 operator/( T a, const int4 b )
        {
            return int4{a} / b;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr int4& operator>>( const T a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            w >>= a;
            return *this;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr int4& operator<<( const T a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            w <<= a;
            return *this;
        }
    };

    struct alignas( 16 ) int3
    {
        int3() = default;

        constexpr int3( const int32_t a, const int32_t b, const int32_t c )
            : x{a}
            , y{b}
            , z{c}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        explicit constexpr int3( const int32_t a )
            : x{static_cast<int>(a)}
            , y{static_cast<int>(a)}
            , z{static_cast<int>(a)}
        {
        }

        explicit constexpr int3( const int4& a )
            : x{a.x}
            , y{a.y}
            , z{a.z}
        {
        }

        int32_t x{};
        int32_t y{};
        int32_t z{};
        int32_t dummy{};

        constexpr int32_t& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const int32_t& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const int3& lhs, const int3& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y
                    && lhs.z == rhs.z;
        }

        friend bool operator!=( const int3& lhs, const int3& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr int3 operator-() const noexcept
        {
            return {-x, -y, -z};
        }

        constexpr int3 operator+=( const int3& a ) noexcept
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }

        constexpr int3 operator-=( const int3& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }

        constexpr int3 operator*=( const int3& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }

        constexpr int3 operator/=( const int3& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            return *this;
        }

        constexpr friend int3 operator+( int3 a, const int3& b )
        {
            return a += b;
        }

        constexpr friend int3 operator-( int3 a, const int3& b )
        {
            return a -= b;
        }

        constexpr friend int3 operator*( int3 a, const int3& b )
        {
            return a *= b;
        }

        constexpr friend int3 operator/( int3 a, const int3& b )
        {
            return a /= b;
        }

        constexpr int3& operator>>( const int a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            return *this;
        }

        constexpr int3& operator<<( const int a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int3 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            z += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int3 operator-=( T a ) noexcept
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int3 operator*=( T a ) noexcept
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr int3 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0 / a};
                return *this * s;
            }
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator+( int3 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator+( T b, int3 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator-( int3 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator-( T b, int3 a )
        {
            return {b - a.x, b - a.y, b - a.z};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator*( int3 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator*( T a, int3 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator/( int3 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend int3 operator/( T a, const int3 b )
        {
            return int3{a} / b;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr int3& operator>>( const T a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            return *this;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr int3& operator<<( const T a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            return *this;
        }
    };

    struct uint3;

    struct alignas( 16 ) uint4
    {
        uint4() = default;

        constexpr uint4( const uint a, const uint b, const uint c, const uint d )
            : x{a}
            , y{b}
            , z{c}
            , w{d}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        explicit constexpr uint4( const T a )
            : x{static_cast<uint>(a)}
            , y{static_cast<uint>(a)}
            , z{static_cast<uint>(a)}
            , w{static_cast<uint>(a)}
        {
        }

        explicit uint4( const uint3& a, uint d );

        uint x{};
        uint y{};
        uint z{};
        uint w{};

        constexpr uint& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                case 4: return w;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const uint& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                case 4: return w;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const uint4& lhs, const uint4& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y
                    && lhs.z == rhs.z
                    && lhs.w == rhs.w;
        }

        friend bool operator!=( const uint4& lhs, const uint4& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr uint4 operator+=( const uint4& a ) noexcept
        {
            x += a.x;
            y += a.y;
            z += a.z;
            w += a.w;
            return *this;
        }

        constexpr uint4 operator-=( const uint4& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            w -= a.w;
            return *this;
        }

        constexpr uint4 operator*=( const uint4& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            w *= a.w;
            return *this;
        }

        constexpr uint4 operator/=( const uint4& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            w /= a.w;
            return *this;
        }

        constexpr friend uint4 operator+( uint4 a, const uint4& b )
        {
            return a += b;
        }

        constexpr friend uint4 operator-( uint4 a, const uint4& b )
        {
            return a -= b;
        }

        constexpr friend uint4 operator*( uint4 a, const uint4& b )
        {
            return a *= b;
        }

        constexpr friend uint4 operator/( uint4 a, const uint4& b )
        {
            return a /= b;
        }

        constexpr uint4& operator>>( const int a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            w >>= a;
            return *this;
        }

        constexpr uint4& operator<<( const int a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            w <<= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint4 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            z += a;
            w += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint4 operator-=( T a ) noexcept
        {
            x -= a;
            y -= a;
            z -= a;
            w -= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint4 operator*=( T a ) noexcept
        {
            x *= a;
            y *= a;
            z *= a;
            w *= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint4 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0 / a};
                return *this * s;
            }
            x /= a;
            y /= a;
            z /= a;
            w /= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator+( uint4 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator+( T b, uint4 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator-( uint4 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator-( T b, uint4 a )
        {
            return {b - a.x, b - a.y, b - a.z, b - a.w};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator*( uint4 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator*( T a, uint4 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator/( uint4 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint4 operator/( T a, const uint4 b )
        {
            return uint4{a} / b;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr uint4& operator>>( const T a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            w >>= a;
            return *this;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr uint4& operator<<( const T a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            w <<= a;
            return *this;
        }
    };

    struct alignas( 16 ) uint3
    {
        uint3() = default;

        constexpr uint3( const uint a, const uint b, const uint c )
            : x{a}
            , y{b}
            , z{c}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        explicit constexpr uint3( const T a )
            : x{static_cast<uint>(a)}
            , y{static_cast<uint>(a)}
            , z{static_cast<uint>(a)}
        {
        }

        explicit constexpr uint3( const uint4& a )
            : x{a.x}
            , y{a.y}
            , z{a.z}
        {
        }

        uint x{};
        uint y{};
        uint z{};
        uint dummy{};

        constexpr uint& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const uint& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const uint3& lhs, const uint3& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y
                    && lhs.z == rhs.z;
        }

        friend bool operator!=( const uint3& lhs, const uint3& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr uint3 operator+=( const uint3& a ) noexcept
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }

        constexpr uint3 operator-=( const uint3& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }

        constexpr uint3 operator*=( const uint3& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }

        constexpr uint3 operator/=( const uint3& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            return *this;
        }

        constexpr friend uint3 operator+( uint3 a, const uint3& b )
        {
            return a += b;
        }

        constexpr friend uint3 operator-( uint3 a, const uint3& b )
        {
            return a -= b;
        }

        constexpr friend uint3 operator*( uint3 a, const uint3& b )
        {
            return a *= b;
        }

        constexpr friend uint3 operator/( uint3 a, const uint3& b )
        {
            return a /= b;
        }

        constexpr uint3& operator>>( const int a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            return *this;
        }

        constexpr uint3& operator<<( const int a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint3 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            z += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint3 operator-=( T a ) noexcept
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint3 operator*=( T a ) noexcept
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr uint3 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0 / a};
                return *this * s;
            }
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator+( uint3 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator+( T b, uint3 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator-( uint3 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator-( T b, uint3 a )
        {
            return {b - a.x, b - a.y, b - a.z};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator*( uint3 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator*( T a, uint3 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator/( uint3 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend uint3 operator/( T a, const uint3 b )
        {
            return uint3{a} / b;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr uint3& operator>>( const T a ) noexcept
        {
            x >>= a;
            y >>= a;
            z >>= a;
            return *this;
        }

        template<typename T, typename = std::enable_if<std::is_integral_v<T>, T>>
        constexpr uint3& operator<<( const T a ) noexcept
        {
            x <<= a;
            y <<= a;
            z <<= a;
            return *this;
        }
    };

    struct float3;

    struct alignas( 16 ) float4
    {
        float4() = default;

        constexpr float4( const float a, const float b, const float c, const float d )
            : x{a}
            , y{b}
            , z{c}
            , w{d}
        {
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr explicit float4( const T a )
            : x{static_cast<float>(a)}
            , y{static_cast<float>(a)}
            , z{static_cast<float>(a)}
            , w{static_cast<float>(a)}
        {
        }

        explicit float4( const float3& a, float d );

        explicit float4( const float3& a );

        float x{};
        float y{};
        float z{};
        float w{};

        constexpr float& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                case 4: return w;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const float& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                case 4: return w;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const float4& lhs, const float4& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y
                    && lhs.z == rhs.z
                    && lhs.w == rhs.w;
        }

        friend bool operator!=( const float4& lhs, const float4& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr float4 operator-() const noexcept
        {
            return {-x, -y, -z, -w};
        }

        constexpr float4 operator+=( const float4& a ) noexcept
        {
            x += a.x;
            y += a.y;
            z += a.z;
            w += a.w;
            return *this;
        }

        constexpr float4 operator-=( const float4& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            w -= a.w;
            return *this;
        }

        constexpr float4 operator*=( const float4& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            w *= a.w;
            return *this;
        }

        constexpr float4 operator/=( const float4& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            w /= a.w;
            return *this;
        }

        constexpr friend float4 operator+( float4 a, const float4& b )
        {
            return a += b;
        }

        constexpr friend float4 operator-( float4 a, const float4& b )
        {
            return a -= b;
        }

        constexpr friend float4 operator*( float4 a, const float4& b )
        {
            return a *= b;
        }

        constexpr friend float4 operator/( float4 a, const float4& b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float4 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            z += a;
            w += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float4 operator-=( T a ) noexcept
        {
            x -= a;
            y -= a;
            z -= a;
            w -= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float4 operator*=( T a ) noexcept
        {
            const float s{static_cast<float>(a)};
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float4 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0f / a};
                return *this * s;
            }
            const float s{static_cast<float>(a)};
            x /= s;
            y /= s;
            z /= s;
            w /= s;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator+( float4 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator+( T b, float4 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator-( float4 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator-( T b, float4 a )
        {
            return {b - a.x, b - a.y, b - a.z, b - a.w};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator*( float4 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator*( T a, float4 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator/( float4 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float4 operator/( T a, const float4 b )
        {
            return float4{a} / b;
        }
    };

    struct alignas( 16 ) float3
    {
        float3() = default;

        constexpr float3( float a, float b, float c );

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr explicit float3( const T a )
            : x{static_cast<float>(a)}
            , y{static_cast<float>(a)}
            , z{static_cast<float>(a)}
        {
        }

        constexpr explicit float3( const float4& a )
            : x{a.x}
            , y{a.y}
            , z{a.z}
        {
        }

        constexpr explicit float3( const uint3& a )
            : x{static_cast<float>(a.x)}
            , y{static_cast<float>(a.y)}
            , z{static_cast<float>(a.z)}
        {
        }

        float x{};
        float y{};
        float z{};
        float dummy{};

        constexpr float& operator[]( const uint i )
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        constexpr const float& operator[]( const uint i ) const
        {
            switch ( i )
            {
                case 0: return x;
                case 1: return y;
                case 3: return z;
                default: throw std::out_of_range{"uint2 operator[]; index >= 2"};
            }
        }

        friend bool operator==( const float3& lhs, const float3& rhs )
        {
            return lhs.x == rhs.x
                    && lhs.y == rhs.y
                    && lhs.z == rhs.z;
        }

        friend bool operator!=( const float3& lhs, const float3& rhs )
        {
            return !( lhs == rhs );
        }

        constexpr float3 operator-() const noexcept
        {
            return {-x, -y, -z};
        }

        constexpr float3 operator+=( const float3& a ) noexcept
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }

        constexpr float3 operator-=( const float3& a ) noexcept
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }

        constexpr float3 operator*=( const float3& a ) noexcept
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }

        constexpr float3 operator/=( const float3& a ) noexcept
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            return *this;
        }

        constexpr friend float3 operator+( float3 a, const float3& b )
        {
            return a += b;
        }

        constexpr friend float3 operator-( float3 a, const float3& b )
        {
            return a -= b;
        }

        constexpr friend float3 operator*( float3 a, const float3& b )
        {
            return a *= b;
        }

        constexpr friend float3 operator/( float3 a, const float3& b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float3 operator+=( T a ) noexcept
        {
            x += a;
            y += a;
            z += a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float3 operator-=( T a ) noexcept
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float3 operator*=( T a ) noexcept
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr float3 operator/=( T a ) noexcept
        {
            if constexpr ( std::is_floating_point_v<T> )
            {
                const T s{1.0f / a};
                return *this * s;
            }
            else
            {
                x /= static_cast<float>(a);
                y /= static_cast<float>(a);
                z /= static_cast<float>(a);
                return *this;
            }
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator+( float3 a, T b )
        {
            return a += b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator+( T b, float3 a )
        {
            return a + b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator-( float3 a, T b )
        {
            return a -= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator-( T b, float3 a )
        {
            return {b - a.x, b - a.y, b - a.z};
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator*( float3 a, T b )
        {
            return a *= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator*( T a, float3 b )
        {
            return b * a;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator/( float3 a, T b )
        {
            return a /= b;
        }

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>  = true>
        constexpr friend float3 operator/( T a, const float3 b )
        {
            return float3{static_cast<float>(a)} / b;
        }
    };

    constexpr float3::float3( const float a, const float b, const float c )
        : x{a}
        , y{b}
        , z{c}
    {
    }

    inline int4::int4( const int3& a, const int d )
        : x{a.x}
        , y{a.y}
        , z{a.z}
        , w{d}
    {
    }

    inline uint4::uint4( const uint3& a, const uint d )
        : x{a.x}
        , y{a.y}
        , z{a.z}
        , w{d}
    {
    }

    inline float4::float4( const float3& a, const float d )
        : x{a.x}
        , y{a.y}
        , z{a.z}
        , w{d}
    {
    }

    inline float4::float4( const float3& a )
        : x{a.x}
        , y{a.y}
        , z{a.z}
        , w{0}
    {
    }
}// namespace FawnAlgebra

namespace std
{
    constexpr FawnAlgebra::float2 sqrt( const FawnAlgebra::float2& a )
    {
        return {std::sqrt( a.x ), std::sqrt( a.y )};
    }

    constexpr FawnAlgebra::float3 sqrt( const FawnAlgebra::float3& a )
    {
        return {std::sqrt( a.x ), std::sqrt( a.y ), std::sqrt( a.z )};
    }

    constexpr FawnAlgebra::float4 sqrt( const FawnAlgebra::float4& a )
    {
        return {std::sqrt( a.x ), std::sqrt( a.y ), std::sqrt( a.z ), std::sqrt( a.w )};
    }

    constexpr FawnAlgebra::float2 pow( const FawnAlgebra::float2& a, const float b )
    {
        return {std::pow( a.x, b ), std::pow( a.y, b )};
    }

    constexpr FawnAlgebra::float2 pow( const float a, const FawnAlgebra::float2& b )
    {
        return {std::pow( a, b.x ), std::pow( a, b.y )};
    }

    constexpr FawnAlgebra::float2 pow( const FawnAlgebra::float2& a, const FawnAlgebra::float2& b )
    {
        return {std::pow( a.x, b.x ), std::pow( a.y, b.y )};
    }

    constexpr FawnAlgebra::float3 pow( const FawnAlgebra::float3& a, const float b )
    {
        return {std::pow( a.x, b ), std::pow( a.y, b ), std::pow( a.z, b )};
    }

    constexpr FawnAlgebra::float3 pow( const float a, const FawnAlgebra::float3& b )
    {
        return {std::pow( a, b.x ), std::pow( a, b.y ), std::pow( a, b.z )};
    }

    constexpr FawnAlgebra::float3 pow( const FawnAlgebra::float3& a, const FawnAlgebra::float3& b )
    {
        return {std::pow( a.x, b.x ), std::pow( a.y, b.y ), std::pow( a.z, b.z )};
    }

    constexpr FawnAlgebra::float4 pow( const FawnAlgebra::float4& a, const float b )
    {
        return {std::pow( a.x, b ), std::pow( a.y, b ), std::pow( a.z, b ), std::pow( a.w, b )};
    }

    constexpr FawnAlgebra::float4 pow( const float a, const FawnAlgebra::float4& b )
    {
        return {std::pow( a, b.x ), std::pow( a, b.y ), std::pow( a, b.z ), std::pow( a, b.w )};
    }

    constexpr FawnAlgebra::float4 pow( const FawnAlgebra::float4& a, const FawnAlgebra::float4& b )
    {
        return {std::pow( a.x, b.x ), std::pow( a.y, b.y ), std::pow( a.z, b.z ), std::pow( a.w, b.w )};
    }

    constexpr FawnAlgebra::float2 fminf( const FawnAlgebra::float2& a, const FawnAlgebra::float2& b )
    {
        return {std::fminf( a.x, b.x ), std::fminf( a.y, b.y )};
    }

    constexpr FawnAlgebra::float3 fminf( const FawnAlgebra::float3& a, const FawnAlgebra::float3& b )
    {
        return {std::fminf( a.x, b.x ), std::fminf( a.y, b.y ), std::fminf( a.z, b.z )};
    }

    constexpr FawnAlgebra::float4 fminf( const FawnAlgebra::float4& a, const FawnAlgebra::float4& b )
    {
        return {std::fminf( a.x, b.x ), std::fminf( a.y, b.y ), std::fminf( a.z, b.z ), std::fminf( a.w, b.w )};
    }

    constexpr FawnAlgebra::int2 min( const FawnAlgebra::int2& a, const FawnAlgebra::int2& b )
    {
        return {std::min( a.x, b.x ), std::min( a.y, b.y )};
    }

    constexpr FawnAlgebra::int3 min( const FawnAlgebra::int3& a, const FawnAlgebra::int3& b )
    {
        return {std::min( a.x, b.x ), std::min( a.y, b.y ), std::min( a.z, b.z )};
    }

    constexpr FawnAlgebra::int4 min( const FawnAlgebra::int4& a, const FawnAlgebra::int4& b )
    {
        return {std::min( a.x, b.x ), std::min( a.y, b.y ), std::min( a.z, b.z ), std::min( a.w, b.w )};
    }

    constexpr FawnAlgebra::uint2 min( const FawnAlgebra::uint2& a, const FawnAlgebra::uint2& b )
    {
        return {std::min( a.x, b.x ), std::min( a.y, b.y )};
    }

    constexpr FawnAlgebra::uint3 min( const FawnAlgebra::uint3& a, const FawnAlgebra::uint3& b )
    {
        return {std::min( a.x, b.x ), std::min( a.y, b.y ), std::min( a.z, b.z )};
    }

    constexpr FawnAlgebra::uint4 min( const FawnAlgebra::uint4& a, const FawnAlgebra::uint4& b )
    {
        return {std::min( a.x, b.x ), std::min( a.y, b.y ), std::min( a.z, b.z ), std::min( a.w, b.w )};
    }

    constexpr FawnAlgebra::float2 fmaxf( const FawnAlgebra::float2& a, const FawnAlgebra::float2& b )
    {
        return {fmaxf( a.x, b.x ), fmaxf( a.y, b.y )};
    }

    constexpr FawnAlgebra::float3 fmaxf( const FawnAlgebra::float3& a, const FawnAlgebra::float3& b )
    {
        return {fmaxf( a.x, b.x ), fmaxf( a.y, b.y ), fmaxf( a.z, b.z )};
    }

    constexpr FawnAlgebra::float4 fmaxf( const FawnAlgebra::float4& a, const FawnAlgebra::float4& b )
    {
        return {fmaxf( a.x, b.x ), fmaxf( a.y, b.y ), fmaxf( a.z, b.z ), fmaxf( a.w, b.w )};
    }

    constexpr FawnAlgebra::int2 max( const FawnAlgebra::int2& a, const FawnAlgebra::int2& b )
    {
        return {max( a.x, b.x ), max( a.y, b.y )};
    }

    constexpr FawnAlgebra::int3 max( const FawnAlgebra::int3& a, const FawnAlgebra::int3& b )
    {
        return {max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z )};
    }

    constexpr FawnAlgebra::int4 max( const FawnAlgebra::int4& a, const FawnAlgebra::int4& b )
    {
        return {max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ), max( a.w, b.w )};
    }

    constexpr FawnAlgebra::uint2 max( const FawnAlgebra::uint2& a, const FawnAlgebra::uint2& b )
    {
        return {max( a.x, b.x ), max( a.y, b.y )};
    }

    constexpr FawnAlgebra::uint3 max( const FawnAlgebra::uint3& a, const FawnAlgebra::uint3& b )
    {
        return {max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z )};
    }

    constexpr FawnAlgebra::uint4 max( const FawnAlgebra::uint4& a, const FawnAlgebra::uint4& b )
    {
        return {max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ), max( a.w, b.w )};
    }

    constexpr FawnAlgebra::float2 floor( const FawnAlgebra::float2& v )
    {
        return {std::floor( v.x ), std::floor( v.y )};
    }

    constexpr FawnAlgebra::float3 floor( const FawnAlgebra::float3& v )
    {
        return {std::floor( v.x ), std::floor( v.y ), std::floor( v.z )};
    }

    constexpr FawnAlgebra::float4 floor( const FawnAlgebra::float4& v )
    {
        return {std::floor( v.x ), std::floor( v.y ), std::floor( v.z ), std::floor( v.w )};
    }

    constexpr FawnAlgebra::float2 fmod( const FawnAlgebra::float2& a, const FawnAlgebra::float2& b )
    {
        return {std::fmod( a.x, b.x ), std::fmod( a.y, b.y )};
    }

    constexpr FawnAlgebra::float3 fmod( const FawnAlgebra::float3& a, const FawnAlgebra::float3& b )
    {
        return {std::fmod( a.x, b.x ), std::fmod( a.y, b.y ), std::fmod( a.z, b.z )};
    }

    constexpr FawnAlgebra::float4 fmod( const FawnAlgebra::float4& a, const FawnAlgebra::float4& b )
    {
        return {std::fmod( a.x, b.x ), std::fmod( a.y, b.y ), std::fmod( a.z, b.z ), std::fmod( a.w, b.w )};
    }

    constexpr FawnAlgebra::float2 fabs( const FawnAlgebra::float2& v )
    {
        return {std::fabs( v.x ), std::fabs( v.y )};
    }

    constexpr FawnAlgebra::float3 fabs( const FawnAlgebra::float3& v )
    {
        return {std::fabs( v.x ), std::fabs( v.y ), std::fabs( v.z )};
    }

    constexpr FawnAlgebra::float4 fabs( const FawnAlgebra::float4& v )
    {
        return {std::fabs( v.x ), std::fabs( v.y ), std::fabs( v.z ), std::fabs( v.w )};
    }

    constexpr FawnAlgebra::int2 abs( const FawnAlgebra::int2& v )
    {
        return {std::abs( v.x ), std::abs( v.y )};
    }

    constexpr FawnAlgebra::int3 abs( const FawnAlgebra::int3& v )
    {
        return {std::abs( v.x ), std::abs( v.y ), std::abs( v.z )};
    }

    constexpr FawnAlgebra::int4 abs( const FawnAlgebra::int4& v )
    {
        return {std::abs( v.x ), std::abs( v.y ), std::abs( v.z ), std::abs( v.w )};
    }
}// namespace std
