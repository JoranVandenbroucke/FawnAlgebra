//
// Copyright (c) 2024.
// Author: Joran.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

import FawnAlgebra.Interpolation;
import FawnAlgebra.Constants;
using namespace FawnAlgebra;

TEST_CASE( "Math: toRadians", "[math]" )
{
    REQUIRE( ToRadians( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( ToRadians( 180.0f ) == Catch::Approx( 3.14159f ) );
    REQUIRE( ToRadians( 360.0f ) == Catch::Approx( 6.28318f ) );
}

TEST_CASE( "Math: toDegree", "[math]" )
{
    REQUIRE( ToDegree( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( ToDegree( 3.14159f ) == Catch::Approx( 180.0f ) );
    REQUIRE( ToDegree( 6.28318f ) == Catch::Approx( 360.0f ) );
}

TEST_CASE( "Math: fade", "[math]" )
{
    REQUIRE( Fade( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( Fade( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( Fade( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Math: grad", "[math]" )
{
    REQUIRE( Grad( 1, 0.5f, 0.5f, 0.5f ) == Catch::Approx( 0.0f ) );
}

TEST_CASE( "Math: lerp", "[math]" )
{
    REQUIRE( Lerp( 0.0f, 1.0f, 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( Lerp( 1.0f, 2.0f, 0.25f ) == Catch::Approx( 1.25f ) );
}

TEST_CASE( "Math: ilerp", "[math]" )
{
    REQUIRE( Ilerp( 0.0f, 1.0f, 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( Ilerp( 1.0f, 2.0f, 1.25f ) == Catch::Approx( 0.25f ) );
}

TEST_CASE( "Math: remap", "[math]" )
{
    REQUIRE( Remap( 0.0f, 100.0f, 0.0f, 1.0f, 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( Remap( 0.0f, 100.0f, 0.0f, 1.0f, 50.0f ) == Catch::Approx( 0.5f ) );
    REQUIRE( Remap( 0.0f, 100.0f, 0.0f, 1.0f, 100.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Math: lerpAngleEuler", "[math]" )
{
    REQUIRE( LerpAngleEuler( 0.0f, 360.0f, 0.5f ) == Catch::Approx( 180.0f ) );
}

TEST_CASE( "Math: ilerpAngleEuler", "[math]" )
{
    REQUIRE( IlerpAngleEuler( 0.0f, 360.0f, 180.0f ) == Catch::Approx( 0.5f ) );
}

TEST_CASE( "Math: remapAngleToValueEuler", "[math]" )
{
    REQUIRE( RemapAngleToValueEuler( 0.0f, 360.0f, 0.0f, 100.0f, 180.0f ) == Catch::Approx( 50.0f ) );
}

TEST_CASE( "Math: remapAngleFromValueEuler", "[math]" )
{
    REQUIRE( RemapAngleFromValueEuler( 0.0f, 100.0f, 0.0f, 360.0f, 50.0f ) == Catch::Approx( 180.0f ) );
}

TEST_CASE( "Math: remapAngleEuler", "[math]" )
{
    REQUIRE( RemapAngleEuler( 0.0f, 360.0f, 0.0f, 100.0f, 180.0f ) == Catch::Approx( 50.0f ) );
}

TEST_CASE( "Math: lerpAngleRad", "[math]" )
{
    REQUIRE( LerpAngleRad( 0.0f, FawnAlgebra::two_pi<float>, 0.5f ) == Catch::Approx( FawnAlgebra::pi<float> ) );
}

TEST_CASE( "Math: ilerpAngleRad", "[math]" )
{
    REQUIRE( IlerpAngleRad( 0.0f, FawnAlgebra::two_pi<float>, FawnAlgebra::pi<float> ) == Catch::Approx( 0.5f ) );
}

TEST_CASE( "Math: remapAngleToValueRad", "[math]" )
{
    REQUIRE( RemapAngleToValueRad( 0.0f, FawnAlgebra::two_pi<float>, 0.0f, 100.0f, FawnAlgebra::pi<float> ) == Catch::Approx( 50.0f ) );
}

TEST_CASE( "Math: remapAngleFromValueRad", "[math]" )
{
    REQUIRE( RemapAngleFromValueRad( 0.0f, 100.0f, 0.0f, FawnAlgebra::two_pi<float>, 50.0f ) == Catch::Approx( 3.14159f ) );
}

TEST_CASE( "Math: remapAngleRad", "[math]" )
{
    REQUIRE( RemapAngleRad( 0.0f, FawnAlgebra::two_pi<float>, 0.0f, 100.0f, FawnAlgebra::pi<float>) == Catch::Approx( -0.265483737f ) );
}

TEST_CASE( "Math: exponentialDecay", "[math]" )
{
    REQUIRE( ExponentialDecay( 0.0f, 1.0f, 1.0f, 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( ExponentialDecay( 0.0f, 1.0f, 1.0f, 0.5f ) == Catch::Approx( 0.393469334f ) );
    REQUIRE( ExponentialDecay( 0.0f, 1.0f, 1.0f, 1.0f ) == Catch::Approx( 0.63212055f ) );
}

TEST_CASE( "Easing: EaseInSine", "[easing]" )
{
    REQUIRE( EaseInSine( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInSine( 0.5f ) == Catch::Approx( 0.292893231f ) );
    REQUIRE( EaseInSine( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutSine", "[easing]" )
{
    REQUIRE( EaseOutSine( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutSine( 0.5f ) == Catch::Approx( 0.7071f ) );
    REQUIRE( EaseOutSine( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutSine", "[easing]" )
{
    REQUIRE( EaseInOutSine( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutSine( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutSine( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInQuad", "[easing]" )
{
    REQUIRE( EaseInQuad( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInQuad( 0.5f ) == Catch::Approx( 0.25f ) );
    REQUIRE( EaseInQuad( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutQuad", "[easing]" )
{
    REQUIRE( EaseOutQuad( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutQuad( 0.5f ) == Catch::Approx( 0.75f ) );
    REQUIRE( EaseOutQuad( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutQuad", "[easing]" )
{
    REQUIRE( EaseInOutQuad( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutQuad( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutQuad( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInCubic", "[easing]" )
{
    REQUIRE( EaseInCube( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInCube( 0.5f ) == Catch::Approx( 0.125f ) );
    REQUIRE( EaseInCube( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutCubic", "[easing]" )
{
    REQUIRE( EaseOutCube( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutCube( 0.5f ) == Catch::Approx( 0.875f ) );
    REQUIRE( EaseOutCube( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutCubic", "[easing]" )
{
    REQUIRE( EaseInOutCube( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutCube( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutCube( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInQuart", "[easing]" )
{
    REQUIRE( EaseInQuart( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInQuart( 0.5f ) == Catch::Approx( 0.0625f ) );
    REQUIRE( EaseInQuart( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutQuart", "[easing]" )
{
    REQUIRE( EaseOutQuart( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutQuart( 0.5f ) == Catch::Approx( 0.9375f ) );
    REQUIRE( EaseOutQuart( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutQuart", "[easing]" )
{
    REQUIRE( EaseInOutQuart( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutQuart( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutQuart( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInQuint", "[easing]" )
{
    REQUIRE( EaseInQuint( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInQuint( 0.5f ) == Catch::Approx( 0.03125f ) );
    REQUIRE( EaseInQuint( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutQuint", "[easing]" )
{
    REQUIRE( EaseOutQuint( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutQuint( 0.5f ) == Catch::Approx( 0.96875f ) );
    REQUIRE( EaseOutQuint( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutQuint", "[easing]" )
{
    REQUIRE( EaseInOutQuint( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutQuint( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutQuint( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInExpo", "[easing]" )
{
    REQUIRE( EaseInExpo( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInExpo( 0.5f ) == Catch::Approx( 0.03125f ) );
    REQUIRE( EaseInExpo( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutExpo", "[easing]" )
{
    REQUIRE( EaseOutExpo( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutExpo( 0.5f ) == Catch::Approx( 0.96875f ) );
    REQUIRE( EaseOutExpo( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutExpo", "[easing]" )
{
    REQUIRE( EaseInOutExpo( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutExpo( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutExpo( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInCirc", "[easing]" )
{
    REQUIRE( EaseInCirc( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInCirc( 0.5f ) == Catch::Approx( 0.133974612f ) );
    REQUIRE( EaseInCirc( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutCirc", "[easing]" )
{
    REQUIRE( EaseOutCirc( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutCirc( 0.5f ) == Catch::Approx( 0.86602f ) );
    REQUIRE( EaseOutCirc( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutCirc", "[easing]" )
{
    REQUIRE( EaseInOutCirc( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutCirc( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutCirc( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInBack", "[easing]" )
{
    REQUIRE( EaseInBack( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInBack( 0.5f ) == Catch::Approx( -0.087697506f ) );
    REQUIRE( EaseInBack( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutBack", "[easing]" )
{
    REQUIRE( EaseOutBack( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutBack( 0.5f ) == Catch::Approx( 1.087697506f ) );
    REQUIRE( EaseOutBack( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutBack", "[easing]" )
{
    REQUIRE( EaseInOutBack( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutBack( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutBack( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInElastic", "[easing]" )
{
    REQUIRE( EaseInElastic( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInElastic( 0.5f ) == Catch::Approx( -0.015624988f ) );
    REQUIRE( EaseInElastic( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutElastic", "[easing]" )
{
    REQUIRE( EaseOutElastic( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutElastic( 0.5f ) == Catch::Approx( 1.015625f ) );
    REQUIRE( EaseOutElastic( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutElastic", "[easing]" )
{
    REQUIRE( EaseInOutElastic( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutElastic( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutElastic( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInBounce", "[easing]" )
{
    REQUIRE( EaseInBounce( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInBounce( 0.5f ) == Catch::Approx( 0.234375f ) );
    REQUIRE( EaseInBounce( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseOutBounce", "[easing]" )
{
    REQUIRE( EaseOutBounce( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseOutBounce( 0.5f ) == Catch::Approx( 0.76562f ) );
    REQUIRE( EaseOutBounce( 1.0f ) == Catch::Approx( 1.0f ) );
}

TEST_CASE( "Easing: EaseInOutBounce", "[easing]" )
{
    REQUIRE( EaseInOutBounce( 0.0f ) == Catch::Approx( 0.0f ) );
    REQUIRE( EaseInOutBounce( 0.5f ) == Catch::Approx( 0.5f ) );
    REQUIRE( EaseInOutBounce( 1.0f ) == Catch::Approx( 1.0f ) );
}
