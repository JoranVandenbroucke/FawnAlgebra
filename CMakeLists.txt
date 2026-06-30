set (CURRENT_PROJECT_NAME "FawnAlgebra")
if (NOT DEFINED PROJECT_NAME)   # standalone build
    cmake_minimum_required(VERSION 4.3)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
    set(CMAKE_CXX_MODULE_STD 1)
    project(${CURRENT_PROJECT_NAME} VERSION 0.3.0 LANGUAGES CXX)
endif()

option(FawnAlgebra_Test "If the tests need to be build to (Default OFF)" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(${CURRENT_PROJECT_NAME})
add_library(${CURRENT_PROJECT_NAME}::${CURRENT_PROJECT_NAME} ALIAS ${CURRENT_PROJECT_NAME})

set_target_properties(${CURRENT_PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${CURRENT_PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_sources(${CURRENT_PROJECT_NAME}
        PRIVATE
        FILE_SET fawnalgebra_headers TYPE HEADERS
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/source/config
        ${CMAKE_CURRENT_SOURCE_DIR}/source/geometry
        ${CMAKE_CURRENT_SOURCE_DIR}/source/space_partitioning
        FILES
        source/config/architecture.hpp
        source/config/assert.hpp
        source/config/compiler.hpp
        source/config/platform.hpp
        source/config/todo.hpp

        source/geometry/aabb.hpp
        source/geometry/bounding_sphere.hpp
        source/geometry/obb.hpp


        source/space_partitioning/bounding_volume_hierarchy.hpp
        source/space_partitioning/kdtree.hpp
        source/space_partitioning/octree.hpp
        source/space_partitioning/quadtree.hpp
        source/space_partitioning/ray.hpp

        PUBLIC
        FILE_SET fawnalgebra_modules TYPE CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        FILES
        source/arithmetics.ixx
        source/bezier.ixx
        source/constants.ixx
        source/hashing.ixx
        source/interpolation.ixx
        source/FawnAlgebra.ixx
        source/random.ixx
        source/statistics.ixx
        source/simd.ixx
        source/trigonometric.ixx
)

target_include_directories(${CURRENT_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_compile_definitions(${CURRENT_PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${CURRENT_PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${CURRENT_PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
    CXX_SCAN_FOR_MODULES  ON
)

include(compiler_select_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
set_compile_flags(${CURRENT_PROJECT_NAME})
select_best_stl(${CURRENT_PROJECT_NAME})
select_best_linker(${CURRENT_PROJECT_NAME})

if (DEFINED FawnAlgebra_Test AND FawnAlgebra_Test )
    add_subdirectory(tests)
endif ()
