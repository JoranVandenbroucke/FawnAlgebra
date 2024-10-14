//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

module;
#include <bit>
#include <cstddef>

export module FawnAlgebra.Hashing;
import FawnAlgebra.Arithmetics;

namespace FawnAlgebra
{
    constexpr float intAsFloat( const uint32 i ) noexcept
    {
        return std::bit_cast<float>( i );
    }

    [[nodiscard]] constexpr double ptrAsFloat( const void* p ) noexcept
    {
        return std::bit_cast<double>( p );
    }

    constexpr uint32 floatAsInt( const float f ) noexcept
    {
        return std::bit_cast<uint32>( f );
    }

    constexpr uint64 ptrAsInt( const void* p ) noexcept
    {
        return std::bit_cast<uint64>( p );
    }

    constexpr const void* intAsPtr( const uint64 i ) noexcept
    {
        return std::bit_cast<const void*>( i );
    }

    constexpr const void* floatAsPtr( const double f ) noexcept
    {
        return std::bit_cast<const void*>( f );
    }

    // https://github.com/blender/blender/blob/main/intern/cycles/util/hash.h
    /* [0, uint_max] -> [0.0, 1.0) */
    export constexpr float uintToFloatExcl( const uint32 n ) noexcept
    {
        // Note: we divide by 4294967808 instead of 2^32 because the latter
        // leads to a [0.0, 1.0] mapping instead of [0.0, 1.0) due to floating
        // point rounding error. 4294967808 unfortunately leaves (precisely)
        // one unused ulp between the max number this outputs and 1.0, but
        // that's the best you can do with this construction.
        return static_cast<float>(n) * ( 1.0f / 4294967808.0f );
    }

    /* [0, uint_max] -> [0.0, 1.0] */
    export constexpr float uintToFloatIncl( const uint32 n ) noexcept
    {
        return static_cast<float>(n) * ( 1.0f / static_cast<float>(0xFFFFFFFFU) );
    }

    #pragma region yorku
    // http://www.cse.yorku.ca/~oz/hash.html
    export [[nodiscard]] constexpr unsigned long hashDjb2( const char* str ) noexcept
    {
        unsigned long hash = 5381;
        int c;

        while ( ( c = static_cast<unsigned char>(*str++) ) )
        {
            hash = ( hash << 5 ) + hash + c;/* hash * 33 + c */
        }

        return hash;
    }

    export [[nodiscard]] constexpr unsigned long hashSdbm( const char* str ) noexcept
    {
        unsigned long hash = 0;
        int c;

        while ( ( c = static_cast<unsigned char>(*str++) ) )
        {
            hash = c + ( hash << 6 ) + ( hash << 16 ) - hash;
        }

        return hash;
    }

    export [[nodiscard]] constexpr unsigned long hashLoseLose( const char* str ) noexcept
    {
        unsigned int hash = 0;
        int c;

        while ( ( c = static_cast<unsigned char>(*str++) ) )
        {
            hash += c;
        }

        return hash;
    }
    #pragma endregion

    #pragma region burtleburlte lookup 3
    // http://burtleburtle.net/bob/c/lookup3.c
    constexpr uint32 hashsize( const uint32 n ) noexcept
    {
        return 1U << n;
    }

    constexpr uint32 hasmask( const uint32 n ) noexcept
    {
        return hashsize( n ) - 1;
    }

    constexpr uint32 rot( const uint32 x, const uint32 k ) noexcept
    {
        return ( x << k ) | ( x >> ( 32U - k ) );
    }

    constexpr void mix( uint32& a, uint32& b, uint32& c ) noexcept
    {
        a -= c;
        a ^= rot( c, 4U );
        c += b;

        b -= a;
        b ^= rot( a, 6U );
        a += c;

        c -= b;
        c ^= rot( b, 8U );
        b += a;

        a -= c;
        a ^= rot( c, 16U );
        c += b;

        b -= a;
        b ^= rot( a, 19U );
        a += c;

        c -= b;
        c ^= rot( b, 4U );
        b += a;
    }

