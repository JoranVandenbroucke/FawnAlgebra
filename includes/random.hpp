//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

#pragma once

#include <cstdint>
#include <ctime>

#include "interpolation.hpp"

namespace FawnAlgebra
{
    constexpr uint32_t MAX_PERMUTATION_INDEX{255};
    constexpr uint32_t permutation[ MAX_PERMUTATION_INDEX + 1 ]
            = {151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
               203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105,
               92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52,
               217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155,
               167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
               107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

    // Random number generator is deterministic \o/
    // Queue complaints of "random isn't random enough"
    // I have hash functions if you want something quick and "random"
    class Random
    {
    public:
        constexpr explicit Random( const uint32_t seed = time( nullptr ) & MAX_PERMUTATION_INDEX )
            : m_offset{seed & MAX_PERMUTATION_INDEX}
        {
        }

        constexpr void initState( const uint32_t seed = time( nullptr ) & MAX_PERMUTATION_INDEX ) noexcept
        {
            m_offset = seed & MAX_PERMUTATION_INDEX;
        }

        constexpr uint32_t next() noexcept
        {
            const uint32_t value = permutation[ m_offset ];
            m_offset = ++m_offset & MAX_PERMUTATION_INDEX;
            return value;
        }

        constexpr uint32_t next( const uint32_t min, const uint32_t max ) noexcept
        {
            const uint32_t diff = max - min;
            const uint32_t value = permutation[ m_offset ];
            m_offset = ++m_offset & MAX_PERMUTATION_INDEX;
            return min + value % diff;
        }

        constexpr uint32_t next( const uint32_t max ) noexcept
        {
            return next( 0, max );
        }

        constexpr uint64_t next64() noexcept
        {
            const uint32_t value1 = permutation[ m_offset ];
            m_offset = ++m_offset & MAX_PERMUTATION_INDEX;
            const uint32_t value2 = permutation[ m_offset ];
            m_offset = ++m_offset & MAX_PERMUTATION_INDEX;

            const uint64_t value3 = static_cast<uint64_t>(value1) << 32ULL;
            return value3 | static_cast<uint64_t>(value2);
        }

        constexpr uint32_t next64( const uint64_t min, const uint64_t max ) noexcept
        {
            const uint32_t diff = max - min;
            const uint32_t value1 = permutation[ m_offset ];
            m_offset = ++m_offset & MAX_PERMUTATION_INDEX;
            const uint32_t value2 = permutation[ m_offset ];
            m_offset = ++m_offset & MAX_PERMUTATION_INDEX;

            const uint64_t value3 = static_cast<uint64_t>(value1) << 32ULL;
            return min + ( value3 | value2 ) % diff;
        }

        constexpr uint32_t next64( const uint64_t max ) noexcept
        {
            return next64( 0, max );
        }

        constexpr double nextDouble() noexcept
        {
            constexpr double div = 1.0 / 255.0;
            const uint32_t value = next();

            return static_cast<double>(value) * div;
        }

        constexpr float nextSingle() noexcept
        {
            constexpr float div = 1.0F / 255.0F;
            const uint32_t value = next();

            return static_cast<float>(value) * div;
        }

        template<size_t N>
        constexpr void nextBytes( uint32_t ( &data )[ N ] ) noexcept
        {
            for ( size_t i = 0; i < N; i++ )
            {
                data[ i ] = next();
            }
        }

        // https://mrl.cs.nyu.edu/~perlin/noise/
        static constexpr float Perlin( float x, float y, float z = 0.0f ) noexcept
        {
            const uint32_t X = static_cast<uint32_t>(std::floor( x )) & MAX_PERMUTATION_INDEX;// FIND UNIT CUBE THAT
            const uint32_t Y = static_cast<uint32_t>(std::floor( y )) & MAX_PERMUTATION_INDEX;// CONTAINS POINT.
            const uint32_t Z = static_cast<uint32_t>(std::floor( z )) & MAX_PERMUTATION_INDEX;

            x -= std::floor( x );// FIND RELATIVE X,Y,Z
            y -= std::floor( y );// OF POINT IN CUBE.
            z -= std::floor( z );

            const float u = fade( x );// COMPUTE FADE CURVES
            const float v = fade( y );// FOR EACH OF X,Y,Z.
            const float w = fade( z );

            const uint32_t A = permutation[ X ] + Y; // HASH COORDINATES OF
            const uint32_t AA = permutation[ A ] + Z;// THE 8 CUBE CORNERS,
            const uint32_t AB = permutation[ A + 1 & MAX_PERMUTATION_INDEX ] + Z;
            const uint32_t B = permutation[ X + 1 & MAX_PERMUTATION_INDEX ] + Y;
            const uint32_t BA = permutation[ B ] + Z;
            const uint32_t BB = permutation[ B + 1 & MAX_PERMUTATION_INDEX ] + Z;

            return lerp( w,
                         lerp( v,
                               lerp( u,
                                     grad( permutation[ AA & MAX_PERMUTATION_INDEX ], x, y, z ),
                                     // AND ADD
                                     grad( permutation[ BA & MAX_PERMUTATION_INDEX ], x - 1, y, z ) ),
                               // BLENDED
                               lerp( u,
                                     grad( permutation[ AB & MAX_PERMUTATION_INDEX ], x, y - 1, z ),
                                     // RESULTS
                                     grad( permutation[ BB & MAX_PERMUTATION_INDEX ], x - 1, y - 1, z ) ) ),
                         // FROM  8
                         lerp( v,
                               lerp( u,
                                     grad( permutation[ AA + 1 & MAX_PERMUTATION_INDEX ], x, y, z - 1 ),
                                     // CORNERS
                                     grad( permutation[ BA + 1 & MAX_PERMUTATION_INDEX ], x - 1, y, z - 1 ) ),
                               // OF CUBE
                               lerp( u, grad( permutation[ AB + 1 & MAX_PERMUTATION_INDEX ], x, y - 1, z - 1 ), grad( permutation[ BB + 1 & MAX_PERMUTATION_INDEX ], x - 1, y - 1, z - 1 ) ) ) );
        }

    private:
        uint32_t m_offset;
    };
}// namespace FawnAlgebra
