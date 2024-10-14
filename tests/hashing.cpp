//
// Copyright (c) 2024.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <cstdint>
import FawnAlgebra;
using namespace FawnAlgebra;

TEST_CASE( "Hash: hashDjb2 computes correct hash", "[hash]" )
{
    REQUIRE( hashDjb2( "hello" ) == 0xF923099 );
    REQUIRE( hashDjb2( "world" ) == 0x10A7356D );
}

TEST_CASE( "Hash: hashSdbm computes correct hash", "[hash]" )
{
    REQUIRE( hashSdbm( "hello" ) == 0x28D19932 );
    REQUIRE( hashSdbm( "world" ) == 0xF7E3AEB2 );
}

TEST_CASE( "Hash: hashLoseLose computes correct hash", "[hash]" )
{
    REQUIRE( hashLoseLose( "hello" ) == 0x214 );
    REQUIRE( hashLoseLose( "world" ) == 0x228 );
}

TEST_CASE( "Hash: hashword computes correct hash", "[hash]" )
{
    constexpr uint32 data[] { 1, 2, 3, 4 };
    const uint32* ptr { data };
    REQUIRE( hashword( ptr, 4, 0 ) == 0x66491246 );
}

TEST_CASE( "Hash: hashword2 computes correct hashes", "[hash]" )
{
    constexpr uint32 data[] { 1, 2, 3, 4 };
    uint32 pc {};
    uint32 pb {};
    hashword2( data, 4, &pc, &pb );
    REQUIRE( pc == 0x66491246 );
    REQUIRE( pb == 0xBF826FCA );
}

TEST_CASE( "Hash: hashword2 with array computes correct hashes", "[hash]" )
{
    constexpr uint32 data[] { 1, 2, 3, 4 };
    uint32 pc[ 4 ] {};
    uint32 pb[ 4 ] {};
    hashword2( data, pc, pb );
    REQUIRE( pc[ 0 ] == 0x66491246 );
    REQUIRE( pb[ 0 ] == 0xBF826FCA );
}

TEST_CASE( "Hash: hashlittle computes correct hash", "[hash]" )
{
    const char* data = "test data";
    REQUIRE( hashlittle( data, strlen( data ), 0 ) == 0x6458EDF1 );
}

TEST_CASE( "Hash: hashlittle2 computes correct hashes", "[hash]" )
{
    const char* data { "test data" };
    uint32 pc {};
    uint32 pb {};
    hashlittle2( data, strlen( data ), &pc, &pb );
    REQUIRE( pc == 0x6458EDF1 );// Replace with actual expected primary hash
    REQUIRE( pb == 0x5D960786 );// Replace with actual expected secondary hash
}

TEST_CASE( "Hash: hashbig computes correct hash", "[hash]" )
{
    const char* data { "test data"};
    REQUIRE( hashbig( data, strlen( data ), 0 ) == 0xb6e43300 );
}

TEST_CASE( "Float: uintToFloatExcl conversion works", "[float]" )
{
    REQUIRE( uintToFloatExcl( 0 ) == Catch::Approx( 0.0f ) );
    REQUIRE( uintToFloatExcl( 0xFFFFFFFFU ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Float: uintToFloatIncl conversion works", "[float]" )
{
    REQUIRE( uintToFloatIncl( 0 ) == Catch::Approx( 0.0f ) );
    REQUIRE( uintToFloatIncl( 0xFFFFFFFFU ) == Catch::Approx( 1.0f ) );
}
