cmake_minimum_required(VERSION 3.28)
project(FawnAlgebra VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
        PRIVATE
        includes/config/todo.hpp
        PUBLIC
        FILE_SET CXX_MODULES FILES
        includes/FawnAlgebra.ixx
        includes/arithmetics.ixx
        includes/bezier.ixx
        includes/constants.ixx
        includes/hashing.ixx
        includes/interpolation.ixx
        includes/random.ixx
        includes/statistics.ixx
        includes/trigonometric.ixx
        includes/triangulation/EarClip.ixx
        includes/triangulation/HertelMehlhorn.ixx
        includes/triangulation/Triangulation.ixx
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_options(${PROJECT_NAME}
        PRIVATE
        $<$<CONFIG:Debug>:${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT} ${DEFAULT_CXX_EXCEPTION_HANDLING}>
        $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX /MP $<$<CONFIG:Debug>:${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT} ${DEFAULT_CXX_EXCEPTION_HANDLING}>>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
        PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
        #todo -embed-dir="${CMAKE_SOURCE_DIR}/Embedded"
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>

)

add_subdirectory(tests)
