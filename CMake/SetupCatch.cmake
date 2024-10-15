function(setup_catch target_name)
    # Predeclare Catch2_SOURCE_DIR
    set(Catch2_SOURCE_DIR "Catch2::Catch2")
    # Check if Catch2 is already a target
    if (NOT TARGET Catch2::Catch2)
        # Try to find Catch2 package
        find_package(Catch2 QUIET PATHS "${CMAKE_SOURCE_DIR}/3rdParty/Catch2")

        # If Catch2 is not found, attempt to add it from source or fetch it
        if (NOT Catch2_FOUND)
            if (EXISTS "${CMAKE_SOURCE_DIR}/3rdParty/Catch2/CMakeLists.txt")
                add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/Catch2" Catch2 EXCLUDE_FROM_ALL)
                set(Catch2_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/Catch2")
            elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Catch2/CMakeLists.txt")
                add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Catch2" Catch2 EXCLUDE_FROM_ALL)
                set(Catch2_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Catch2")
            else ()
                include(FetchContent)
                set(BUILD_TESTING OFF CACHE BOOL "When ON and the project is not used as a subproject, Catch2's test binary will be built. Defaults to ON.")
                set(CATCH_INSTALL_DOCS OFF CACHE BOOL "When ON, Catch2's documentation will be included in the installation. Defaults to ON.")
                set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "When ON, Catch2's extras folder (the CMake scripts mentioned above, debugger helpers) will be included in the installation. Defaults to ON.")
                set(CATCH_DEVELOPMENT_BUILD OFF CACHE BOOL "When ON, configures the build for development of Catch2. This means enabling test projects, warnings and so on. Defaults to OFF.")

                FetchContent_Declare(
                        Catch2
                        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                        GIT_TAG devel  # Replace with a specific tag or commit hash if needed
                        GIT_SHALLOW TRUE
                        GIT_PROGRESS TRUE
                )
                FetchContent_MakeAvailable(Catch2)
                set_property(DIRECTORY "${CMAKE_BINARY_DIR}/_deps/Catch2-src" PROPERTY EXCLUDE_FROM_ALL TRUE)
                set(Catch2_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/Catch2-src")
            endif ()
        else ()
            set(Catch2_SOURCE_DIR "${Catch2_DIR}")
        endif ()
    else ()
        get_target_property(Catch2_SOURCE_DIR Catch2::Catch2 SOURCE_DIR)
        set(Catch2_SOURCE_DIR ${Catch2_SOURCE_DIR}/..)
    endif ()

    get_target_property(Catch2_INCLUDE_DIRS Catch2::Catch2 INCLUDE_DIRECTORIES)
    # Add Catch2 as a dependency and link it to the target
    add_dependencies(${target_name}_test Catch2::Catch2)
    target_include_directories(${target_name}_test PUBLIC ${Catch2_INCLUDE_DIRS})
    target_link_libraries(${target_name}_test PUBLIC Catch2::Catch2WithMain)

    list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")

    include(CTest)
    include(Catch)
    catch_discover_tests(${target_name}_test)
endfunction()
