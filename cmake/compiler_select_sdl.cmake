function(select_best_stl target)
    message(STATUS "Configuring STL for target: ${target}")
    message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Detect the target triple (helps us know if we’re on MSVC or GNU ABI)
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -print-target-triple
            OUTPUT_VARIABLE CLANG_TRIPLE
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Detected Clang target triple: ${CLANG_TRIPLE}")

        # If targeting MSVC ABI, don’t try to force libc++
        if (CLANG_TRIPLE MATCHES "-msvc")
            message(STATUS "ℹ️ Clang using MSVC ABI — using MSVC STL")
            return()
        endif()

        include(CheckCXXSourceCompiles)
        set(CMAKE_REQUIRED_FLAGS "-stdlib=libc++")
        check_cxx_source_compiles("#include <vector>\nint main(){std::vector<int> v;}" HAS_LIBCXX)

        if (HAS_LIBCXX)
            message(STATUS "✅ libc++ detected — using it for Clang")

            # Detect headers
            execute_process(
                COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=include/c++/v1
                OUTPUT_VARIABLE LIBCXX_INCLUDE
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            if (EXISTS "${LIBCXX_INCLUDE}")
                message(STATUS "ℹ️ Found libc++ headers at: ${LIBCXX_INCLUDE}")
                target_include_directories(${target} PRIVATE "${LIBCXX_INCLUDE}")
            endif()

            # Detect libraries
            execute_process(
                COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libc++.a
                OUTPUT_VARIABLE LIBCXX_LIB
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            execute_process(
                COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libc++abi.a
                OUTPUT_VARIABLE LIBCXXABI_LIB
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )

            if (EXISTS "${LIBCXX_LIB}")
                target_link_libraries(${target} PRIVATE "${LIBCXX_LIB}")
                target_link_options(${target} PRIVATE -stdlib=libc++)
            endif()
            if (EXISTS "${LIBCXXABI_LIB}")
                target_link_libraries(${target} PRIVATE "${LIBCXXABI_LIB}")
            endif()

        else()
            message(STATUS "⚠️ libc++ not usable — falling back to default STL")
        endif()

    elseif(MSVC)
        message(STATUS "✅ Using MSVC STL")

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "✅ Using GCC libstdc++")

    else()
        message(WARNING "⚠️ Unknown compiler — using default STL behavior")
    endif()
endfunction()
