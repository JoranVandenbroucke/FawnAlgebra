cmake_minimum_required(VERSION 4.3.0)
set(CMAKE_CXX_COMPILER_IMPORT_STD ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
project(FawnAlgebra VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_MODULE_STD 1)

option(FawnAlgebra_Test "If the tests need to be build to (Default OFF)" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_sources(${PROJECT_NAME}
        PRIVATE
        FILE_SET fawnalgebra_headers TYPE HEADERS
        BASE_DIRS
        ${PROJECT_SOURCE_DIR}/source/config
        ${PROJECT_SOURCE_DIR}/source/geometry
        ${PROJECT_SOURCE_DIR}/source/space_partitioning
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
        ${PROJECT_SOURCE_DIR}/source
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

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/source)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
    CXX_SCAN_FOR_MODULES  ON
)

include(compiler_select_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
set_compile_flags(${PROJECT_NAME})
select_best_stl(${PROJECT_NAME})
select_best_linker(${PROJECT_NAME})

if (DEFINED FawnAlgebra_Test AND FawnAlgebra_Test )
    add_subdirectory(tests)
endif ()
