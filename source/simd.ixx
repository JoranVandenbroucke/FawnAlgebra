//
// Copyright (c) 2025.
// Author: Joran.
//
module;
#include <array>
#include <cstdint>
#include <immintrin.h>

// #include "config/architecture.hpp"
export module FawnAlgebra.SIMD;

export template<typename, std::size_t>
class simd;

template<>
class simd<uint8_t, 32>
{
public:
    using value_type = uint8_t;
    using abi_type   = __m256i;

    simd() noexcept = default;
    explicit simd( const abi_type& value ) noexcept : m_value( value )
    {
    }
    explicit simd( const value_type* data ) noexcept
    {
        load( data );
    }
    explicit simd( const value_type ( &data )[ 32 ] ) noexcept
    {
        load( data );
    }
    explicit simd( const std::array<value_type, 32>& data ) noexcept
    {
        load( data.data() );
    }

    [[nodiscard]] bool operator==( const simd& other ) const noexcept
    {
        const abi_type cmp_mask { _mm256_cmpeq_epi8( m_value, other.m_value ) };
        return _mm256_movemask_epi8( cmp_mask ) == static_cast<int>( 0xFFFFFFFF );
    }

    [[nodiscard]] bool operator!=( const simd& other ) const noexcept
    {
        return !( *this == other );
    }

    [[nodiscard]] auto& Abs() const noexcept
    {
        return *this;
    }

    value_type operator[]( const int idx ) const noexcept
    {
        return static_cast<value_type>( _mm256_extract_epi8( m_value, idx ) );
    }

    auto operator~() const noexcept
    {
        return simd( _mm256_andnot_si256( m_value, _mm256_set1_epi8( -1 ) ) );
    }

    auto& operator+=( const simd other ) noexcept
    {
        m_value = _mm256_add_epi8( m_value, other.m_value );
        return *this;
    }

    auto& operator-=( const simd other ) noexcept
    {
        m_value = _mm256_sub_epi8( m_value, other.m_value );
        return *this;
    }

    auto& operator*=( const simd other ) noexcept
    {
        // AVX2 doesn't directly support 8-bit multiplication.
        // We use unpacking and handle the halves separately.
        const abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        const abi_type lo_product { _mm256_mullo_epi16( lo1, lo2 ) };
        const abi_type hi_product { _mm256_mullo_epi16( hi1, hi2 ) };

        m_value = _mm256_packus_epi16( lo_product, hi_product );
        return *this;
    }