    constexpr void final( uint32& a, uint32& b, uint32& c ) noexcept
    {
        c ^= b;
        c -= rot( b, 14U );

        a ^= c;
        a -= rot( c, 11U );

        b ^= a;
        b -= rot( a, 25U );

        c ^= b;
        c -= rot( b, 16U );

        a ^= c;
        a -= rot( c, 4U );

        b ^= a;
        b -= rot( a, 14U );

        c ^= b;
        c -= rot( b, 24U );
    }

    export constexpr uint32 hashword( const uint32*& k,              /* the key, an array of uint32 values */
                                        uint32 length,                 /* the length of the key, in uint32s */
                                        const uint32 initval ) noexcept/* the previous hash, or an arbitrary value */
    {
        uint32 b;
        uint32 c;

        /* Set up the internal state */
        uint32 a = b = c = 0xdeadbeef + ( length << 2U ) + initval;

        /*------------------------------------------------- handle most of the key */
        while ( length > 3 )
        {
            a += k[ 0 ];
            b += k[ 1 ];
            c += k[ 2 ];
            mix( a, b, c );
            length -= 3;
            k += 3;
        }

        /*------------------------------------------- handle the last 3 uint32's */
        switch ( length )/* all the case statements fall through */
        {
            case 3: c += k[ 2 ];
            case 2: b += k[ 1 ];
            case 1: a += k[ 0 ];
                final( a, b, c );
            case 0: [[fallthrough]];
            default:
                /* case 0: nothing left to add */ break;
        }
        /*------------------------------------------------------ report the result */
        return c;
    }

    export template<std::size_t length>                           /* the length of the key, in uint32s */
    constexpr uint32 hashword( const uint32 ( &k )[ length ], /* the key, an array of uint32 values */
                                 const uint32 initval ) noexcept/* the previous hash, or an arbitrary value */
    {
        static_assert( length != 0, "The array can not be empty" );
        return hashword( &k[ 0 ], length, initval );
    }

    export constexpr void hashword2( const uint32* k,     /* the key, an array of uint32 values */
                                     uint32 length,       /* the length of the key, in uint32s */
                                     uint32* pc,          /* IN: seed OUT: primary hash value */
                                     uint32* pb ) noexcept/* IN: more seed OUT: secondary hash value */
    {
        uint32 b;
        uint32 c;

        /* Set up the internal state */
        uint32 a = b = c = 0xdeadbeef + ( length << 2 ) + *pc;
        c += *pb;

        /*------------------------------------------------- handle most of the key */
        while ( length > 3 )
        {
            a += k[ 0 ];
            b += k[ 1 ];
            c += k[ 2 ];
            mix( a, b, c );
            length -= 3;
            k += 3;
        }

        /*------------------------------------------- handle the last 3 uint32's */
        switch ( length )/* all the case statements fall through */
        {
            case 3: c += k[ 2 ];
            case 2: b += k[ 1 ];
            case 1: a += k[ 0 ];
                final( a, b, c );
            case 0: [[fallthrough]];
            default:
                /* case 0: nothing left to add */ break;
        }
        /*------------------------------------------------------ report the result */
        *pc = c;
        *pb = b;
    }

    export template<std::size_t length>                            /* the length of the key, in uint32s */
    constexpr void hashword2( const uint32 ( &k )[ length ],     /* the key, an array of uint32 values */
                              uint32 ( &pc )[ length ],          /* IN: seed OUT: primary hash value */
                              uint32 ( &pb )[ length ] ) noexcept/* IN: more seed OUT: secondary hash value */
    {
        static_assert( length != 0, "The array can not be empty" );
        return hashword2( &k[ 0 ], length, &pc[ 0 ], &pb[ 0 ] );
    }

