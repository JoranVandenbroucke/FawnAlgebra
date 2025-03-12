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

set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_EXTENSIONS OFF)

include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)
set(FUNCTION_NAME "get_compile_flags_${CMAKE_BUILD_TYPE}_${CMAKE_CXX_COMPILER_ID}")

# Call the dynamically constructed function
if(EXISTS ${FUNCTION_NAME})
    execute_process(COMMAND ${FUNCTION_NAME} COMPILER_FLAGS)
endif()
target_compile_options(${PROJECT_NAME}_test
        PRIVATE
            ${COMPILER_FLAGS}
)

target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../source)

include(${CMAKE_SOURCE_DIR}/cmake/setup_catch.cmake)
setup_catch(${PROJECT_NAME})