    auto& operator/=( const simd other ) noexcept
    {
        // Integer division needs to be implemented manually.
        alignas( 32 ) uint8_t lhs[ 32 ], rhs[ 32 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i = 0; i < 32; ++i )
        {
            lhs[ i ] /= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator%=( const simd other ) noexcept
    {
        alignas( 32 ) value_type lhs[ 32 ], rhs[ 32 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i {}; i < 32; ++i )
        {
            lhs[ i ] %= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator<<=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) uint16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<uint16_t>( data1[ i ] << data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator>>=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) uint16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<uint16_t>( data1[ i ] >> data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator^=( const simd& other ) noexcept
    {
        m_value = _mm256_xor_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator|=( const simd& other ) noexcept
    {
        m_value = _mm256_or_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator&=( const simd& other ) noexcept
    {
        m_value = _mm256_and_si256( m_value, other.m_value );
        return *this;
    }

    auto& SaturateAdd( const simd& other ) noexcept
    {
        m_value = _mm256_adds_epu8( m_value, other.m_value );
        return *this;
    }

    auto& SaturateSubtract( const simd& other ) noexcept
    {
        m_value = _mm256_subs_epu8( m_value, other.m_value );
        return *this;
    }

    auto& AlignsElements( const simd& other, const int mask ) noexcept
    {
        m_value = _mm256_alignr_epi8( m_value, other.m_value, mask );
        return *this;
    }

    auto& Blend( const simd& other, const simd& mask ) noexcept
    {
        m_value = _mm256_blendv_epi8( m_value, other.m_value, mask.m_value );
        return *this;
    }

    auto& Broadcast( const simd& other ) noexcept
    {
        m_value = _mm256_broadcastb_epi8( _mm256_castsi256_si128( other.m_value ) );
        return *this;
    }

    auto& Equals( const simd& other ) noexcept
    {
        m_value = _mm256_cmpeq_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Greater( const simd& other ) noexcept
    {
        m_value = _mm256_cmpgt_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Min( const simd& other ) noexcept
    {
        m_value = _mm256_min_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Max( const simd& other ) noexcept
    {
        m_value = _mm256_max_epi8( m_value, other.m_value );
        return *this;
    }

    [[nodiscard]] int32_t MoveMask() const noexcept
    {
        return _mm256_movemask_epi8( m_value );
    }

    auto& Shuffle( const simd& other ) noexcept
    {
        m_value = _mm256_shuffle_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackHigh( const simd& other ) noexcept
    {
        m_value = _mm256_unpackhi_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackLow( const simd& other ) noexcept
    {
        m_value = _mm256_unpacklo_epi8( m_value, other.m_value );
        return *this;
    }

private:
    abi_type m_value { _mm256_setzero_si256() };

    void load( const value_type* data ) noexcept
    {
        m_value = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data ) );
    }

    void store( value_type* data ) const noexcept
    {
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data ), m_value );
    }
};

template<>
class simd<int8_t, 32>
{
public:
    using value_type = int8_t;
    using abi_type   = __m256i;
    simd() noexcept  = default;
    explicit simd( const abi_type& value ) noexcept : m_value( value )
    {
    }
    explicit simd( const value_type* data ) noexcept
    {
        load( data );
    }
    explicit simd( const value_type ( &data )[ 8 ] ) noexcept
    {
        load( data );
    }
    explicit simd( const std::array<value_type, 8>& data ) noexcept
    {
        load( data.data() );
    }

    [[nodiscard]] bool operator==( const simd& other ) const noexcept
    {
        const abi_type cmp_mask { _mm256_cmpeq_epi8( m_value, other.m_value ) };
        return _mm256_movemask_epi8( cmp_mask ) == 0xFFFF;
    }

    [[nodiscard]] bool operator!=( const simd& other ) const noexcept
    {
        return !( *this == other );
    }

    [[nodiscard]] auto& Abs() noexcept
    {
        m_value = _mm256_abs_epi8( m_value );
        return *this;
    }

    value_type operator[]( const int idx ) const noexcept
    {
        return static_cast<value_type>( _mm256_extract_epi8( m_value, idx ) );
    }

    auto& operator+=( const simd other ) noexcept
    {
        m_value = _mm256_add_epi8( m_value, other.m_value );
        return *this;
    }

    auto& operator-=( const simd other ) noexcept
    {
        m_value = _mm256_sub_epi8( m_value, other.m_value );
        return *this;
    }

    auto& operator*=( const simd other ) noexcept
    {
        // AVX2 doesn't directly support 8-bit multiplication.
        // We use unpacking and handle the halves separately.
        const abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        const abi_type lo_product { _mm256_mullo_epi16( lo1, lo2 ) };
        const abi_type hi_product { _mm256_mullo_epi16( hi1, hi2 ) };

        m_value = _mm256_packus_epi16( lo_product, hi_product );
        return *this;
    }

    auto& operator/=( const simd other ) noexcept
    {
        // Integer division needs to be implemented manually.
        alignas( 32 ) value_type lhs[ 32 ], rhs[ 32 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i = 0; i < 32; ++i )
        {
            lhs[ i ] /= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator%=( const simd other ) noexcept
    {
        alignas( 32 ) value_type lhs[ 32 ], rhs[ 32 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i = 0; i < 32; ++i )
        {
            lhs[ i ] %= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator<<=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) int16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<int16_t>( data1[ i ] << data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator>>=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) int16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<int16_t>( data1[ i ] >> data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator^=( const simd& other ) noexcept
    {
        m_value = _mm256_xor_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator|=( const simd& other ) noexcept
    {
        m_value = _mm256_or_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator&=( const simd& other ) noexcept
    {
        m_value = _mm256_and_si256( m_value, other.m_value );
        return *this;
    }

    auto& SaturateAdd( const simd& other ) noexcept
    {
        m_value = _mm256_adds_epi8( m_value, other.m_value );
        return *this;
    }

    auto& SaturateSubtract( const simd& other ) noexcept
    {
        m_value = _mm256_subs_epi8( m_value, other.m_value );
        return *this;
    }

    auto& AlignsElements( const simd& other, const int mask ) noexcept
    {
        m_value = _mm256_alignr_epi8( m_value, other.m_value, mask );
        return *this;
    }

    auto& Blend( const simd& other, const simd& mask ) noexcept
    {
        m_value = _mm256_blendv_epi8( m_value, other.m_value, mask.m_value );
        return *this;
    }

    auto& Broadcast( const simd& other ) noexcept
    {
        m_value = _mm256_broadcastb_epi8( _mm256_castsi256_si128( other.m_value ) );
        return *this;
    }

    auto& Equals( const simd& other ) noexcept
    {
        m_value = _mm256_cmpeq_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Greater( const simd& other ) noexcept
    {
        m_value = _mm256_cmpgt_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Min( const simd& other ) noexcept
    {
        m_value = _mm256_min_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Max( const simd& other ) noexcept
    {
        m_value = _mm256_max_epi8( m_value, other.m_value );
        return *this;
    }

    [[nodiscard]] int32_t MoveMask() const noexcept
    {
        return _mm256_movemask_epi8( m_value );
    }

    auto& Shuffle( const simd& other ) noexcept
    {
        m_value = _mm256_shuffle_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackHigh( const simd& other ) noexcept
    {
        m_value = _mm256_unpackhi_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackLow( const simd& other ) noexcept
    {
        m_value = _mm256_unpacklo_epi8( m_value, other.m_value );
        return *this;
    }

    auto operator~() const noexcept
    {
        return simd( _mm256_andnot_si256( m_value, _mm256_set1_epi8( -1 ) ) );
    }

private:
    abi_type m_value { _mm256_setzero_si256() };

    void load( const value_type* data ) noexcept
    {
        m_value = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data ) );
    }

    void store( value_type* data ) const noexcept
    {
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data ), m_value );
    }
};

template<>
class simd<uint16_t, 16>
{
public:
    using value_type = uint16_t;
    using abi_type   = __m256i;

    simd() noexcept = default;
    explicit simd( const abi_type& value ) noexcept : m_value( value )
    {
    }
    explicit simd( const value_type* data ) noexcept
    {
        load( data );
    }
    explicit simd( const value_type ( &data )[ 32 ] ) noexcept
    {
        load( data );
    }
    explicit simd( const std::array<value_type, 32>& data ) noexcept
    {
        load( data.data() );
    }

    [[nodiscard]] bool operator==( const simd& other ) const noexcept
    {
        const abi_type cmp_mask { _mm256_cmpeq_epi16( m_value, other.m_value ) };
        return _mm256_movemask_epi8( cmp_mask ) == static_cast<int>( 0xFFFF );
    }

    [[nodiscard]] bool operator!=( const simd& other ) const noexcept
    {
        return !( *this == other );
    }

    [[nodiscard]] auto& Abs() const noexcept
    {
        return *this;
    }

    value_type operator[]( const int idx ) const noexcept
    {
        return static_cast<value_type>( _mm256_extract_epi16( m_value, idx ) );
    }

    auto operator~() const noexcept
    {
        return simd( _mm256_andnot_si256( m_value, _mm256_set1_epi16( -1 ) ) );
    }

    auto& operator+=( const simd other ) noexcept
    {
        m_value = _mm256_add_epi16( m_value, other.m_value );
        return *this;
    }

    auto& operator-=( const simd other ) noexcept
    {
        m_value = _mm256_sub_epi16( m_value, other.m_value );
        return *this;
    }

    auto& operator*=( const simd other ) noexcept
    {
        m_value = _mm256_madd_epi16( m_value, other.m_value );
        return *this;
    }

    auto& operator/=( const simd other ) noexcept
    {
        // Integer division needs to be implemented manually.
        alignas( 32 ) value_type lhs[ 16 ], rhs[ 16 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i = 0; i < 32; ++i )
        {
            lhs[ i ] /= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator%=( const simd other ) noexcept
    {
        alignas( 32 ) value_type lhs[ 32 ], rhs[ 32 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i {}; i < 32; ++i )
        {
            lhs[ i ] %= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator<<=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) uint16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<uint16_t>( data1[ i ] << data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator>>=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) uint16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<uint16_t>( data1[ i ] >> data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator^=( const simd& other ) noexcept
    {
        m_value = _mm256_xor_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator|=( const simd& other ) noexcept
    {
        m_value = _mm256_or_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator&=( const simd& other ) noexcept
    {
        m_value = _mm256_and_si256( m_value, other.m_value );
        return *this;
    }

    auto& SaturateAdd( const simd& other ) noexcept
    {
        m_value = _mm256_adds_epi8( m_value, other.m_value );
        return *this;
    }

    auto& SaturateSubtract( const simd& other ) noexcept
    {
        m_value = _mm256_subs_epi8( m_value, other.m_value );
        return *this;
    }

    auto& AlignsElements( const simd& other, const int mask ) noexcept
    {
        m_value = _mm256_alignr_epi8( m_value, other.m_value, mask );
        return *this;
    }

    auto& Blend( const simd& other, const simd& mask ) noexcept
    {
        m_value = _mm256_blendv_epi8( m_value, other.m_value, mask.m_value );
        return *this;
    }

    auto& Broadcast( const simd& other ) noexcept
    {
        m_value = _mm256_broadcastb_epi8( _mm256_castsi256_si128( other.m_value ) );
        return *this;
    }

    auto& Equals( const simd& other ) noexcept
    {
        m_value = _mm256_cmpeq_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Greater( const simd& other ) noexcept
    {
        m_value = _mm256_cmpgt_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Min( const simd& other ) noexcept
    {
        m_value = _mm256_min_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Max( const simd& other ) noexcept
    {
        m_value = _mm256_max_epi8( m_value, other.m_value );
        return *this;
    }

    [[nodiscard]] int32_t MoveMask() const noexcept
    {
        return _mm256_movemask_epi8( m_value );
    }

    auto& Shuffle( const simd& other ) noexcept
    {
        m_value = _mm256_shuffle_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackHigh( const simd& other ) noexcept
    {
        m_value = _mm256_unpackhi_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackLow( const simd& other ) noexcept
    {
        m_value = _mm256_unpacklo_epi8( m_value, other.m_value );
        return *this;
    }

private:
    abi_type m_value { _mm256_setzero_si256() };

    void load( const value_type* data ) noexcept
    {
        m_value = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data ) );
    }

    void store( value_type* data ) const noexcept
    {
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data ), m_value );
    }
};

template<>
class simd<int16_t, 32>
{
public:
    using value_type = int16_t;
    using abi_type   = __m256i;
    simd() noexcept  = default;
    explicit simd( const abi_type& value ) noexcept : m_value( value )
    {
    }
    explicit simd( const value_type* data ) noexcept
    {
        load( data );
    }
    explicit simd( const value_type ( &data )[ 8 ] ) noexcept
    {
        load( data );
    }
    explicit simd( const std::array<value_type, 8>& data ) noexcept
    {
        load( data.data() );
    }

    [[nodiscard]] bool operator==( const simd& other ) const noexcept
    {
        const abi_type cmp_mask { _mm256_cmpeq_epi8( m_value, other.m_value ) };
        return _mm256_movemask_epi8( cmp_mask ) == 0xFFFF;
    }

    [[nodiscard]] bool operator!=( const simd& other ) const noexcept
    {
        return !( *this == other );
    }

    [[nodiscard]] auto& Abs() noexcept
    {
        m_value = _mm256_abs_epi8( m_value );
        return *this;
    }

    value_type operator[]( const int idx ) const noexcept
    {
        return static_cast<value_type>( _mm256_extract_epi8( m_value, idx ) );
    }

    auto& operator+=( const simd other ) noexcept
    {
        m_value = _mm256_add_epi8( m_value, other.m_value );
        return *this;
    }

    auto& operator-=( const simd other ) noexcept
    {
        m_value = _mm256_sub_epi8( m_value, other.m_value );
        return *this;
    }

    auto& operator*=( const simd other ) noexcept
    {
        // AVX2 doesn't directly support 8-bit multiplication.
        // We use unpacking and handle the halves separately.
        const abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        const abi_type lo_product { _mm256_mullo_epi16( lo1, lo2 ) };
        const abi_type hi_product { _mm256_mullo_epi16( hi1, hi2 ) };

        m_value = _mm256_packus_epi16( lo_product, hi_product );
        return *this;
    }

    auto& operator/=( const simd other ) noexcept
    {
        // Integer division needs to be implemented manually.
        alignas( 32 ) value_type lhs[ 32 ], rhs[ 32 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i = 0; i < 32; ++i )
        {
            lhs[ i ] /= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator%=( const simd other ) noexcept
    {
        alignas( 32 ) value_type lhs[ 32 ], rhs[ 32 ];
        store( lhs );
        other.store( rhs );

        for ( std::size_t i = 0; i < 32; ++i )
        {
            lhs[ i ] %= rhs[ i ];
        }

        load( lhs );
        return *this;
    }

    auto& operator<<=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) int16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<int16_t>( data1[ i ] << data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator>>=( const simd& other )
    {
        abi_type lo1 { _mm256_unpacklo_epi8( m_value, _mm256_setzero_si256() ) };
        abi_type hi1 { _mm256_unpackhi_epi8( m_value, _mm256_setzero_si256() ) };
        const abi_type lo2 { _mm256_unpacklo_epi8( other.m_value, _mm256_setzero_si256() ) };
        const abi_type hi2 { _mm256_unpackhi_epi8( other.m_value, _mm256_setzero_si256() ) };

        // Perform per-element shifts using a scalar loop.
        alignas( 32 ) int16_t data1[ 16 ], data2[ 16 ];
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 ), lo1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data1 + 8 ), hi1 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 ), lo2 );
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data2 + 8 ), hi2 );

        for ( std::size_t i = 0; i < 16; ++i )
        {
            data1[ i ] = static_cast<int16_t>( data1[ i ] >> data2[ i ] );
        }

        // Load results back.
        lo1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 ) );
        hi1 = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data1 + 8 ) );

        m_value = _mm256_packus_epi16( lo1, hi1 );
        return *this;
    }

    auto& operator^=( const simd& other ) noexcept
    {
        m_value = _mm256_xor_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator|=( const simd& other ) noexcept
    {
        m_value = _mm256_or_si256( m_value, other.m_value );
        return *this;
    }

    auto& operator&=( const simd& other ) noexcept
    {
        m_value = _mm256_and_si256( m_value, other.m_value );
        return *this;
    }

    auto& SaturateAdd( const simd& other ) noexcept
    {
        m_value = _mm256_adds_epi8( m_value, other.m_value );
        return *this;
    }

    auto& SaturateSubtract( const simd& other ) noexcept
    {
        m_value = _mm256_subs_epi8( m_value, other.m_value );
        return *this;
    }

    auto& AlignsElements( const simd& other, const int mask ) noexcept
    {
        m_value = _mm256_alignr_epi8( m_value, other.m_value, mask );
        return *this;
    }

    auto& Blend( const simd& other, const simd& mask ) noexcept
    {
        m_value = _mm256_blendv_epi8( m_value, other.m_value, mask.m_value );
        return *this;
    }

    auto& Broadcast( const simd& other ) noexcept
    {
        m_value = _mm256_broadcastb_epi8( _mm256_castsi256_si128( other.m_value ) );
        return *this;
    }

    auto& Equals( const simd& other ) noexcept
    {
        m_value = _mm256_cmpeq_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Greater( const simd& other ) noexcept
    {
        m_value = _mm256_cmpgt_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Min( const simd& other ) noexcept
    {
        m_value = _mm256_min_epi8( m_value, other.m_value );
        return *this;
    }

    auto& Max( const simd& other ) noexcept
    {
        m_value = _mm256_max_epi8( m_value, other.m_value );
        return *this;
    }

    [[nodiscard]] int32_t MoveMask() const noexcept
    {
        return _mm256_movemask_epi8( m_value );
    }

    auto& Shuffle( const simd& other ) noexcept
    {
        m_value = _mm256_shuffle_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackHigh( const simd& other ) noexcept
    {
        m_value = _mm256_unpackhi_epi8( m_value, other.m_value );
        return *this;
    }

    auto& UnpackLow( const simd& other ) noexcept
    {
        m_value = _mm256_unpacklo_epi8( m_value, other.m_value );
        return *this;
    }

    auto operator~() const noexcept
    {
        return simd( _mm256_andnot_si256( m_value, _mm256_set1_epi8( -1 ) ) );
    }

private:
    abi_type m_value { _mm256_setzero_si256() };

    void load( const value_type* data ) noexcept
    {
        m_value = _mm256_loadu_si256( reinterpret_cast<const abi_type*>( data ) );
    }

    void store( value_type* data ) const noexcept
    {
        _mm256_storeu_si256( reinterpret_cast<abi_type*>( data ), m_value );
    }
};

export template<typename T, std::size_t N>
auto operator+( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs += rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator-( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs -= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator*( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs *= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator/( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs /= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator<<( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs <<= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator>>( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs >>= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator|( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs |= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator^( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs ^= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto operator&( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs &= rhs;
    return lhs;
}

export template<typename T, std::size_t N>
auto SaturateAdd( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.SaturateAdd( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto SaturateSubtract( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.SaturateSubtract( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto AlignsElements( simd<T, N> lhs, const simd<T, N>& rhs, const int mask ) noexcept
{
    lhs.AlignsElements( rhs, mask );
    return lhs;
}

export template<typename T, std::size_t N>
auto Blend( simd<T, N> lhs, const simd<T, N>& rhs, const simd<T, N>& mask ) noexcept
{
    lhs.Blend( rhs, mask );
    return lhs;
}

export template<typename T, std::size_t N>
auto Broadcast( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.Broadcast( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto Equals( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.Equals( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto Greater( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.Greater( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto Min( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.Min( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto Max( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.Max( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto MoveMask( const simd<T, N>& other ) noexcept
{
    return other.MoveMask();
}

export template<typename T, std::size_t N>
auto Shuffle( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.Shuffle( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto UnpackHigh( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.UnpackHigh( rhs );
    return lhs;
}

export template<typename T, std::size_t N>
auto UnpackLow( simd<T, N> lhs, const simd<T, N>& rhs ) noexcept
{
    lhs.UnpackLow( rhs );
    return lhs;
}
