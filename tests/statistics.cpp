//
// Copyright (c) 2024.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <utility>
#include <vector>

import FawnAlgebra;

TEST_CASE( "statistics" )
{
    constexpr std::array array1{10, 20, 30, 40, 50, 60, -70, 80, 90, 100, 0, 10, 20};
    constexpr std::array array2{15, 25, 35, 45, 55, 65, -75, 85, 95, 105, 0, 15, 25};

    SECTION( "measures of frequency" )
    {
        std::array sorted{array1};
        std::ranges::sort( sorted );
        REQUIRE( FawnAlgebra::FrequencyTable(sorted ) == std::vector<std::pair<int, std::size_t>>{{-70, 1}, {0, 1}, {10, 2}, {20, 2}, {30, 1}, {40, 1}, {50, 1}, {60, 1}, {80, 1}, {90, 1}, {100, 1}} );
        REQUIRE( FawnAlgebra::QuartilesQ1(sorted ) == 20 );
        REQUIRE( FawnAlgebra::quartilesQ2(sorted ) == 40 );
        REQUIRE( FawnAlgebra::QuartilesQ3(sorted ) == 80 );
    }
    SECTION( "central tendency" )
    {
        std::array sorted{array1};
        std::ranges::sort( sorted );
        REQUIRE( FawnAlgebra::Mean( sorted ) == 33 );
        REQUIRE( FawnAlgebra::Median( sorted ) == 30 );
        REQUIRE( FawnAlgebra::Mode( sorted ) == 10 );
        REQUIRE( FawnAlgebra::GemetricMedian( sorted ) == 0 );
    }

    SECTION( "measures of variability" )
    {
        std::array sorted{array1};
        std::ranges::sort( sorted );
        REQUIRE( FawnAlgebra::Range( sorted ) == 170 );
        REQUIRE( FawnAlgebra::InterquartileRange( sorted ) == 60 );
        REQUIRE( FawnAlgebra::VariancePopulation( sorted ) == 11 );
        REQUIRE( FawnAlgebra::VarianceSample( sorted ) == 12 );
        REQUIRE( FawnAlgebra::StandardDeviationPopulation( sorted ) == 3 );
        REQUIRE( FawnAlgebra::StandardDeviationSample( sorted ) == 3 );
    }

    SECTION( "normal distribution" )
    {
        std::array sorted{array1};
        std::ranges::sort( sorted );
        REQUIRE_THAT( FawnAlgebra::NormalProbabilityDensity( 33, 30, 11, 3 ), Catch::Matchers::WithinAbs( 0.13298076013381091, 1e-6 ) );
        REQUIRE( FawnAlgebra::NormalProbabilityDensity( sorted ) == std::vector<std::pair<int,
                 double>>{ {-70,
                 6.2208328709615639E-211},
                 {0,
                 0.000000000000000000000069720292080873287},
                 {10,
                 0.0000000000050202026130564222},
                 {10,
                 0.0000000000050202026130564222},
                 {20,
                 0.00012126275693175315},
                 {20,
                 0.00012126275693175315},
                 {30,
                 0.13298076013381091},
                 {40,
                 0.017996988837729353},
                 {50,
                 3.0058032269314661e-7},
                 {60,
                 6.195422209527299e-16},
                 {80,
                 4.9469853104677929e-45},
                 {90,
                 1.9164609361902576e-65},
                 {100,
                 3.3706565508222219e-90}} );
    }

    REQUIRE_THAT( FawnAlgebra::ZScore( 33, 30, 3 ), Catch::Matchers::WithinAbs( 1.0, 1e-6 ) );
    REQUIRE_THAT( FawnAlgebra::PoissonDistribution( 2, 0.61 ), Catch::Matchers::WithinAbs( 0.1010904292, 1e-6 ) );
    REQUIRE_THAT( FawnAlgebra::ChiSquareTest( array1, array2 ), Catch::Matchers::WithinAbs( 7, 1e-6 ) );
}
