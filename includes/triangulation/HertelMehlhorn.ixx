//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>
export module FawnAlgebra.Triangulation.HertelMehlhorn;
import FawnAlgebra.Arithmetics;

namespace FawnAlgebra
{
    /**
     * procedure TRIANGULATE:
     * begin
     *   X+-n given points, sorted by increasing x coordinate;
     *   Y+- {-0% oo}; type ([-o% ov])*-out;
     *   C~- ~;
     *   TRI ~- ;2~;
     *   EDGES ~-n polygon edges, given in counterclockwise order;
     *   while x ¢ ~3 do
     *     p +- MIN(X);
     *     TRANSITION(p)
     *   od
     * end {of TRIANGULATE }.
     */
//    template<typename T, std::size_t N>
//    constexpr auto HertelMehlhorn_Triangulate( const std::vector<Vec<T, N>>& points, std::vector<uint32>& indexList )
//    {
//    std::vector<uint32> indices(points.size());
//    std::iota( indices.begin(), indices.end(), 0 );
//    std::ranges::sort( indices,
//    [ &points ]( uint32 a, uint32 b )
//    {
//    return points[ a ].x() < points[ b ].x();
//    } );
//    float2 y{-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};

//    }

//    export template<typename T, std::size_t N>
//    constexpr auto HertelMehlhorn( const std::vector<Vec<T, N>>& points, std::vector<uint32>& indexList )
//    {
//    }
}