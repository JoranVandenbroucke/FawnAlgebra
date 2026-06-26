function(select_best_linker target)
    if (WIN32)
        # Check for RAD linker
        find_program(radlink_exe radlink)
        if (radlink_exe)
            execute_process(
                    COMMAND "${radlink_exe}" --version
                    RESULT_VARIABLE radlink_status
                    OUTPUT_QUIET
                    ERROR_QUIET
            )
            if (radlink_status EQUAL 0)
                message(STATUS "✅ RAD linker detected and verified: ${radlink_exe}")
                target_link_options(${target} PRIVATE
                        "$<$<CXX_COMPILER_ID:MSVC>:${radlink_exe}>"
                        "$<$<CXX_COMPILER_ID:Clang>:${radlink_exe}>"
                        "$<$<CXX_COMPILER_ID:GNU>:${radlink_exe}>"
                )
                return()
            else()
                message(STATUS "⚠️ RAD linker found but not callable — skipping")
            endif()
        endif()

        # If using Clang, try to detect lld-link
        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            find_program(lld_link_exe lld-link)
            if (lld_link_exe)
                message(STATUS "🔗 LLVM linker (lld-link) detected: ${lld_link_exe}")
                target_link_options(${target} PRIVATE "${lld_link_exe}")
                return()
            else()
                message(STATUS "⚠️ LLVM linker not found — falling back to MSVC default linker")
            endif()
        else()
            message(STATUS "✅ Using default linker for ${CMAKE_CXX_COMPILER_ID}")
        endif()
    else()
        message(STATUS "✅ Non-Windows platform — using default linker for ${CMAKE_CXX_COMPILER_ID}")
    endif()
endfunction()
