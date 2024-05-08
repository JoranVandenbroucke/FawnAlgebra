//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//
#include <catch2/catch_all.hpp>
#include <complex>

#include "arithmetic.hpp"
#include "interpolation.hpp"

TEST_CASE( "Easing" )
{
    using namespace FawnAlgebra;
    SECTION( "float" )
    {
        constexpr float radians{pi<float>};
        constexpr float degrees{180.0f};
        REQUIRE_THAT( toRadians( degrees ), Catch::Matchers::WithinRel( radians, 1e-6f ) );
        REQUIRE_THAT( toDegree( radians ), Catch::Matchers::WithinRel( degrees, 1e-6f ) );

        REQUIRE_THAT( fade( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( fade( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( fade( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( grad( 0, 1.0f, 2.0f, 3.0f ), Catch::Matchers::WithinRel( 3.0f, 1e-6f ) );
        REQUIRE_THAT( grad( 5, 1.0f, 2.0f, 3.0f ), Catch::Matchers::WithinRel( 2.0f, 1e-6f ) );
        REQUIRE_THAT( grad( 10, 1.0f, 2.0f, 3.0f ), Catch::Matchers::WithinRel( -1.0f, 1e-6f ) );

        REQUIRE_THAT( lerp( 0.0f, 1.0f, 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( lerp( 0.0f, 1.0f, 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( lerp( 0.0f, 1.0f, 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( ilerp( 0.0f, 10.0f, 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( ilerp( 0.0f, 10.0f, 5.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( ilerp( 0.0f, 10.0f, 10.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( remap( 0.0f, 90.0f, 0.0f, 1.0f, 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( remap( 0.0f, 90.0f, 0.0f, 1.0f, 45.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( remap( 0.0f, 90.0f, 0.0f, 1.0f, 90.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( lerpAngleEuler( 0.0f, 180.0f, 0.5f ), Catch::Matchers::WithinRel( 90.0f, 1e-6f ) );
        REQUIRE_THAT( ilerpAngleEuler( 0.0f, 180.0f, 90.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( remapAngleFromValueEuler( 0.0f, 100.0f, 0.0f, 180.0f, 50.0f ), Catch::Matchers::WithinRel( 90.0f, 1e-6f ) );
        REQUIRE_THAT( remapAngleToValueEuler( 0.0f, 180.0f, 0.0f, 100.0f, 90.0f ), Catch::Matchers::WithinRel( 50.0f, 1e-6f ) );
        REQUIRE_THAT( remapAngleEuler( 0.0f, 180.0f, 180.0f, 360.0f, 90.0f ), Catch::Matchers::WithinRel( 270.0f, 1e-6f ) );

        REQUIRE_THAT( lerpAngleRad( 0.0f, pi<float>, 0.5f ), Catch::Matchers::WithinRel( pi<float> / 2.0f, 1e-6f ) );
        REQUIRE_THAT( ilerpAngleRad( 0.0f, pi<float>, pi<float> / 2.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( remapAngleFromValueRad( 0.0f, 100.0f, 0.0f, pi<float>, 50.0f ), Catch::Matchers::WithinRel( pi<float> / 2.0f, 1e-6f ) );
        REQUIRE_THAT( remapAngleToValueRad( 0.0f, pi<float>, 0.0f, 100.0f, pi<float> / 2.0f ), Catch::Matchers::WithinRel( 50.0f, 1e-6f ) );
        REQUIRE_THAT( remapAngleRad( 0.0f, pi<float>, pi<float>, two_pi<float>, pi<float> / 2.0f ), Catch::Matchers::WithinRel( (3.0f * pi<float>) / 2.0f, 1e-6f ) );

        REQUIRE_THAT( easeInSine( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInSine( 0.5f ), Catch::Matchers::WithinRel( 0.2928932188135f, 1e-6f ) );
        REQUIRE_THAT( easeInSine( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutSine( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutSine( 0.5f ), Catch::Matchers::WithinRel( 0.7071067811865f, 1e-6f ) );
        REQUIRE_THAT( easeOutSine( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutSine( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutSine( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutSine( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInQuad( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInQuad( 0.5f ), Catch::Matchers::WithinRel( 0.25f, 1e-6f ) );
        REQUIRE_THAT( easeInQuad( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutQuad( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutQuad( 0.5f ), Catch::Matchers::WithinRel( 0.75f, 1e-6f ) );
        REQUIRE_THAT( easeOutQuad( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutQuad( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutQuad( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutQuad( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInCube( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInCube( 0.5f ), Catch::Matchers::WithinRel( 0.125f, 1e-6f ) );
        REQUIRE_THAT( easeInCube( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutCube( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutCube( 0.5f ), Catch::Matchers::WithinRel( 0.875f, 1e-6f ) );
        REQUIRE_THAT( easeOutCube( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutCube( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutCube( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutCube( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInQuart( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInQuart( 0.5f ), Catch::Matchers::WithinRel( 0.0625f, 1e-6f ) );
        REQUIRE_THAT( easeInQuart( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutQuart( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutQuart( 0.5f ), Catch::Matchers::WithinRel( 0.9375f, 1e-6f ) );
        REQUIRE_THAT( easeInQuart( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutQuart( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutQuart( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutQuart( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInQuint( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInQuint( 0.5f ), Catch::Matchers::WithinRel( 0.03125f, 1e-6f ) );
        REQUIRE_THAT( easeInQuint( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutQuint( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutQuint( 0.5f ), Catch::Matchers::WithinRel( 0.96875f, 1e-6f ) );
        REQUIRE_THAT( easeOutQuint( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutQuint( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutQuint( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutQuint( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInExpo( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInExpo( 0.5f ), Catch::Matchers::WithinRel( 0.03125f, 1e-6f ) );
        REQUIRE_THAT( easeInExpo( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutExpo( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutExpo( 0.5f ), Catch::Matchers::WithinRel( 0.96875f, 1e-6f ) );
        REQUIRE_THAT( easeOutExpo( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutExpo( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutExpo( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutExpo( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInCirc( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInCirc( 0.5f ), Catch::Matchers::WithinRel( 0.13397459621556135323627682924706f, 1e-6f ) );
        REQUIRE_THAT( easeInCirc( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutCirc( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutCirc( 0.5f ), Catch::Matchers::WithinRel( 0.86602540378443864676372317075294f, 1e-6f ) );
        REQUIRE_THAT( easeOutCirc( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutCirc( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutCirc( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutCirc( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInBack( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInBack( 0.5f ), Catch::Matchers::WithinRel( -0.0876975f, 1e-6f ) );
        REQUIRE_THAT( easeInBack( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutBack( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutBack( 0.5f ), Catch::Matchers::WithinRel( 1.0876975f, 1e-6f ) );
        REQUIRE_THAT( easeOutBack( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutBack( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutBack( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutBack( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInElastic( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInElastic( 0.5f ), Catch::Matchers::WithinRel( -0.015625f, 1e-6f ) );
        REQUIRE_THAT( easeInElastic( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutElastic( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutElastic( 0.5f ), Catch::Matchers::WithinRel( 1.015625f, 1e-6f ) );
        REQUIRE_THAT( easeOutElastic( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutElastic( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutElastic( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutElastic( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInBounce( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInBounce( 0.5f ), Catch::Matchers::WithinRel( 0.234375f, 1e-6f ) );
        REQUIRE_THAT( easeInBounce( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeOutBounce( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeOutBounce( 0.5f ), Catch::Matchers::WithinRel( 0.765625f, 1e-6f ) );
        REQUIRE_THAT( easeOutBounce( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        REQUIRE_THAT( easeInOutBounce( 0.0f ), Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( easeInOutBounce( 0.5f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( easeInOutBounce( 1.0f ), Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );
    }
    BENCHMARK( "float" )
    {
        constexpr float radians{pi<float>};
        constexpr float degrees{180.0f};
        toRadians( degrees );
        toDegree( radians );

        fade( 0.0f );
        fade( 0.5f );
        fade( 1.0f );

        grad( 0, 1.0f, 2.0f, 3.0f );
        grad( 5, 1.0f, 2.0f, 3.0f );
        grad( 10, 1.0f, 2.0f, 3.0f );

        lerp( 0.0f, 1.0f, 0.0f );
        lerp( 0.0f, 1.0f, 0.5f );
        lerp( 0.0f, 1.0f, 1.0f );

        ilerp( 0.0f, 10.0f, 0.0f );
        ilerp( 0.0f, 10.0f, 5.0f );
        ilerp( 0.0f, 10.0f, 10.0f );

        remap( 0.0f, 90.0f, 0.0f, 1.0f, 0.0f );
        remap( 0.0f, 90.0f, 0.0f, 1.0f, 45.0f );
        remap( 0.0f, 90.0f, 0.0f, 1.0f, 90.0f );

        lerpAngleEuler( 0.0f, 180.0f, 0.5f );
        ilerpAngleEuler( 0.0f, 180.0f, 90.0f );
        remapAngleFromValueEuler( 0.0f, 100.0f, 0.0f, 180.0f, 50.0f );
        remapAngleToValueEuler( 0.0f, 180.0f, 0.0f, 100.0f, 90.0f );
        remapAngleEuler( 0.0f, 180.0f, 180.0f, 360.0f, 90.0f );

        lerpAngleRad( 0.0f, pi<float>, 0.5f );
        ilerpAngleRad( 0.0f, pi<float>, pi<float> / 2.0f );
        remapAngleFromValueRad( 0.0f, 100.0f, 0.0f, pi<float>, 50.0f );
        remapAngleToValueRad( 0.0f, pi<float>, 0.0f, 100.0f, pi<float> / 2.0f );
        remapAngleRad( 0.0f, pi<float>, pi<float>, two_pi<float>, pi<float> / 2.0f );

        easeInSine( 0.0f );
        easeInSine( 0.5f );
        easeInSine( 1.0f );

        easeOutSine( 0.0f );
        easeOutSine( 0.5f );
        easeOutSine( 1.0f );

        easeInOutSine( 0.0f );
        easeInOutSine( 0.5f );
        easeInOutSine( 1.0f );

        easeInQuad( 0.0f );
        easeInQuad( 0.5f );
        easeInQuad( 1.0f );

        easeOutQuad( 0.0f );
        easeOutQuad( 0.5f );
        easeOutQuad( 1.0f );

        easeInOutQuad( 0.0f );
        easeInOutQuad( 0.5f );
        easeInOutQuad( 1.0f );

        easeInCube( 0.0f );
        easeInCube( 0.5f );
        easeInCube( 1.0f );

        easeOutCube( 0.0f );
        easeOutCube( 0.5f );
        easeOutCube( 1.0f );

        easeInOutCube( 0.0f );
        easeInOutCube( 0.5f );
        easeInOutCube( 1.0f );

        easeInQuart( 0.0f );
        easeInQuart( 0.5f );
        easeInQuart( 1.0f );

        easeOutQuart( 0.0f );
        easeOutQuart( 0.5f );
        easeInQuart( 1.0f );

        easeInOutQuart( 0.0f );
        easeInOutQuart( 0.5f );
        easeInOutQuart( 1.0f );

        easeInQuint( 0.0f );
        easeInQuint( 0.5f );
        easeInQuint( 1.0f );

        easeOutQuint( 0.0f );
        easeOutQuint( 0.5f );
        easeOutQuint( 1.0f );

        easeInOutQuint( 0.0f );
        easeInOutQuint( 0.5f );
        easeInOutQuint( 1.0f );

        easeInExpo( 0.0f );
        easeInExpo( 0.5f );
        easeInExpo( 1.0f );

        easeOutExpo( 0.0f );
        easeOutExpo( 0.5f );
        easeOutExpo( 1.0f );

        easeInOutExpo( 0.0f );
        easeInOutExpo( 0.5f );
        easeInOutExpo( 1.0f );

        easeInCirc( 0.0f );
        easeInCirc( 0.5f );
        easeInCirc( 1.0f );

        easeOutCirc( 0.0f );
        easeOutCirc( 0.5f );
        easeOutCirc( 1.0f );

        easeInOutCirc( 0.0f );
        easeInOutCirc( 0.5f );
        easeInOutCirc( 1.0f );

        easeInBack( 0.0f );
        easeInBack( 0.5f );
        easeInBack( 1.0f );

        easeOutBack( 0.0f );
        easeOutBack( 0.5f );
        easeOutBack( 1.0f );

        easeInOutBack( 0.0f );
        easeInOutBack( 0.5f );
        easeInOutBack( 1.0f );

        easeInElastic( 0.0f );
        easeInElastic( 0.5f );
        easeInElastic( 1.0f );

        easeOutElastic( 0.0f );
        easeOutElastic( 0.5f );
        easeOutElastic( 1.0f );

        easeInOutElastic( 0.0f );
        easeInOutElastic( 0.5f );
        easeInOutElastic( 1.0f );

        easeInBounce( 0.0f );
        easeInBounce( 0.5f );
        easeInBounce( 1.0f );

        easeOutBounce( 0.0f );
        easeOutBounce( 0.5f );
        easeOutBounce( 1.0f );

        easeInOutBounce( 0.0f );
        easeInOutBounce( 0.5f );
        return easeInOutBounce( 1.0f );
    };

    SECTION( "double" )
    {
        constexpr double radians{pi<double>};
        constexpr double degrees{180.0};
        REQUIRE_THAT( toRadians( degrees ), Catch::Matchers::WithinRel( radians, 1e-6 ) );
        REQUIRE_THAT( toDegree( radians ), Catch::Matchers::WithinRel( degrees, 1e-6 ) );

        REQUIRE_THAT( fade( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( fade( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( fade( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( grad( 0, 1.0, 2.0, 3.0 ), Catch::Matchers::WithinRel( 3.0, 1e-6 ) );
        REQUIRE_THAT( grad( 5, 1.0, 2.0, 3.0 ), Catch::Matchers::WithinRel( 2.0, 1e-6 ) );
        REQUIRE_THAT( grad( 10, 1.0, 2.0, 3.0 ), Catch::Matchers::WithinRel( -1.0, 1e-6 ) );

        REQUIRE_THAT( lerp( 0.0, 1.0, 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( lerp( 0.0, 1.0, 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( lerp( 0.0, 1.0, 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( ilerp( 0.0, 10.0, 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( ilerp( 0.0, 10.0, 5.0 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( ilerp( 0.0, 10.0, 10.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( remap( 0.0, 90.0, 0.0, 1.0, 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( remap( 0.0, 90.0, 0.0, 1.0, 45.0 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( remap( 0.0, 90.0, 0.0, 1.0, 90.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( lerpAngleEuler( 0.0, 180.0, 0.5 ), Catch::Matchers::WithinRel( 90.0, 1e-6 ) );
        REQUIRE_THAT( ilerpAngleEuler( 0.0, 180.0, 90.0 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( remapAngleFromValueEuler( 0.0, 100.0, 0.0, 180.0, 50.0 ), Catch::Matchers::WithinRel( 90.0, 1e-6 ) );
        REQUIRE_THAT( remapAngleToValueEuler( 0.0, 180.0, 0.0, 100.0, 90.0 ), Catch::Matchers::WithinRel( 50.0, 1e-6 ) );
        REQUIRE_THAT( remapAngleEuler( 0.0, 180.0, 180.0, 360.0, 90.0 ), Catch::Matchers::WithinRel( 270.0, 1e-6 ) );

        REQUIRE_THAT( lerpAngleRad( 0.0, pi<double>, 0.5 ), Catch::Matchers::WithinRel( pi<double> / 2.0, 1e-6 ) );
        REQUIRE_THAT( ilerpAngleRad( 0.0, pi<double>, pi<double> / 2.0 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( remapAngleFromValueRad( 0.0, 100.0, 0.0, pi<double>, 50.0 ), Catch::Matchers::WithinRel( pi<double> / 2.0, 1e-6 ) );
        REQUIRE_THAT( remapAngleToValueRad( 0.0, pi<double>, 0.0, 100.0, pi<double> / 2.0 ), Catch::Matchers::WithinRel( 50.0, 1e-6 ) );
        REQUIRE_THAT( remapAngleRad( 0.0, pi<double>, pi<double>, two_pi<double>, pi<double> / 2.0 ), Catch::Matchers::WithinRel( (3.0 * pi<float>) / 2.0, 1e-6 ) );

        REQUIRE_THAT( easeInSine( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInSine( 0.5 ), Catch::Matchers::WithinRel( 0.2928932188135, 1e-6 ) );
        REQUIRE_THAT( easeInSine( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutSine( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutSine( 0.5 ), Catch::Matchers::WithinRel( 0.7071067811865, 1e-6 ) );
        REQUIRE_THAT( easeOutSine( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutSine( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutSine( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutSine( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInQuad( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInQuad( 0.5 ), Catch::Matchers::WithinRel( 0.25, 1e-6 ) );
        REQUIRE_THAT( easeInQuad( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutQuad( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutQuad( 0.5 ), Catch::Matchers::WithinRel( 0.75, 1e-6 ) );
        REQUIRE_THAT( easeOutQuad( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutQuad( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutQuad( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutQuad( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInCube( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInCube( 0.5 ), Catch::Matchers::WithinRel( 0.125, 1e-6 ) );
        REQUIRE_THAT( easeInCube( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutCube( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutCube( 0.5 ), Catch::Matchers::WithinRel( 0.875, 1e-6 ) );
        REQUIRE_THAT( easeOutCube( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutCube( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutCube( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutCube( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInQuart( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInQuart( 0.5 ), Catch::Matchers::WithinRel( 0.0625, 1e-6 ) );
        REQUIRE_THAT( easeInQuart( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutQuart( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutQuart( 0.5 ), Catch::Matchers::WithinRel( 0.9375, 1e-6 ) );
        REQUIRE_THAT( easeInQuart( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutQuart( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutQuart( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutQuart( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInQuint( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInQuint( 0.5 ), Catch::Matchers::WithinRel( 0.03125, 1e-6 ) );
        REQUIRE_THAT( easeInQuint( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutQuint( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutQuint( 0.5 ), Catch::Matchers::WithinRel( 0.96875, 1e-6 ) );
        REQUIRE_THAT( easeOutQuint( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutQuint( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutQuint( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutQuint( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInExpo( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInExpo( 0.5 ), Catch::Matchers::WithinRel( 0.03125, 1e-6 ) );
        REQUIRE_THAT( easeInExpo( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutExpo( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutExpo( 0.5 ), Catch::Matchers::WithinRel( 0.96875, 1e-6 ) );
        REQUIRE_THAT( easeOutExpo( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutExpo( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutExpo( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutExpo( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInCirc( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInCirc( 0.5 ), Catch::Matchers::WithinRel( 0.13397459621556135323627682924706, 1e-6 ) );
        REQUIRE_THAT( easeInCirc( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutCirc( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutCirc( 0.5 ), Catch::Matchers::WithinRel( 0.86602540378443864676372317075294, 1e-6 ) );
        REQUIRE_THAT( easeOutCirc( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutCirc( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutCirc( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutCirc( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInBack( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInBack( 0.5 ), Catch::Matchers::WithinRel( -0.0876975, 1e-6 ) );
        REQUIRE_THAT( easeInBack( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutBack( 0.0 ), Catch::Matchers::WithinAbs( 0.0, 1e-3 ) );
        REQUIRE_THAT( easeOutBack( 0.5 ), Catch::Matchers::WithinRel( 1.0876975, 1e-6 ) );
        REQUIRE_THAT( easeOutBack( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutBack( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutBack( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutBack( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInElastic( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInElastic( 0.5 ), Catch::Matchers::WithinRel( -0.015625, 1e-6 ) );
        REQUIRE_THAT( easeInElastic( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutElastic( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutElastic( 0.5 ), Catch::Matchers::WithinRel( 1.015625, 1e-6 ) );
        REQUIRE_THAT( easeOutElastic( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutElastic( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutElastic( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutElastic( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInBounce( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInBounce( 0.5 ), Catch::Matchers::WithinRel( 0.234375, 1e-6 ) );
        REQUIRE_THAT( easeInBounce( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeOutBounce( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeOutBounce( 0.5 ), Catch::Matchers::WithinRel( 0.765625, 1e-6 ) );
        REQUIRE_THAT( easeOutBounce( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );

        REQUIRE_THAT( easeInOutBounce( 0.0 ), Catch::Matchers::WithinRel( 0.0, 1e-6 ) );
        REQUIRE_THAT( easeInOutBounce( 0.5 ), Catch::Matchers::WithinRel( 0.5, 1e-6 ) );
        REQUIRE_THAT( easeInOutBounce( 1.0 ), Catch::Matchers::WithinRel( 1.0, 1e-6 ) );
    }
    BENCHMARK( "double" )
    {
        constexpr double radians{pi<double>};
        constexpr float degrees{180.0};
        toRadians( degrees );
        toDegree( radians );

        fade( 0.0 );
        fade( 0.5 );
        fade( 1.0 );

        grad( 0, 1.0, 2.0, 3.0 );
        grad( 5, 1.0, 2.0, 3.0 );
        grad( 10, 1.0, 2.0, 3.0 );

        lerp( 0.0, 1.0, 0.0 );
        lerp( 0.0, 1.0, 0.5 );
        lerp( 0.0, 1.0, 1.0 );

        ilerp( 0.0, 10.0, 0.0 );
        ilerp( 0.0, 10.0, 5.0 );
        ilerp( 0.0, 10.0, 10.0 );

        remap( 0.0, 90.0, 0.0, 1.0, 0.0 );
        remap( 0.0, 90.0, 0.0, 1.0, 45.0 );
        remap( 0.0, 90.0, 0.0, 1.0, 90.0 );

        lerpAngleEuler( 0.0, 180.0, 0.5 );
        ilerpAngleEuler( 0.0, 180.0, 90.0 );
        remapAngleFromValueEuler( 0.0, 100.0, 0.0, 180.0, 50.0 );
        remapAngleToValueEuler( 0.0, 180.0, 0.0, 100.0, 90.0 );
        remapAngleEuler( 0.0, 180.0, 180.0, 360.0, 90.0 );

        lerpAngleRad( 0.0, pi<double>, 0.5 );
        ilerpAngleRad( 0.0, pi<double>, pi<double> / 2.0 );
        remapAngleFromValueRad( 0.0, 100.0, 0.0, pi<double>, 50.0 );
        remapAngleToValueRad( 0.0, pi<double>, 0.0, 100.0, pi<double> / 2.0 );
        remapAngleRad( 0.0, pi<double>, pi<double>, two_pi<double>, pi<double> / 2.0 );

        easeInSine( 0.0 );
        easeInSine( 0.5 );
        easeInSine( 1.0 );

        easeOutSine( 0.0 );
        easeOutSine( 0.5 );
        easeOutSine( 1.0 );

        easeInOutSine( 0.0 );
        easeInOutSine( 0.5 );
        easeInOutSine( 1.0 );

        easeInQuad( 0.0 );
        easeInQuad( 0.5 );
        easeInQuad( 1.0 );

        easeOutQuad( 0.0 );
        easeOutQuad( 0.5 );
        easeOutQuad( 1.0 );

        easeInOutQuad( 0.0 );
        easeInOutQuad( 0.5 );
        easeInOutQuad( 1.0 );

        easeInCube( 0.0 );
        easeInCube( 0.5 );
        easeInCube( 1.0 );

        easeOutCube( 0.0 );
        easeOutCube( 0.5 );
        easeOutCube( 1.0 );

        easeInOutCube( 0.0 );
        easeInOutCube( 0.5 );
        easeInOutCube( 1.0 );

        easeInQuart( 0.0 );
        easeInQuart( 0.5 );
        easeInQuart( 1.0 );

        easeOutQuart( 0.0 );
        easeOutQuart( 0.5 );
        easeInQuart( 1.0 );

        easeInOutQuart( 0.0 );
        easeInOutQuart( 0.5 );
        easeInOutQuart( 1.0 );

        easeInQuint( 0.0 );
        easeInQuint( 0.5 );
        easeInQuint( 1.0 );

        easeOutQuint( 0.0 );
        easeOutQuint( 0.5 );
        easeOutQuint( 1.0 );

        easeInOutQuint( 0.0 );
        easeInOutQuint( 0.5 );
        easeInOutQuint( 1.0 );

        easeInExpo( 0.0 );
        easeInExpo( 0.5 );
        easeInExpo( 1.0 );

        easeOutExpo( 0.0 );
        easeOutExpo( 0.5 );
        easeOutExpo( 1.0 );

        easeInOutExpo( 0.0 );
        easeInOutExpo( 0.5 );
        easeInOutExpo( 1.0 );

        easeInCirc( 0.0 );
        easeInCirc( 0.5 );
        easeInCirc( 1.0 );

        easeOutCirc( 0.0 );
        easeOutCirc( 0.5 );
        easeOutCirc( 1.0 );

        easeInOutCirc( 0.0 );
        easeInOutCirc( 0.5 );
        easeInOutCirc( 1.0 );

        easeInBack( 0.0 );
        easeInBack( 0.5 );
        easeInBack( 1.0 );

        easeOutBack( 0.0 );
        easeOutBack( 0.5 );
        easeOutBack( 1.0 );

        easeInOutBack( 0.0 );
        easeInOutBack( 0.5 );
        easeInOutBack( 1.0 );

        easeInElastic( 0.0 );
        easeInElastic( 0.5 );
        easeInElastic( 1.0 );

        easeOutElastic( 0.0 );
        easeOutElastic( 0.5 );
        easeOutElastic( 1.0 );

        easeInOutElastic( 0.0 );
        easeInOutElastic( 0.5 );
        easeInOutElastic( 1.0 );

        easeInBounce( 0.0 );
        easeInBounce( 0.5 );
        easeInBounce( 1.0 );

        easeOutBounce( 0.0 );
        easeOutBounce( 0.5 );
        easeOutBounce( 1.0 );

        easeInOutBounce( 0.0 );
        easeInOutBounce( 0.5 );
        return easeInOutBounce( 1.0 );
    };

    SECTION( "float2 - float3" )
    {
        constexpr float2 radians{pi<float2>};
        constexpr float2 degrees{180.0f};
        REQUIRE_THAT( toRadians( degrees ).x, Catch::Matchers::WithinRel( radians.x, 1e-6f ) );
        REQUIRE_THAT( toRadians( degrees ).y, Catch::Matchers::WithinRel( radians.y, 1e-6f ) );
        REQUIRE_THAT( toDegree( radians ).x, Catch::Matchers::WithinRel( degrees.x, 1e-6f ) );
        REQUIRE_THAT( toDegree( radians ).y, Catch::Matchers::WithinRel( degrees.y, 1e-6f ) );

        constexpr float3 commonInput{0.0f, 0.5f, 1.0f};
        float3 output{fade( commonInput )};
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = lerp( float3{0.f}, float3{1.0f}, commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = ilerp( float3{0.f}, float3{10.0f}, float3{0.0f, 5.0f, 10.0f} );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = remap( float3{0.0f}, float3{90.0f}, float3{0.0f}, float3{1.0f}, float3{0.0f, 45.0f, 90.0f} );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        //        REQUIRE_THAT( lerpAngleEuler( 0.0f, 180.0f, 0.5f ), Catch::Matchers::WithinRel( 90.0f, 1e-6f ) );
        //        REQUIRE_THAT( ilerpAngleEuler( 0.0f, 180.0f, 90.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleFromValueEuler( 0.0f, 100.0f, 0.0f, 180.0f, 50.0f ), Catch::Matchers::WithinRel( 90.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleToValueEuler( 0.0f, 180.0f, 0.0f, 100.0f, 90.0f ), Catch::Matchers::WithinRel( 50.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleEuler( 0.0f, 180.0f, 180.0f, 360.0f, 90.0f ), Catch::Matchers::WithinRel( 270.0f, 1e-6f ) );
        //
        //        REQUIRE_THAT( lerpAngleRad( 0.0f, pi<float>, 0.5f ), Catch::Matchers::WithinRel( pi<float> / 2.0f, 1e-6f ) );
        //        REQUIRE_THAT( ilerpAngleRad( 0.0f, pi<float>, pi<float> / 2.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleFromValueRad( 0.0f, 100.0f, 0.0f, pi<float>, 50.0f ), Catch::Matchers::WithinRel( pi<float> / 2.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleToValueRad( 0.0f, pi<float>, 0.0f, 100.0f, pi<float> / 2.0f ), Catch::Matchers::WithinRel( 50.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleRad( 0.0f, pi<float>, pi<float>, two_pi<float>, pi<float> / 2.0f ), Catch::Matchers::WithinRel( (3.0f * pi<float>) / 2.0f, 1e-6f ) );

        output = easeInSine( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.2928932188135f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutSine( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.7071067811865f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutSine( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInQuad( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.25f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutQuad( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.75f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutQuad( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInCube( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.125f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutCube( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.875f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutCube( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInQuart( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.0625f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutQuart( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.9375f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutQuart( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInQuint( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.03125f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutQuint( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.96875f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutQuint( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInExpo( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.03125f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutQuart( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.9375f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutExpo( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInCirc( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.13397459621556135323627682924706f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutCirc( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.86602540378443864676372317075294f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutCirc( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInBack( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( -0.0876975f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutBack( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 1.0876975f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutBack( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInElastic( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( -0.015625f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutElastic( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 1.015625f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutElastic( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInBounce( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.234375f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeOutBounce( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.765625f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );

        output = easeInOutBounce( commonInput );
        REQUIRE_THAT( output.x, Catch::Matchers::WithinRel( 0.0f, 1e-6f ) );
        REQUIRE_THAT( output.y, Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        REQUIRE_THAT( output.z, Catch::Matchers::WithinRel( 1.0f, 1e-6f ) );
    }
    BENCHMARK( "float2 - float3" )
    {
        constexpr float2 radians{pi<float2>};
        constexpr float2 degrees{180.0f};
        toRadians( degrees );
        toDegree( radians );

        constexpr float3 commonInput{0.0f, 0.5f, 1.0f};
        float3 output{fade( commonInput )};
        output = lerp( float3{0.f}, float3{1.0f}, commonInput );
        output = ilerp( float3{0.f}, float3{10.0f}, float3{0.0f, 5.0f, 10.0f} );
        output = remap( float3{0.0f}, float3{90.0f}, float3{0.0f}, float3{1.0f}, float3{0.0f, 45.0f, 90.0f} );

        //        REQUIRE_THAT( lerpAngleEuler( 0.0f, 180.0f, 0.5f ), Catch::Matchers::WithinRel( 90.0f, 1e-6f ) );
        //        REQUIRE_THAT( ilerpAngleEuler( 0.0f, 180.0f, 90.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleFromValueEuler( 0.0f, 100.0f, 0.0f, 180.0f, 50.0f ), Catch::Matchers::WithinRel( 90.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleToValueEuler( 0.0f, 180.0f, 0.0f, 100.0f, 90.0f ), Catch::Matchers::WithinRel( 50.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleEuler( 0.0f, 180.0f, 180.0f, 360.0f, 90.0f ), Catch::Matchers::WithinRel( 270.0f, 1e-6f ) );
        //
        //        REQUIRE_THAT( lerpAngleRad( 0.0f, pi<float>, 0.5f ), Catch::Matchers::WithinRel( pi<float> / 2.0f, 1e-6f ) );
        //        REQUIRE_THAT( ilerpAngleRad( 0.0f, pi<float>, pi<float> / 2.0f ), Catch::Matchers::WithinRel( 0.5f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleFromValueRad( 0.0f, 100.0f, 0.0f, pi<float>, 50.0f ), Catch::Matchers::WithinRel( pi<float> / 2.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleToValueRad( 0.0f, pi<float>, 0.0f, 100.0f, pi<float> / 2.0f ), Catch::Matchers::WithinRel( 50.0f, 1e-6f ) );
        //        REQUIRE_THAT( remapAngleRad( 0.0f, pi<float>, pi<float>, two_pi<float>, pi<float> / 2.0f ), Catch::Matchers::WithinRel( (3.0f * pi<float>) / 2.0f, 1e-6f ) );

        output = easeInSine( commonInput );
        output = easeOutSine( commonInput );
        output = easeInOutSine( commonInput );
        output = easeInQuad( commonInput );
        output = easeOutQuad( commonInput );
        output = easeInOutQuad( commonInput );
        output = easeInCube( commonInput );
        output = easeOutCube( commonInput );
        output = easeInOutCube( commonInput );
        output = easeInQuart( commonInput );
        output = easeOutQuart( commonInput );
        output = easeInOutQuart( commonInput );
        output = easeInQuint( commonInput );
        output = easeOutQuint( commonInput );
        output = easeInOutQuint( commonInput );
        output = easeInExpo( commonInput );
        output = easeOutQuart( commonInput );
        output = easeInOutExpo( commonInput );
        output = easeInCirc( commonInput );
        output = easeOutCirc( commonInput );
        output = easeInOutCirc( commonInput );
        output = easeInBack( commonInput );
        output = easeOutBack( commonInput );
        output = easeInOutBack( commonInput );
        output = easeInElastic( commonInput );
        output = easeOutElastic( commonInput );
        output = easeInOutElastic( commonInput );
        output = easeInBounce( commonInput );
        output = easeOutBounce( commonInput );
        return output = easeInOutBounce( commonInput );
    };
}
