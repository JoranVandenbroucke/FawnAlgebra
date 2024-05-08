cmake_minimum_required(VERSION 3.26)

project(FawnAlgebra VERSION 0.1.0)

add_library(FawnAlgebra INTERFACE
        includes/SpacePartitioning/aabb.hpp
        includes/arithmetic.hpp
        includes/SpacePartitioning/boundingvolumehierarchy.hpp
        includes/SpacePartitioning/quadtree.hpp
        includes/asi.hpp
        includes/constants.hpp
        includes/dekkerdouble.hpp
        includes/interpolation.hpp
        includes/hashing.hpp
        includes/random.hpp
        includes/statistics.hpp
        includes/trigonometric.hpp
        includes/SpacePartitioning/octree.hpp
)
add_library(FawnAlgebra::FawnAlgebra ALIAS FawnAlgebra)
target_include_directories(FawnAlgebra INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
        $<INSTALL_INTERFACE:include>
)

add_subdirectory(tests)