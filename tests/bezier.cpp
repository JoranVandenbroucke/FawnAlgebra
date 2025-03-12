//
// Copyright (c) 2024.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <cstddef>

import FawnAlgebra;

using namespace FawnAlgebra;

TEST_CASE( "Bezier: Add and Remove Points", "[Bezier]" )
{
    dynamic_bezierF2 bezier;
    float2 point1 { 1.0f, 2.0f };
    float2 point2 { 3.0f, 4.0f };
    float2 point3 { 5.0f, 6.0f };

    SECTION( "Add Points" )
    {
        AddPoint( bezier, point1 );
        REQUIRE( bezier.size() == 1 );
        REQUIRE( bezier[ 0 ] == point1 );

        AddPoint( bezier, point2 );
        REQUIRE( bezier.size() == 3 );
        REQUIRE( bezier[ 2 ] == point2 );

        AddPoint( bezier, point3 );
        REQUIRE( bezier.size() == 5 );
        REQUIRE( bezier[ 4 ] == point3 );
    }

    SECTION( "Remove Points" )
    {
        AddPoint( bezier, point1 );
        AddPoint( bezier, point2 );
        AddPoint( bezier, point3 );

        RemovePoint( bezier );
        REQUIRE( bezier.size() == 3 );
        REQUIRE( bezier[ 2 ] == point2 );

        RemovePoint( bezier );
        REQUIRE( bezier.size() == 1 );
        REQUIRE( bezier[ 0 ] == point1 );

        RemovePoint( bezier );
        REQUIRE( bezier.empty() );
    }
}

TEST_CASE( "Bezier: Evaluate", "[Bezier]" )
{
    const std::vector points { float2 { 0.0f, 0.0f },
                               float2 { 1.0f, 2.0f },
                               float2 { 2.0f, 0.0f },
                               float2 { 3.0f, 2.0f },
                               float2 { 4.0f, 0.0f } };
    const dynamic_bezierF2 bezier { points };

    SECTION( "Evaluate at t = 0.0" )
    {
        const float2 result { Evaluate( bezier, 0.0f ) };
        REQUIRE( result == points[ 0 ] );
    }

    SECTION( "Evaluate at t = 1.0" )
    {
        const float2 result { Evaluate( bezier, 1.0f ) };
        REQUIRE( result == points[ 2 ] );
    }

    SECTION( "Evaluate at t = 0.5" )
    {
        const float2 result { Evaluate( bezier, 0.5f ) };
        constexpr float2 expected { 1.0f, 1.0f };// Approximate expected value
        REQUIRE_THAT( result.coord.x, Catch::Matchers::WithinAbs( expected.coord.x, 1e-5f ) );
        REQUIRE_THAT( result.coord.y, Catch::Matchers::WithinAbs( expected.coord.y, 1e-5f ) );
    }
}

TEST_CASE( "Bezier: IsClosed", "[Bezier]" )
{
    const std::vector openPoints { float2 { 0.0f, 0.0f }, float2 { 1.0f, 2.0f }, float2 { 2.0f, 0.0f } };
    const std::vector closedPoints { float2 { 0.0f, 0.0f }, float2 { 1.0f, 2.0f }, float2 { 2.0f, 0.0f }, float2 { 3.0f, 2.0f } };

    const dynamic_bezierF2 openBezier { openPoints };
    const dynamic_bezierF2 closedBezier { closedPoints };

    REQUIRE_FALSE( IsClosed( openBezier ) );
    REQUIRE( IsClosed( closedBezier ) );
}

TEST_CASE( "Bezier: AddPoints", "[Bezier]" )
{
    dynamic_bezierF2 bezier;
    std::vector newPoints { float2 { 0.0f, 0.0f }, float2 { 1.0f, 2.0f }, float2 { 2.0f, 0.0f } };
    constexpr uint32_t newSize { 3 };

    AddPoints( bezier, newPoints );
    REQUIRE( bezier.size() == newSize );
    for ( std::size_t i = 0; i < newSize; ++i )
    {
        REQUIRE( bezier[ i ] == newPoints[ i ] );
    }
}

TEST_CASE( "Bezier: SegmentCount", "[Bezier]" )
{
    const std::vector points { float2 { 0.0f, 0.0f }, float2 { 1.0f, 2.0f }, float2 { 2.0f, 0.0f }, float2 { 3.0f, 2.0f } };
    dynamic_bezierF2 bezier { points };

    REQUIRE( SegmentCount( bezier ) == 1 );

    AddPoint( bezier, float2 { 4.0f, 0.0f } );
    REQUIRE( SegmentCount( bezier ) == 2 );
}
