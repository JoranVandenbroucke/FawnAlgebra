//
// Copyright (c) 2025.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <cstddef>

#include "geometry/bounding_sphere.hpp"
import FawnAlgebra;

using namespace FawnAlgebra;
using namespace DeerGeometry;

TEST_CASE("bounding_sphere2d grows correctly with points", "[aabb]")
{
    bounding_sphere2d<float> sphere;
    REQUIRE_THAT(sphere.origine.coord.x, Catch::Matchers::WithinAbs(std::numeric_limits<float>::max(), 1e-6f));
    REQUIRE_THAT(sphere.radius, Catch::Matchers::WithinAbs(std::numeric_limits<float>::max(), 1e-6f));

    sphere.Grow(float2{1.0f, 2.0f});
    REQUIRE_THAT(sphere.origine.coord.x, Catch::Matchers::WithinAbs(1.0f, 1e-6f));
    REQUIRE_THAT(sphere.origine.coord.y, Catch::Matchers::WithinAbs(2.0f, 1e-6f));
    REQUIRE_THAT(sphere.radius, Catch::Matchers::WithinAbs(0.0f, 1e-6f));

    sphere.Grow(float2{3.0f, 1.0f});
    REQUIRE_THAT(sphere.origine.coord.x, Catch::Matchers::WithinAbs(2.0f, 1e-6f));
    REQUIRE_THAT(sphere.origine.coord.y, Catch::Matchers::WithinAbs(1.5f, 1e-6f));
    REQUIRE_THAT(sphere.radius, Catch::Matchers::WithinAbs(1.11803398875f, 1e-6f));
}

TEST_CASE("bounding_sphere2d grows correctly with another AABB", "[aabb]")
{
    bounding_sphere2d<float> sphere1;
    bounding_sphere2d<float> sphere2;
    sphere1.Grow(float2{1.0f, 1.0f});
    sphere1.Grow(float2{3.0f, 3.0f});

    sphere2.Grow(float2{2.0f, 2.0f});
    sphere2.Grow(float2{4.0f, 4.0f});

    sphere1.Grow(sphere2);
    REQUIRE_THAT(sphere1.origine.coord.x, Catch::Matchers::WithinAbs(2.5f, 1e-6f));
    REQUIRE_THAT(sphere1.origine.coord.y, Catch::Matchers::WithinAbs(2.5f, 1e-6f));
    REQUIRE_THAT(sphere1.radius, Catch::Matchers::WithinAbs(2.121320248f, 1e-6f));
}

TEST_CASE("bounding_sphere2d calculates Area correctly", "[aabb]")
{
    bounding_sphere2d<float> sphere;
    sphere.Grow(float2{1.0f, 1.0f});
    sphere.Grow(float2{4.0f, 4.0f});
    REQUIRE_THAT(sphere.Area(), Catch::Matchers::WithinAbs(14.13716507f, 1e-6f));
}

TEST_CASE("bounding_sphere2d detects intersections correctly", "[aabb]")
{
    bounding_sphere2d<float> sphere1;
    bounding_sphere2d<float> sphere2;
    sphere1.Grow(float2{1.0f, 1.0f});
    sphere1.Grow(float2{3.0f, 3.0f});
    sphere2.Grow(float2{2.0f, 2.0f});
    sphere2.Grow(float2{4.0f, 4.0f});
    REQUIRE(sphere1.Intersect(sphere2));
    REQUIRE(bounding_sphere2d<float>::Intersect(sphere1, sphere2));
}

TEST_CASE("bounding_sphere2d detects containment correctly", "[aabb]")
{
    bounding_sphere2d<float> sphere;
    sphere.Grow(float2{1.0f, 1.0f});
    sphere.Grow(float2{4.0f, 4.0f});
    REQUIRE(sphere.Contains(float2{2.0f, 2.0f}));
    REQUIRE_FALSE(sphere.Contains(float2{5.0f, 5.0f}));
}

TEST_CASE("bounding_sphere3d grows correctly with points", "[aabb]")
{
    bounding_sphere3d<float> sphere;
    REQUIRE(sphere.origine.coord.x == std::numeric_limits<float>::max());
    REQUIRE(sphere.radius == std::numeric_limits<float>::max());

    sphere.Grow(float3{1.0f, 2.0f, 3.0f});
    REQUIRE_THAT(sphere.origine.coord.x, Catch::Matchers::WithinAbs(1.0f, 1e-6f));
    REQUIRE_THAT(sphere.origine.coord.y, Catch::Matchers::WithinAbs(2.0f, 1e-6f));
    REQUIRE_THAT(sphere.origine.coord.z, Catch::Matchers::WithinAbs(3.0f, 1e-6f));
    REQUIRE_THAT(sphere.radius, Catch::Matchers::WithinAbs(0.0f, 1e-6f));

    sphere.Grow(float3{4.0f, 1.0f, 0.0f});
    REQUIRE_THAT(sphere.origine.coord.x, Catch::Matchers::WithinAbs(2.5f, 1e-6f));
    REQUIRE_THAT(sphere.origine.coord.y, Catch::Matchers::WithinAbs(1.5f, 1e-6f));
    REQUIRE_THAT(sphere.origine.coord.z, Catch::Matchers::WithinAbs(1.5f, 1e-6f));
    REQUIRE_THAT(sphere.radius, Catch::Matchers::WithinAbs(2.179449558f, 1e-6f));
}

TEST_CASE("bounding_sphere3d grows correctly with another AABB", "[aabb]")
{
    bounding_sphere3d<float> sphere1;
    bounding_sphere3d<float> sphere2;
    sphere1.Grow(float3(1.0f));
    sphere1.Grow(float3(3.0f));

    sphere2.Grow(float3(2.0f));
    sphere2.Grow(float3(4.0f));

    sphere1.Grow(sphere2);
    REQUIRE_THAT(sphere1.origine.coord.x, Catch::Matchers::WithinAbs(2.5f, 1e-6f));
    REQUIRE_THAT(sphere1.origine.coord.y, Catch::Matchers::WithinAbs(2.5f, 1e-6f));
    REQUIRE_THAT(sphere1.origine.coord.z, Catch::Matchers::WithinAbs(2.5f, 1e-6f));
    REQUIRE_THAT(sphere1.radius, Catch::Matchers::WithinAbs(2.598076105f, 1e-6f));
}

TEST_CASE("bounding_sphere3d calculates Area correctly", "[aabb]")
{
    bounding_sphere3d<float> sphere;
    sphere.Grow(float3(1.0f));
    sphere.Grow(float3{3.0f, 4.0f, 4.0f});
    REQUIRE_THAT(sphere.Area(), Catch::Matchers::WithinAbs(54.029712677f, 1e-6f));
}

TEST_CASE("bounding_sphere3d detects intersections correctly", "[aabb]")
{
    bounding_sphere3d<float> sphere1;
    bounding_sphere3d<float> sphere2;
    sphere1.Grow(float3(1.0f));
    sphere1.Grow(float3(3.0f));
    sphere2.Grow(float3(2.0f));
    sphere2.Grow(float3(4.0f));
    REQUIRE(sphere1.Intersect(sphere2));
    REQUIRE(bounding_sphere3d<float>::Intersect(sphere1, sphere2));
}

TEST_CASE("bounding_sphere3d detects containment correctly", "[aabb]")
{
    bounding_sphere3d<float> sphere;
    sphere.Grow(float3(1.0f));
    sphere.Grow(float3(4.0f));
    REQUIRE(sphere.Contains(float3(2.0f)));
    REQUIRE_FALSE(sphere.Contains(float3(5.0f)));
}
