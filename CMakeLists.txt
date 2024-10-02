cmake_minimum_required(VERSION 3.28)
project(FawnAlgebra VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
        PRIVATE
        includes/config/todo.hpp
        PUBLIC
        FILE_SET CXX_MODULES FILES
        includes/FawnAlgebra.ixx
        includes/aritmetics.ixx
        includes/bezier.ixx
        includes/constants.ixx
        includes/hashing.ixx
        includes/interpolation.ixx
        includes/random.ixx
        includes/statistics.ixx
        includes/trigonometric.ixx
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
target_compile_options(${PROJECT_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:GNU>:-fmodules-ts>
)

add_subdirectory(tests)
