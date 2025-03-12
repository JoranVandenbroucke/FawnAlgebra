//
// Copyright (c) 2025.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <cstddef>

#include "geometry/aabb.hpp"
import FawnAlgebra;

using namespace FawnAlgebra;
using namespace DeerGeometry;

TEST_CASE("aabb2d grows correctly with points", "[aabb]")
{
    aabb2d<float> box;
    REQUIRE(box.bMin.coord.x == std::numeric_limits<float>::max());
    REQUIRE(box.bMax.coord.x == std::numeric_limits<float>::min());

    box.Grow(float2{1.0f, 2.0f});
    REQUIRE(box.bMin.coord.x == 1.0f);
    REQUIRE(box.bMin.coord.y == 2.0f);
    REQUIRE(box.bMax.coord.x == 1.0f);
    REQUIRE(box.bMax.coord.y == 2.0f);

    box.Grow(float2{3.0f, 1.0f});
    REQUIRE(box.bMin.coord.x == 1.0f);
    REQUIRE(box.bMin.coord.y == 1.0f);
    REQUIRE(box.bMax.coord.x == 3.0f);
    REQUIRE(box.bMax.coord.y == 2.0f);
}

TEST_CASE("aabb2d grows correctly with another AABB", "[aabb]")
{
    aabb2d<float> box1;
    aabb2d<float> box2;
    box1.Grow(float2{1.0f, 1.0f});
    box1.Grow(float2{3.0f, 3.0f});

    box2.Grow(float2{2.0f, 2.0f});
    box2.Grow(float2{4.0f, 4.0f});

    box1.Grow(box2);
    REQUIRE(box1.bMin.coord.x == 1.0f);
    REQUIRE(box1.bMin.coord.y == 1.0f);
    REQUIRE(box1.bMax.coord.x == 4.0f);
    REQUIRE(box1.bMax.coord.y == 4.0f);
}

TEST_CASE("aabb2d calculates Area correctly", "[aabb]")
{
    aabb2d<float> box;
    box.Grow(float2{1.0f, 1.0f});
    box.Grow(float2{3.0f, 4.0f});
    REQUIRE_THAT(box.Area(), Catch::Matchers::WithinAbs(6.0f, 1e-6f));
}

TEST_CASE("aabb2d detects intersections correctly", "[aabb]")
{
    aabb2d<float> box1;
    aabb2d<float> box2;
    box1.Grow(float2{1.0f, 1.0f});
    box1.Grow(float2{3.0f, 3.0f});
    box2.Grow(float2{2.0f, 2.0f});
    box2.Grow(float2{4.0f, 4.0f});
    REQUIRE(box1.Intersect(box2));
    REQUIRE(aabb2d<float>::Intersect(box1, box2));
}

TEST_CASE("aabb2d detects containment correctly", "[aabb]")
{
    aabb2d<float> box;
    box.Grow(float2{1.0f, 1.0f});
    box.Grow(float2{4.0f, 4.0f});
    REQUIRE(box.Contains(float2{2.0f, 2.0f}));
    REQUIRE_FALSE(box.Contains(float2{5.0f, 5.0f}));
}

TEST_CASE("aabb3d grows correctly with points", "[aabb]")
{
    aabb3d<float> box;
    REQUIRE(box.bMin.coord.x == std::numeric_limits<float>::max());
    REQUIRE(box.bMax.coord.x == std::numeric_limits<float>::min());

    box.Grow(float3{1.0f, 2.0f, 3.0f});
    REQUIRE(box.bMin.coord.x == 1.0f);
    REQUIRE(box.bMin.coord.y == 2.0f);
    REQUIRE(box.bMin.coord.z == 3.0f);
    REQUIRE(box.bMax.coord.x == 1.0f);
    REQUIRE(box.bMax.coord.y == 2.0f);
    REQUIRE(box.bMax.coord.z == 3.0f);

    box.Grow(float3{4.0f, 1.0f, 0.0f});
    REQUIRE(box.bMin.coord.x == 1.0f);
    REQUIRE(box.bMin.coord.y == 1.0f);
    REQUIRE(box.bMin.coord.z == 0.0f);
    REQUIRE(box.bMax.coord.x == 4.0f);
    REQUIRE(box.bMax.coord.y == 2.0f);
    REQUIRE(box.bMax.coord.z == 3.0f);
}

TEST_CASE("aabb3d grows correctly with another AABB", "[aabb]")
{
    aabb3d<float> box1;
    aabb3d<float> box2;
    box1.Grow(float3(1.0f));
    box1.Grow(float3(3.0f));

    box2.Grow(float3(2.0f));
    box2.Grow(float3(4.0f));

    box1.Grow(box2);
    REQUIRE(box1.bMin.coord.x == 1.0f);
    REQUIRE(box1.bMin.coord.y == 1.0f);
    REQUIRE(box1.bMin.coord.z == 1.0f);
    REQUIRE(box1.bMax.coord.x == 4.0f);
    REQUIRE(box1.bMax.coord.y == 4.0f);
    REQUIRE(box1.bMax.coord.z == 4.0f);
}

TEST_CASE("aabb3d calculates Area correctly", "[aabb]")
{
    aabb3d<float> box;
    box.Grow(float3(1.0f));
    box.Grow(float3{3.0f, 4.0f, 4.0f});
    REQUIRE_THAT(box.Area(), Catch::Matchers::WithinAbs(18.0f, 1e-6f));
}

TEST_CASE("aabb3d detects intersections correctly", "[aabb]")
{
    aabb3d<float> box1;
    aabb3d<float> box2;
    box1.Grow(float3(1.0f));
    box1.Grow(float3(3.0f));
    box2.Grow(float3(2.0f));
    box2.Grow(float3(4.0f));
    REQUIRE(box1.Intersect(box2) == true);
    REQUIRE(aabb3d<float>::Intersect(box1, box2) == true);
}

TEST_CASE("aabb3d detects containment correctly", "[aabb]")
{
    aabb3d<float> box;
    box.Grow(float3(1.0f));
    box.Grow(float3(4.0f));
    REQUIRE(box.Contains(float3(2.0f)) == true);
    REQUIRE(box.Contains(float3(5.0f)) == false);
}
