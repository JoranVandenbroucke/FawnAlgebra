add_executable(${PROJECT_NAME}_test
        geometry/aabb.cpp
        geometry/bounding_sphere.cpp
        arithmetics.cpp
        bezier.cpp
        hashing.cpp
        interpolation.cpp
        random.cpp
        statistics.cpp
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE
        ${PROJECT_NAME}::${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../source)

set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(${PROJECT_NAME}_test PRIVATE $<$<CONFIG:Debug>:-DBALBINO_DEBUG>)
target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_23)

include(compiler_select_flags)
include(compiler_select_sdl)
include(compiler_select_linker)
include(setup_catch)
set_compile_flags(${PROJECT_NAME})
select_best_stl(${PROJECT_NAME}_test)
select_best_linker(${PROJECT_NAME}_test)

enable_testing()
setup_catch(${PROJECT_NAME}_test)
