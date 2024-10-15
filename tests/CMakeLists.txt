set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}_test
        arithmetics.cpp
        bezier.cpp
        hashing.cpp
        interpolation.cpp
        random.cpp
        statistics.cpp
)

target_compile_options(${PROJECT_NAME}_test PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX /MP $<$<CONFIG:Debug>:${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT} ${DEFAULT_CXX_EXCEPTION_HANDLING}>>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE
        ${PROJECT_NAME}::${PROJECT_NAME}
)

include(${CMAKE_SOURCE_DIR}/CMake/SetupCatch.cmake)
setup_catch(${PROJECT_NAME})