    export constexpr uint32 hashlittle( const void* key, std::size_t length, const uint32 initval ) noexcept
    {
        uint32 b;
        uint32 c;/* internal state */

        /* Set up the internal state */
        uint32 a = b = c = 0xdeadbeef + static_cast<uint32>(length) + initval;

        if constexpr ( std::endian::native == std::endian::little )
        {
            if ( ( ptrAsInt( key ) & 0x3 ) == 0 )
            {
                const auto* k = static_cast<const uint32*>(key);/* read 32-bit chunks */

                /*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
                while ( length > 12 )
                {
                    a += k[ 0 ];
                    b += k[ 1 ];
                    c += k[ 2 ];
                    mix( a, b, c );
                    length -= 12;
                    k += 3;
                }

                /*----------------------------- handle the last (probably partial) block */
                /*
     * "k[2]&0xffffff" actually reads beyond the end of the string, but
     * then masks off the part it's not allowed to read.  Because the
     * string is aligned, the masked-off tail is in the same word as the
     * rest of the string.  Every machine with memory protection I've seen
     * does it on word boundaries, so is OK with this.  But VALGRIND will
     * still catch it and complain.  The masking trick does make the hash
     * noticably faster for short strings (like English words).
     */
                #ifndef VALGRIND

                switch ( length )
                {
                    case 12: c += k[ 2 ];
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 11: c += k[ 2 ] & 0xffffff;
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 10: c += k[ 2 ] & 0xffff;
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 9: c += k[ 2 ] & 0xff;
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 8: b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 7: b += k[ 1 ] & 0xffffff;
                        a += k[ 0 ];
                        break;
                    case 6: b += k[ 1 ] & 0xffff;
                        a += k[ 0 ];
                        break;
                    case 5: b += k[ 1 ] & 0xff;
                        a += k[ 0 ];
                        break;
                    case 4: a += k[ 0 ];
                        break;
                    case 3: a += k[ 0 ] & 0xffffff;
                        break;
                    case 2: a += k[ 0 ] & 0xffff;
                        break;
                    case 1: a += k[ 0 ] & 0xff;
                        break;
                    case 0: [[fallthrough]];
                    default: return c;/* zero length strings require no mixing */
                }

                #else /* make valgrind happy */
                const uint8* k8 = (const uint8*) k;
                switch ( length )
                {
                    case 12:
                        c += k[ 2 ];
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 11: c += ( (uint32) k8[ 10 ] ) << 16; /* fall through */
                    case 10: c += ( (uint32) k8[ 9 ] ) << 8;   /* fall through */
                    case 9: c += k8[ 8 ];                        /* fall through */
                    case 8:
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 7: b += ( (uint32) k8[ 6 ] ) << 16; /* fall through */
                    case 6: b += ( (uint32) k8[ 5 ] ) << 8;  /* fall through */
                    case 5: b += k8[ 4 ];                      /* fall through */
                    case 4: a += k[ 0 ]; break;
                    case 3: a += ( (uint32) k8[ 2 ] ) << 16; /* fall through */
                    case 2: a += ( (uint32) k8[ 1 ] ) << 8;  /* fall through */
                    case 1: a += k8[ 0 ]; break;
                    case 0: return c;
                }

                #endif /* !valgrind */
            }
            else if ( ( ptrAsInt( key ) & 0x1 ) == 0 )
            {
                const auto* k = static_cast<const uint16*>(key);/* read 16-bit chunks */

                /*--------------- all but last block: aligned reads and different mixing */
                while ( length > 12 )
                {
                    a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                    b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                    c += k[ 4 ] + ( static_cast<uint32>(k[ 5 ]) << 16 );
                    mix( a, b, c );
                    length -= 12;
                    k += 6;
                }

                /*----------------------------- handle the last (probably partial) block */
                const auto* k8 = reinterpret_cast<const uint8*>(k);
                switch ( length )
                {
                    case 12: c += k[ 4 ] + ( static_cast<uint32>(k[ 5 ]) << 16 );
                        b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 11: c += static_cast<uint32>(k8[ 10 ]) << 16;/* fall through */
                    case 10: c += k[ 4 ];
                        b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 9: c += k8[ 8 ];/* fall through */
                    case 8: b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 7: b += static_cast<uint32>(k8[ 6 ]) << 16;/* fall through */
                    case 6: b += k[ 2 ];
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 5: b += k8[ 4 ];/* fall through */
                    case 4: a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 3: a += static_cast<uint32>(k8[ 2 ]) << 16;/* fall through */
                    case 2: a += k[ 0 ];
                        break;
                    case 1: a += k8[ 0 ];
                        break;
                    case 0: [[fallthrough]];
                    default: return c;/* zero length requires no mixing */
                }
            }
        }
        else
        {
            /* need to read the key one byte at a time */
            const auto* k = static_cast<const uint8*>(key);

            /*--------------- all but the last block: affect some 32 bits of (a,b,c) */
            while ( length > 12 )
            {
                a += k[ 0 ];
                a += static_cast<uint32>(k[ 1 ]) << 8;
                a += static_cast<uint32>(k[ 2 ]) << 16;
                a += static_cast<uint32>(k[ 3 ]) << 24;
                b += k[ 4 ];
                b += static_cast<uint32>(k[ 5 ]) << 8;
                b += static_cast<uint32>(k[ 6 ]) << 16;
                b += static_cast<uint32>(k[ 7 ]) << 24;
                c += k[ 8 ];
                c += static_cast<uint32>(k[ 9 ]) << 8;
                c += static_cast<uint32>(k[ 10 ]) << 16;
                c += static_cast<uint32>(k[ 11 ]) << 24;
                mix( a, b, c );
                length -= 12;
                k += 12;
            }

            /*-------------------------------- last block: affect all 32 bits of (c) */
            switch ( length )/* all the case statements fall through */
            {
                case 12: c += static_cast<uint32>(k[ 11 ]) << 24;
                case 11: c += static_cast<uint32>(k[ 10 ]) << 16;
                case 10: c += static_cast<uint32>(k[ 9 ]) << 8;
                case 9: c += k[ 8 ];
                case 8: b += static_cast<uint32>(k[ 7 ]) << 24;
                case 7: b += static_cast<uint32>(k[ 6 ]) << 16;
                case 6: b += static_cast<uint32>(k[ 5 ]) << 8;
                case 5: b += k[ 4 ];
                case 4: a += static_cast<uint32>(k[ 3 ]) << 24;
                case 3: a += static_cast<uint32>(k[ 2 ]) << 16;
                case 2: a += static_cast<uint32>(k[ 1 ]) << 8;
                case 1: a += k[ 0 ];
                    break;
                case 0: [[fallthrough]];
                default: return c;
            }
        }

        final( a, b, c );
        return c;
    }

