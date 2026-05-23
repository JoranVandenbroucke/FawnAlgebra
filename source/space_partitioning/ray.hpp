#pragma once

import FawnAlgebra;
import std;
using namespace fawn_algebra;

struct Intersection
{
    float t{};                // intersection distance along ray
    float2 uv{};              // barycentric coordinate of the intersection
    std::uint32_t instPrim{}; // instance index (12 bit) and primitive index (20 bit)
};

template <typename Type, std::uint8_t Dimention>
struct Ray
{
    Vec<Type, Dimention> origine{};
    Vec<Type, Dimention> direction{};
    Vec<Type, Dimention> reciprocalDirection{};
    Intersection hit{};
};

using Ray2D = Ray<float, 2>;
using Ray3D = Ray<float, 2>;
