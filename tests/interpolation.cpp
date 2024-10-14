//
// Copyright (c) 2024.
// Author: Joran.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

import FawnAlgebra;
using namespace FawnAlgebra;
TEST_CASE( "Math: toRadians", "[math]" )
{
    REQUIRE( toRadians( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( toRadians( 180.0f ) == Catch::Approx( 3.14159f ) );
    REQUIRE( toRadians( 360.0f ) == Catch::Approx( 6.28318f ) );
}

TEST_CASE( "Math: toDegree", "[math]" )
{
    REQUIRE( toDegree( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( toDegree( 3.14159f ) == Catch::Approx( 180.0f ) );
    REQUIRE( toDegree( 6.28318f ) == Catch::Approx( 360.0f ) );
}

TEST_CASE( "Math: fade", "[math]" )
{
    REQUIRE( fade( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( fade( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( fade( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Math: grad", "[math]" )
{
    REQUIRE( grad( 1, 0.5f, 0.5f, 0.5f ) == Catch::Approx( 0.0f ) );
}

TEST_CASE( "Math: lerp", "[math]" )
{
    REQUIRE( lerp( 0.0f, 1.0f, 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( lerp( 1.0f, 2.0f, 0.25f ) == Catch::Approx( 1.25f ) );
}

TEST_CASE( "Math: ilerp", "[math]" )
{
    REQUIRE( ilerp( 0.0f, 1.0f, 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( ilerp( 1.0f, 2.0f, 1.25f ) == Catch::Approx( 0.25f ) );
}

TEST_CASE( "Math: remap", "[math]" )
{
    REQUIRE( remap( 0.0f, 100.0f, 0.0f, 1.0f, 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( remap( 0.0f, 100.0f, 0.0f, 1.0f, 50.0f ) == Catch::Approx( 0.5f ) );
    REQUIRE( remap( 0.0f, 100.0f, 0.0f, 1.0f, 100.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Math: lerpAngleEuler", "[math]" )
{
    REQUIRE( lerpAngleEuler( 0.0f, 360.0f, 0.5f ) == Catch::Approx( 180.0f ) );
}

TEST_CASE( "Math: ilerpAngleEuler", "[math]" )
{
    REQUIRE( ilerpAngleEuler( 0.0f, 360.0f, 180.0f ) == Catch::Approx( 0.5f ) );
}

TEST_CASE( "Math: remapAngleToValueEuler", "[math]" )
{
    REQUIRE( remapAngleToValueEuler( 0.0f, 360.0f, 0.0f, 100.0f, 180.0f ) == Catch::Approx( 50.0f ) );
}

TEST_CASE( "Math: remapAngleFromValueEuler", "[math]" )
{
    REQUIRE( remapAngleFromValueEuler( 0.0f, 100.0f, 0.0f, 360.0f, 50.0f ) == Catch::Approx( 180.0f ) );
}

TEST_CASE( "Math: remapAngleEuler", "[math]" )
{
    REQUIRE( remapAngleEuler( 0.0f, 360.0f, 0.0f, 100.0f, 180.0f ) == Catch::Approx( 50.0f ) );
}

TEST_CASE( "Math: lerpAngleRad", "[math]" )
{
    REQUIRE( lerpAngleRad( 0.0f, FawnAlgebra::two_pi<float>, 0.5f ) == Catch::Approx( FawnAlgebra::pi<float> ) );
}

TEST_CASE( "Math: ilerpAngleRad", "[math]" )
{
    REQUIRE( ilerpAngleRad( 0.0f, FawnAlgebra::two_pi<float>, FawnAlgebra::pi<float> ) == Catch::Approx( 0.5f ) );
}

TEST_CASE( "Math: remapAngleToValueRad", "[math]" )
{
    REQUIRE( remapAngleToValueRad( 0.0f, FawnAlgebra::two_pi<float>, 0.0f, 100.0f, FawnAlgebra::pi<float> ) == Catch::Approx( 50.0f ) );
}

TEST_CASE( "Math: remapAngleFromValueRad", "[math]" )
{
    REQUIRE( remapAngleFromValueRad( 0.0f, 100.0f, 0.0f, FawnAlgebra::two_pi<float>, 50.0f ) == Catch::Approx( 3.14159f ) );
}

TEST_CASE( "Math: remapAngleRad", "[math]" )
{
    REQUIRE( remapAngleRad( 0.0f, FawnAlgebra::two_pi<float>, 0.0f, 100.0f,  FawnAlgebra::pi<float>) == Catch::Approx( -0.265483737f ) );
}

TEST_CASE( "Math: exponentialDecay", "[math]" )
{
    REQUIRE( exponentialDecay( 0.0f, 1.0f, 1.0f, 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( exponentialDecay( 0.0f, 1.0f, 1.0f, 0.5f ) == Catch::Approx( 0.393469334f ) );
    REQUIRE( exponentialDecay( 0.0f, 1.0f, 1.0f, 1.0f ) == Catch::Approx( 0.63212055f ) );
}

TEST_CASE( "Easing: easeInSine", "[easing]" )
{
    REQUIRE( easeInSine( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInSine( 0.5f ) == Catch::Approx( 0.292893231f ) );
    REQUIRE( easeInSine( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutSine", "[easing]" )
{
    REQUIRE( easeOutSine( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutSine( 0.5f ) == Catch::Approx( 0.7071f ) );
    REQUIRE( easeOutSine( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutSine", "[easing]" )
{
    REQUIRE( easeInOutSine( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutSine( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutSine( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInQuad", "[easing]" )
{
    REQUIRE( easeInQuad( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInQuad( 0.5f ) == Catch::Approx( 0.25f ) );
    REQUIRE( easeInQuad( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutQuad", "[easing]" )
{
    REQUIRE( easeOutQuad( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutQuad( 0.5f ) == Catch::Approx( 0.75f ) );
    REQUIRE( easeOutQuad( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutQuad", "[easing]" )
{
    REQUIRE( easeInOutQuad( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutQuad( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutQuad( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInCubic", "[easing]" )
{
    REQUIRE( easeInCube( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInCube( 0.5f ) == Catch::Approx( 0.125f ) );
    REQUIRE( easeInCube( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutCubic", "[easing]" )
{
    REQUIRE( easeOutCube( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutCube( 0.5f ) == Catch::Approx( 0.875f ) );
    REQUIRE( easeOutCube( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutCubic", "[easing]" )
{
    REQUIRE( easeInOutCube( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutCube( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutCube( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInQuart", "[easing]" )
{
    REQUIRE( easeInQuart( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInQuart( 0.5f ) == Catch::Approx( 0.0625f ) );
    REQUIRE( easeInQuart( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutQuart", "[easing]" )
{
    REQUIRE( easeOutQuart( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutQuart( 0.5f ) == Catch::Approx( 0.9375f ) );
    REQUIRE( easeOutQuart( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutQuart", "[easing]" )
{
    REQUIRE( easeInOutQuart( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutQuart( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutQuart( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInQuint", "[easing]" )
{
    REQUIRE( easeInQuint( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInQuint( 0.5f ) == Catch::Approx( 0.03125f ) );
    REQUIRE( easeInQuint( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutQuint", "[easing]" )
{
    REQUIRE( easeOutQuint( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutQuint( 0.5f ) == Catch::Approx( 0.96875f ) );
    REQUIRE( easeOutQuint( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutQuint", "[easing]" )
{
    REQUIRE( easeInOutQuint( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutQuint( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutQuint( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInExpo", "[easing]" )
{
    REQUIRE( easeInExpo( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInExpo( 0.5f ) == Catch::Approx( 0.03125f ) );
    REQUIRE( easeInExpo( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutExpo", "[easing]" )
{
    REQUIRE( easeOutExpo( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutExpo( 0.5f ) == Catch::Approx( 0.96875f ) );
    REQUIRE( easeOutExpo( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutExpo", "[easing]" )
{
    REQUIRE( easeInOutExpo( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutExpo( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutExpo( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInCirc", "[easing]" )
{
    REQUIRE( easeInCirc( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInCirc( 0.5f ) == Catch::Approx( 0.133974612f ) );
    REQUIRE( easeInCirc( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutCirc", "[easing]" )
{
    REQUIRE( easeOutCirc( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutCirc( 0.5f ) == Catch::Approx( 0.86602f ) );
    REQUIRE( easeOutCirc( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutCirc", "[easing]" )
{
    REQUIRE( easeInOutCirc( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutCirc( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutCirc( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInBack", "[easing]" )
{
    REQUIRE( easeInBack( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInBack( 0.5f ) == Catch::Approx( -0.087697506f ) );
    REQUIRE( easeInBack( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutBack", "[easing]" )
{
    REQUIRE( easeOutBack( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutBack( 0.5f ) == Catch::Approx( 1.087697506f ) );
    REQUIRE( easeOutBack( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutBack", "[easing]" )
{
    REQUIRE( easeInOutBack( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutBack( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutBack( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInElastic", "[easing]" )
{
    REQUIRE( easeInElastic( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInElastic( 0.5f ) == Catch::Approx( -0.015624988f ) );
    REQUIRE( easeInElastic( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutElastic", "[easing]" )
{
    REQUIRE( easeOutElastic( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutElastic( 0.5f ) == Catch::Approx( 1.015625f ) );
    REQUIRE( easeOutElastic( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutElastic", "[easing]" )
{
    REQUIRE( easeInOutElastic( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutElastic( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutElastic( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInBounce", "[easing]" )
{
    REQUIRE( easeInBounce( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInBounce( 0.5f ) == Catch::Approx( 0.234375f ) );
    REQUIRE( easeInBounce( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeOutBounce", "[easing]" )
{
    REQUIRE( easeOutBounce( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeOutBounce( 0.5f ) == Catch::Approx( 0.76562f ) );
    REQUIRE( easeOutBounce( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: easeInOutBounce", "[easing]" )
{
    REQUIRE( easeInOutBounce( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( easeInOutBounce( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( easeInOutBounce( 1.0f ) == Catch::Approx( 1.0f ) );
}
