//
// Copyright (c) 2024.
// Author: Joran.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

import FawnAlgebra;
using namespace FawnAlgebra;

TEST_CASE( "Random: next", "[random]" )
{
    Random rng( 123 );
    REQUIRE( rng.next() == 48074 );
    REQUIRE( rng.next() == 30889 );
}

TEST_CASE( "Random: next with range", "[random]" )
{
    Random rng( 123 );
    constexpr uint32_t min { 10 };
    constexpr uint32_t max { 20 };
    const uint32_t value { rng.next( min, max ) };
    REQUIRE( value >= min );
    REQUIRE( value < max );
}

TEST_CASE( "Random: next64", "[random]" )
{
    Random rng( 123 );
    const uint64_t value { rng.next64() };
    REQUIRE( value == 206476257818793 );
}

TEST_CASE( "Random: next64 with range", "[random]" )
{
    Random rng( 123 );
    constexpr uint64_t min { 100 };
    constexpr uint64_t max { 200 };
    const uint64_t value { rng.next64( min, max ) };
    REQUIRE( value >= min );
    REQUIRE( value < max );
}

TEST_CASE( "Random: nextDouble", "[random]" )
{
    Random rng( 123 );
    const double value { rng.nextDouble() };
    REQUIRE( value == Catch::Approx( 3150625739.20489835739135742 ) );
}

TEST_CASE( "Random: nextSingle", "[random]" )
{
    Random rng( 123 );
    const float value { rng.nextSingle() };
    REQUIRE( value == Catch::Approx( 0.733562231f ) );
}

TEST_CASE( "Random: nextBytes", "[random]" )
{
    Random rng( 123 );
    uint32_t data[ 4 ];
    rng.nextBytes( data );
    REQUIRE( data[ 0 ] == 48074 );
    REQUIRE( data[ 1 ] == 30889 );
    REQUIRE( data[ 2 ] == 4921 );
    REQUIRE( data[ 3 ] == 39526 );
}