    /*
 * hashlittle2: return 2 32-bit hash values
 *
 * This is identical to hashlittle(), except it returns two 32-bit hash
 * values instead of just one.  This is good enough for hash table
 * lookup with 2^^64 buckets, or if you want a second hash if you're not
 * happy with the first, or if you want a probably-unique 64-bit ID for
 * the key.  *pc is better mixed than *pb, so use *pc first.  If you want
 * a 64-bit value do something like "*pc + (((uint64)*pb)<<32)".
 */
    export constexpr void hashlittle2( const void* key,       /* the key to hash */
                                       std::size_t length,    /* length of the key */
                                       uint32* pc,          /* IN: primary initval, OUT: primary hash */
                                       uint32* pb ) noexcept/* IN: secondary initval, OUT: secondary hash */
    {
        uint32 b;
        uint32 c;/* internal state */

        /* Set up the internal state */
        uint32 a = b = c = 0xdeadbeef + static_cast<uint32>(length) + *pc;
        c += *pb;

        if constexpr ( std::endian::native == std::endian::little )
        {
            if ( ( ptrAsInt( key ) & 0x3 ) == 0 )
            {
                const auto* k = static_cast<const uint32*>(key);/* read 32-bit chunks */

                /*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
                while ( length > 12 )
                {
                    a += k[ 0 ];
                    b += k[ 1 ];
                    c += k[ 2 ];
                    mix( a, b, c );
                    length -= 12;
                    k += 3;
                }

                /*----------------------------- handle the last (probably partial) block */
                /*
     * "k[2]&0xffffff" actually reads beyond the end of the string, but
     * then masks off the part it's not allowed to read.  Because the
     * string is aligned, the masked-off tail is in the same word as the
     * rest of the string.  Every machine with memory protection I've seen
     * does it on word boundaries, so is OK with this.  But VALGRIND will
     * still catch it and complain.  The masking trick does make the hash
     * noticably faster for short strings (like English words).
     */
                #ifndef VALGRIND

                switch ( length )
                {
                    case 12: c += k[ 2 ];
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 11: c += k[ 2 ] & 0xffffff;
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 10: c += k[ 2 ] & 0xffff;
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 9: c += k[ 2 ] & 0xff;
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 8: b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 7: b += k[ 1 ] & 0xffffff;
                        a += k[ 0 ];
                        break;
                    case 6: b += k[ 1 ] & 0xffff;
                        a += k[ 0 ];
                        break;
                    case 5: b += k[ 1 ] & 0xff;
                        a += k[ 0 ];
                        break;
                    case 4: a += k[ 0 ];
                        break;
                    case 3: a += k[ 0 ] & 0xffffff;
                        break;
                    case 2: a += k[ 0 ] & 0xffff;
                        break;
                    case 1: a += k[ 0 ] & 0xff;
                        break;
                    case 0: [[fallthrough]];
                    default: *pc = c;
                        *pb = b;
                        return;/* zero length strings require no mixing */
                }

                #else /* make valgrind happy */
                const uint8* k8 = (const uint8*) k;
                switch ( length )
                {
                    case 12:
                        c += k[ 2 ];
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 11: c += ( (uint32) k8[ 10 ] ) << 16; /* fall through */
                    case 10: c += ( (uint32) k8[ 9 ] ) << 8;   /* fall through */
                    case 9: c += k8[ 8 ];                        /* fall through */
                    case 8:
                        b += k[ 1 ];
                        a += k[ 0 ];
                        break;
                    case 7: b += ( (uint32) k8[ 6 ] ) << 16; /* fall through */
                    case 6: b += ( (uint32) k8[ 5 ] ) << 8;  /* fall through */
                    case 5: b += k8[ 4 ];                      /* fall through */
                    case 4: a += k[ 0 ]; break;
                    case 3: a += ( (uint32) k8[ 2 ] ) << 16; /* fall through */
                    case 2: a += ( (uint32) k8[ 1 ] ) << 8;  /* fall through */
                    case 1: a += k8[ 0 ]; break;
                    case 0:
                        *pc = c;
                        *pb = b;
                        return; /* zero length strings require no mixing */
                }

                #endif /* !valgrind */
            }
            else if ( ( ptrAsInt( key ) & 0x1 ) == 0 )
            {
                const auto* k = static_cast<const uint16*>(key);/* read 16-bit chunks */

                /*--------------- all but last block: aligned reads and different mixing */
                while ( length > 12 )
                {
                    a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                    b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                    c += k[ 4 ] + ( static_cast<uint32>(k[ 5 ]) << 16 );
                    mix( a, b, c );
                    length -= 12;
                    k += 6;
                }

                /*----------------------------- handle the last (probably partial) block */
                const auto* k8 = reinterpret_cast<const uint8*>(k);
                switch ( length )
                {
                    case 12: c += k[ 4 ] + ( static_cast<uint32>(k[ 5 ]) << 16 );
                        b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 11: c += static_cast<uint32>(k8[ 10 ]) << 16;/* fall through */
                    case 10: c += k[ 4 ];
                        b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 9: c += k8[ 8 ];/* fall through */
                    case 8: b += k[ 2 ] + ( static_cast<uint32>(k[ 3 ]) << 16 );
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 7: b += static_cast<uint32>(k8[ 6 ]) << 16;/* fall through */
                    case 6: b += k[ 2 ];
                        a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 5: b += k8[ 4 ];/* fall through */
                    case 4: a += k[ 0 ] + ( static_cast<uint32>(k[ 1 ]) << 16 );
                        break;
                    case 3: a += static_cast<uint32>(k8[ 2 ]) << 16;/* fall through */
                    case 2: a += k[ 0 ];
                        break;
                    case 1: a += k8[ 0 ];
                        break;
                    case 0: [[fallthrough]];
                    default: *pc = c;
                        *pb = b;
                        return;/* zero length strings require no mixing */
                }
            }
        }
        else
        {
            /* need to read the key one byte at a time */
            const auto* k = static_cast<const uint8*>(key);

            /*--------------- all but the last block: affect some 32 bits of (a,b,c) */
            while ( length > 12 )
            {
                a += k[ 0 ];
                a += static_cast<uint32>(k[ 1 ]) << 8;
                a += static_cast<uint32>(k[ 2 ]) << 16;
                a += static_cast<uint32>(k[ 3 ]) << 24;
                b += k[ 4 ];
                b += static_cast<uint32>(k[ 5 ]) << 8;
                b += static_cast<uint32>(k[ 6 ]) << 16;
                b += static_cast<uint32>(k[ 7 ]) << 24;
                c += k[ 8 ];
                c += static_cast<uint32>(k[ 9 ]) << 8;
                c += static_cast<uint32>(k[ 10 ]) << 16;
                c += static_cast<uint32>(k[ 11 ]) << 24;
                mix( a, b, c );
                length -= 12;
                k += 12;
            }

            /*-------------------------------- last block: affect all 32 bits of (c) */
            switch ( length )/* all the case statements fall through */
            {
                case 12: c += static_cast<uint32>(k[ 11 ]) << 24;
                case 11: c += static_cast<uint32>(k[ 10 ]) << 16;
                case 10: c += static_cast<uint32>(k[ 9 ]) << 8;
                case 9: c += k[ 8 ];
                case 8: b += static_cast<uint32>(k[ 7 ]) << 24;
                case 7: b += static_cast<uint32>(k[ 6 ]) << 16;
                case 6: b += static_cast<uint32>(k[ 5 ]) << 8;
                case 5: b += k[ 4 ];
                case 4: a += static_cast<uint32>(k[ 3 ]) << 24;
                case 3: a += static_cast<uint32>(k[ 2 ]) << 16;
                case 2: a += static_cast<uint32>(k[ 1 ]) << 8;
                case 1: a += k[ 0 ];
                    break;
                case 0: [[fallthrough]];
                default: *pc = c;
                    *pb = b;
                    return;/* zero length strings require no mixing */
            }
        }

        final( a, b, c );
        *pc = c;
        *pb = b;
    }

