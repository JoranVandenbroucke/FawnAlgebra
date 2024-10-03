set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_executable(FawnAlgebra_test
        aritmetics.cpp
        bezier.cpp
        statistics.cpp
)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
find_package(Catch2 QUIET)

if (NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.5.4
    )
    FetchContent_MakeAvailable(Catch2)
endif ()
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

target_compile_options(FawnAlgebra_test PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

target_link_libraries(FawnAlgebra_test PRIVATE
        Catch2::Catch2WithMain
        FawnAlgebra::FawnAlgebra
)

include(CTest)
include(Catch)
catch_discover_tests(FawnAlgebra_test)
