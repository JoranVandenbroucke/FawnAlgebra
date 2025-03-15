cmake_minimum_required(VERSION 3.28)
project(FawnAlgebra VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_SCAN_FOR_MODULES ON)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

option(FawnAlgebra_Test "If the tests need to be build to (Default OFF)" ON)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_sources(${PROJECT_NAME}
        PRIVATE
        FILE_SET fawnalgebra_config TYPE HEADERS
        BASE_DIRS
        ${PROJECT_SOURCE_DIR}/source/config
        FILES
        source/config/architecture.hpp
        source/config/assert.hpp
        source/config/compiler.hpp
        source/config/platform.hpp
        source/config/todo.hpp

        FILE_SET fawnalgebra_geometry TYPE HEADERS
        BASE_DIRS
        ${PROJECT_SOURCE_DIR}/source/geometry
        FILES
        source/geometry/aabb.hpp
        source/geometry/bounding_sphere.hpp
        source/geometry/obb.hpp

        FILE_SET fawnalgebra_space_partitioning TYPE HEADERS
        BASE_DIRS
        ${PROJECT_SOURCE_DIR}/source/space_partitioning
        FILES
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
#        source/simd.ixx
        source/trigonometric.ixx
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(compiler_flags)
set(FUNCTION_NAME "get_compile_flags_${CMAKE_BUILD_TYPE}_${CMAKE_CXX_COMPILER_ID}")
if(EXISTS ${FUNCTION_NAME})
    execute_process(COMMAND ${FUNCTION_NAME} COMPILER_FLAGS)
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_FLAGS})

target_link_options(${PROJECT_NAME}
        PRIVATE
        $<$<CXX_COMPILER_ID:Clang>: $<$<BOOL:${USE_LLD}>:-fuse-ld=lld>>>
        $<$NOT<$<BOOL:${USE_LLD}>>:
            $<$<CXX_COMPILER_ID:MSVC>:
                /Gw /Gy /GL
            >
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
                -flto=full
            >
        >
)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

if (DEFINED FawnAlgebra_Test AND FawnAlgebra_Test )
    add_subdirectory(tests)
endif ()