    /*
 * hashbig():
 * This is the same as hashword() on big-endian machines.  It is different
 * from hashlittle() on all machines.  hashbig() takes advantage of
 * big-endian byte ordering.
 */
    export constexpr uint32 hashbig( const void* key, std::size_t length, const uint32 initval ) noexcept
    {
        uint32 b;
        uint32 c;

        /* Set up the internal state */
        uint32 a = b = c = 0xdeadbeef + static_cast<uint32>(length) + initval;

        if ( std::endian::native == std::endian::little && ( ptrAsInt( key ) & 0x3U ) == 0 )
        {
            const auto* k = static_cast<const uint32*>(key);/* read 32-bit chunks */

            /*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
            while ( length > 12 )
            {
                a += k[ 0 ];
                b += k[ 1 ];
                c += k[ 2 ];
                mix( a, b, c );
                length -= 12;
                k += 3;
            }

            /*----------------------------- handle the last (probably partial) block */
            /*
     * "k[2]<<8" actually reads beyond the end of the string, but
     * then shifts out the part it's not allowed to read.  Because the
     * string is aligned, the illegal read is in the same word as the
     * rest of the string.  Every machine with memory protection I've seen
     * does it on word boundaries, so is OK with this.  But VALGRIND will
     * still catch it and complain.  The masking trick does make the hash
     * noticably faster for short strings (like English words).
     */
            #ifndef VALGRIND

            switch ( length )
            {
                case 12: c += k[ 2 ];
                    b += k[ 1 ];
                    a += k[ 0 ];
                    break;
                case 11: c += k[ 2 ] & 0xffffff00;
                    b += k[ 1 ];
                    a += k[ 0 ];
                    break;
                case 10: c += k[ 2 ] & 0xffff0000;
                    b += k[ 1 ];
                    a += k[ 0 ];
                    break;
                case 9: c += k[ 2 ] & 0xff000000;
                    b += k[ 1 ];
                    a += k[ 0 ];
                    break;
                case 8: b += k[ 1 ];
                    a += k[ 0 ];
                    break;
                case 7: b += k[ 1 ] & 0xffffff00;
                    a += k[ 0 ];
                    break;
                case 6: b += k[ 1 ] & 0xffff0000;
                    a += k[ 0 ];
                    break;
                case 5: b += k[ 1 ] & 0xff000000;
                    a += k[ 0 ];
                    break;
                case 4: a += k[ 0 ];
                    break;
                case 3: a += k[ 0 ] & 0xffffff00;
                    break;
                case 2: a += k[ 0 ] & 0xffff0000;
                    break;
                case 1: a += k[ 0 ] & 0xff000000;
                    break;
                case 0: [[fallthrough]];
                default: return c;/* zero length strings require no mixing */
            }

            #else /* make valgrind happy */
            const uint8* k8 = (const uint8*) k;
            switch ( length ) /* all the case statements fall through */
            {
                case 12:
                    c += k[ 2 ];
                    b += k[ 1 ];
                    a += k[ 0 ];
                    break;
                case 11: c += ( (uint32) k8[ 10 ] ) << 8; /* fall through */
                case 10: c += ( (uint32) k8[ 9 ] ) << 16; /* fall through */
                case 9: c += ( (uint32) k8[ 8 ] ) << 24;  /* fall through */
                case 8:
                    b += k[ 1 ];
                    a += k[ 0 ];
                    break;
                case 7: b += ( (uint32) k8[ 6 ] ) << 8;  /* fall through */
                case 6: b += ( (uint32) k8[ 5 ] ) << 16; /* fall through */
                case 5: b += ( (uint32) k8[ 4 ] ) << 24; /* fall through */
                case 4: a += k[ 0 ]; break;
                case 3: a += ( (uint32) k8[ 2 ] ) << 8;  /* fall through */
                case 2: a += ( (uint32) k8[ 1 ] ) << 16; /* fall through */
                case 1: a += ( (uint32) k8[ 0 ] ) << 24; break;
                case 0: return c;
            }

            #endif /* !VALGRIND */
        }
        else
        {
            /* need to read the key one byte at a time */
            const auto* k = static_cast<const uint8*>(key);

            /*--------------- all but the last block: affect some 32 bits of (a,b,c) */
            while ( length > 12 )
            {
                a += static_cast<uint32>(k[ 0 ]) << 24;
                a += static_cast<uint32>(k[ 1 ]) << 16;
                a += static_cast<uint32>(k[ 2 ]) << 8;
                a += static_cast<uint32>(k[ 3 ]);
                b += static_cast<uint32>(k[ 4 ]) << 24;
                b += static_cast<uint32>(k[ 5 ]) << 16;
                b += static_cast<uint32>(k[ 6 ]) << 8;
                b += static_cast<uint32>(k[ 7 ]);
                c += static_cast<uint32>(k[ 8 ]) << 24;
                c += static_cast<uint32>(k[ 9 ]) << 16;
                c += static_cast<uint32>(k[ 10 ]) << 8;
                c += static_cast<uint32>(k[ 11 ]);
                mix( a, b, c );
                length -= 12;
                k += 12;
            }

            /*-------------------------------- last block: affect all 32 bits of (c) */
            switch ( length )/* all the case statements fall through */
            {
                case 12: c += k[ 11 ];
                case 11: c += static_cast<uint32>(k[ 10 ]) << 8;
                case 10: c += static_cast<uint32>(k[ 9 ]) << 16;
                case 9: c += static_cast<uint32>(k[ 8 ]) << 24;
                case 8: b += k[ 7 ];
                case 7: b += static_cast<uint32>(k[ 6 ]) << 8;
                case 6: b += static_cast<uint32>(k[ 5 ]) << 16;
                case 5: b += static_cast<uint32>(k[ 4 ]) << 24;
                case 4: a += k[ 3 ];
                case 3: a += static_cast<uint32>(k[ 2 ]) << 8;
                case 2: a += static_cast<uint32>(k[ 1 ]) << 16;
                case 1: a += static_cast<uint32>(k[ 0 ]) << 24;
                    break;
                case 0: [[fallthrough]];
                default: return c;
            }
        }

        final( a, b, c );
        return c;
    }
    #pragma endregion
}// namespace FawnAlgebra
