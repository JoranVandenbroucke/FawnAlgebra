//
// Copyright (c) 2024.
// Author: Joran.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <cstdint>
import FawnAlgebra;
using namespace FawnAlgebra;

TEST_CASE( "Vec: Vector arithmetic operations", "[Vector]" )
{
    constexpr int2 a { 1, 2 };
    constexpr int2 b { 3, 4 };

    SECTION( "Addition" )
    {
        constexpr int2 result { a + b };
        REQUIRE( result[ 0 ] == 4 );
        REQUIRE( result[ 1 ] == 6 );
    }

    SECTION( "Subtraction" )
    {
        constexpr int2 result { b - a };
        REQUIRE( result[ 0 ] == 2 );
        REQUIRE( result[ 1 ] == 2 );
    }

    SECTION( "Multiplication" )
    {
        constexpr int2 result { a * b };
        REQUIRE( result[ 0 ] == 3 );
        REQUIRE( result[ 1 ] == 8 );
    }

    SECTION( "Division" )
    {
        constexpr int2 result { b / a };
        REQUIRE( result[ 0 ] == 3 );
        REQUIRE( result[ 1 ] == 2 );
    }
    SECTION( "Dot" )
    {
        constexpr int result { b | a };
        REQUIRE( result == 11 );
    }
}

TEST_CASE( "Vec: Vector math operations", "[Vector]" )
{
    constexpr float2 a { 3.1f, 4.9f };

    SECTION( "ceil" )
    {
        const float2 squareRoot { ceil( a ) };
        REQUIRE_THAT( squareRoot[ 0 ], Catch::Matchers::WithinAbs( 2.f, 1e5f ) );
        REQUIRE_THAT( squareRoot[ 1 ], Catch::Matchers::WithinAbs( 5.f, 1e5f ) );
    }
    SECTION( "floor" )
    {
        const float2 squareRoot { floor( a ) };
        REQUIRE_THAT( squareRoot[ 0 ], Catch::Matchers::WithinAbs( 1.f, 1e5f ) );
        REQUIRE_THAT( squareRoot[ 1 ], Catch::Matchers::WithinAbs( 4.f, 1e5f ) );
    }
    SECTION( "round" )
    {
        const float2 squareRoot { round( a ) };
        REQUIRE_THAT( squareRoot[ 0 ], Catch::Matchers::WithinAbs( 1.f, 1e5f ) );
        REQUIRE_THAT( squareRoot[ 1 ], Catch::Matchers::WithinAbs( 5.f, 1e5f ) );
    }
}
