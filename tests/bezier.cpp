//
// Copyright (c) 2024.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <cstdint>
import FawnAlgebra;

using namespace FawnAlgebra;

TEST_CASE( "Bezier: Add and Remove Points", "[Bezier]" )
{
    bezierF2 bezier;
    float2 point1{1.0f, 2.0f};
    float2 point2{3.0f, 4.0f};
    float2 point3{5.0f, 6.0f};

    SECTION( "Add Points" )
    {
        addPoint( bezier, point1 );
        REQUIRE( bezier.size() == 1 );
        REQUIRE( bezier[0] == point1 );

        addPoint( bezier, point2 );
        REQUIRE( bezier.size() == 3 );
        REQUIRE( bezier[2] == point2 );

        addPoint( bezier, point3 );
        REQUIRE( bezier.size() == 5 );
        REQUIRE( bezier[4] == point3 );
    }

    SECTION( "Remove Points" )
    {
        addPoint( bezier, point1 );
        addPoint( bezier, point2 );
        addPoint( bezier, point3 );

        removePoint( bezier );
        REQUIRE( bezier.size() == 3 );
        REQUIRE( bezier[2] == point2 );

        removePoint( bezier );
        REQUIRE( bezier.size() == 1 );
        REQUIRE( bezier[0] == point1 );

        removePoint( bezier );
        REQUIRE( bezier.empty() );
    }
}

TEST_CASE( "Bezier: Evaluate", "[Bezier]" )
{
    const std::vector<float2> points{
            {0.0f, 0.0f}, {1.0f, 2.0f}, {2.0f, 0.0f},
            {3.0f, 2.0f}, {4.0f, 0.0f}
    };
    const bezierF2 bezier{points};

    SECTION( "Evaluate at t = 0.0" )
    {
        const float2 result{evaluate( bezier, 0.0f )};
        REQUIRE( result == points[0] );
    }

    SECTION( "Evaluate at t = 1.0" )
    {
        const float2 result{evaluate( bezier, 1.0f )
        };
        REQUIRE( result == points[2] );
    }

    SECTION( "Evaluate at t = 0.5" )
    {
        const float2 result{evaluate( bezier, 0.5f )};
        constexpr float2 expected{1.0f, 1.0f};// Approximate expected value
        REQUIRE_THAT( result.x(), Catch::Matchers::WithinAbs( expected.x(), 1e-5f ) );
        REQUIRE_THAT( result.y(), Catch::Matchers::WithinAbs( expected.y(), 1e-5f ) );
    }
}

TEST_CASE( "Bezier: IsClosed", "[Bezier]" )
{
    const std::vector<float2> openPoints{
            {0.0f, 0.0f}, {1.0f, 2.0f}, {2.0f, 0.0f}
    };
    const std::vector<float2> closedPoints{
            {0.0f, 0.0f}, {1.0f, 2.0f}, {2.0f, 0.0f}, {3.0f, 2.0f}
    };

    const bezierF2 openBezier{openPoints};
    const bezierF2 closedBezier{closedPoints};

    REQUIRE_FALSE( isClosed(openBezier) );
    REQUIRE( isClosed(closedBezier) );
}

TEST_CASE( "Bezier: AddPoints", "[Bezier]" )
{
    bezierF2 bezier;
    std::vector<float2> newPoints{
            {0.0f, 0.0f}, {1.0f, 2.0f}, {2.0f, 0.0f}
    };
    constexpr uint32 newSize{3};

    addPoints( bezier, newPoints );
    REQUIRE( bezier.size() == newSize );
    for ( std::size_t i = 0; i < newSize; ++i )
    {
        REQUIRE( bezier[i] == newPoints[i] );
    }
}

TEST_CASE( "Bezier: SegmentCount", "[Bezier]" )
{
    const std::vector<float2> points{
            {0.0f, 0.0f}, {1.0f, 2.0f}, {2.0f, 0.0f}, {3.0f, 2.0f}
    };
    bezierF2 bezier{points};

    REQUIRE( segmentCount(bezier) == 1 );

    addPoint( bezier, {4.0f, 0.0f} );
    REQUIRE( segmentCount(bezier) == 2 );
}
