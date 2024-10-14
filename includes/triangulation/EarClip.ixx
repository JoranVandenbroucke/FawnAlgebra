//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>
export module FawnAlgebra.Triangulation.Earclipping;
import FawnAlgebra.Arithmetics;

namespace FawnAlgebra
{
    // Cross product to determine orientation
    template<typename T, std::size_t N>
    constexpr auto cross( const Vec<T, N>& p1, const Vec<T, N>& p2, const Vec<T, N>& p3 ) noexcept
    {
        return ( p2.x() - p1.x() ) * ( p3.y() - p1.y() ) - ( p2.y() - p1.y() ) * ( p3.x() - p1.x() );
    }
    // Check if point p is inside triangle p1, p2, p3
    template<typename T, std::size_t N>
    constexpr bool isPointInTriangle( const Vec<T, N>& p1, const Vec<T, N>& p2, const Vec<T, N>& p3, const Vec<T, N>& p ) noexcept
    {
        const T cross1 { cross( p1, p2, p ) };
        const T cross2 { cross( p2, p3, p ) };
        const T cross3 { cross( p3, p1, p ) };

        // If all cross products have the same sign, p is inside the triangle
        return ( cross1 >= 0 && cross2 >= 0 && cross3 >= 0 ) || ( cross1 <= 0 && cross2 <= 0 && cross3 <= 0 );
    }

    // Check if vertex v is an "ear"
    template<typename T, std::size_t N>
    constexpr bool isEar( const std::vector<Vec<T, N>>& polygon, const std::vector<uint32_t>& indices, const int i ) noexcept
    {
        const uint32 prev { static_cast<uint32>( ( i - 1 + indices.size() ) % indices.size() ) };
        const uint32 next { static_cast<uint32>( ( i + 1 ) % indices.size() ) };

        // The ear must form a convex angle
        if ( cross( polygon[ indices[ prev ] ], polygon[ indices[ i ] ], polygon[ indices[ next ] ] ) <= 0 )
        {
            return false;
        }

        // No other points should be inside the triangle formed by prev, i, and next
        for ( std::size_t j = 0; j < indices.size(); ++j )
        {
            if ( j != prev && j != i && j != next && isPointInTriangle( polygon[ indices[ prev ] ], polygon[ indices[ i ] ], polygon[ indices[ next ] ], polygon[ indices[ j ] ] ) )
            {
                return false;
            }
        }
        return true;
    }

    export template<typename T, std::size_t N>
    constexpr auto EarClipping( const std::vector<Vec<T, N>>& points, std::vector<uint32>& indexList ) noexcept
    {
        std::vector<uint32_t> indices( points.size() );

        std::iota( indices.begin(), indices.end(), 0 );

        // Perform the ear clipping algorithm
        while ( indices.size() > 3 )
        {
            bool earFound = false;
            for ( uint i {}; i < indices.size(); ++i )
            {
                if ( IsEar( points, indices, i ) )
                {
                    const uint prev { static_cast<uint>( ( i - 1UL + indices.size() ) % indices.size() ) };
                    const uint next { static_cast<uint>( ( i + 1UL ) % indices.size() ) };

                    // Add the triangle's vertex indices to the result
                    indexList.push_back( indices[ prev ] );
                    indexList.push_back( indices[ i ] );
                    indexList.push_back( indices[ next ] );

                    // Remove the ear from the polygon's indices
                    indices.erase( indices.begin() + i );

                    earFound = true;
                    break;
                }
            }

            if ( !earFound )
            {
                std::cerr << "Error: No ear found. The polygon might be self-intersecting." << std::endl;
                break;
            }
        }

        // Add the final triangle
        if ( indices.size() == 3 )
        {
            indexList.push_back( indices[ 0 ] );
            indexList.push_back( indices[ 1 ] );
            indexList.push_back( indices[ 2 ] );
        }
    }
}// namespace FawnAlgebra
