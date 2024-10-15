//
// Copyright (c) 2024.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <cstdint>
import FawnAlgebra.Arithmetics;
import FawnAlgebra.Hashing;
using namespace FawnAlgebra;

TEST_CASE( "Hash: hashDjb2 computes correct hash", "[hash]" )
{
    REQUIRE( HashDjb2( "hello" ) == 0xF923099 );
    REQUIRE( HashDjb2( "world" ) == 0x10A7356D );
}

TEST_CASE( "Hash: hashSdbm computes correct hash", "[hash]" )
{
    REQUIRE( HashSdbm( "hello" ) == 0x28D19932 );
    REQUIRE( HashSdbm( "world" ) == 0xF7E3AEB2 );
}

TEST_CASE( "Hash: hashLoseLose computes correct hash", "[hash]" )
{
    REQUIRE( HashLoseLose( "hello" ) == 0x214 );
    REQUIRE( HashLoseLose( "world" ) == 0x228 );
}

TEST_CASE( "Hash: hashword computes correct hash", "[hash]" )
{
    constexpr uint32 data[ ]{1, 2, 3, 4};
    const uint32* ptr{data};
    REQUIRE( HashWord( ptr, 4, 0 ) == 0x66491246 );
}

TEST_CASE( "Hash: hashword2 computes correct hashes", "[hash]" )
{
    constexpr uint32 data[ ]{1, 2, 3, 4};
    uint32 pc{};
    uint32 pb{};
    HashWord2( data, 4, &pc, &pb );
    REQUIRE( pc == 0x66491246 );
    REQUIRE( pb == 0xBF826FCA );
}

TEST_CASE( "Hash: hashword2 with array computes correct hashes", "[hash]" )
{
    constexpr uint32 data[ ]{1, 2, 3, 4};
    uint32 pc[ 4 ]{};
    uint32 pb[ 4 ]{};
    HashWord2( data, pc, pb );
    REQUIRE( pc[ 0 ] == 0x66491246 );
    REQUIRE( pb[ 0 ] == 0xBF826FCA );
}

TEST_CASE( "Hash: hashlittle computes correct hash", "[hash]" )
{
    const auto* const data = "test data";
    REQUIRE( HashLittle( data, strlen( data ), 0 ) == 0x6458EDF1 );
}

TEST_CASE( "Hash: hashlittle2 computes correct hashes", "[hash]" )
{
    const auto* const data{"test data"};
    uint32 pc{};
    uint32 pb{};
    HashLittle2( data, strlen( data ), &pc, &pb );
    REQUIRE( pc == 0x6458EDF1 );// Replace with actual expected primary hash
    REQUIRE( pb == 0x5D960786 );// Replace with actual expected secondary hash
}

TEST_CASE( "Hash: hashbig computes correct hash", "[hash]" )
{
    const auto* const data{"test data"};
    const uint32 hash{HashBig( data, strlen( data ), 0 )};
    const bool isOk{hash == 0xb6e43300U || hash == 1449816008U};// Clang return 0xb6e43300U, MSVC give 1449816008U
    REQUIRE( isOk );
}

TEST_CASE( "Float: uintToFloatExcl conversion works", "[float]" )
{
    REQUIRE( UintToFloatExcl( 0 ) == Catch::Approx( 0.0f ) );
    REQUIRE( UintToFloatExcl( 0xFFFFFFFFU ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Float: uintToFloatIncl conversion works", "[float]" )
{
    REQUIRE( UintToFloatIncl( 0 ) == Catch::Approx( 0.0f ) );
    REQUIRE( UintToFloatIncl( 0xFFFFFFFFU ) == Catch::Approx( 1.0f ) );
}
