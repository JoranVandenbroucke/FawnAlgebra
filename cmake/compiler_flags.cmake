# MSVC Debug
macro(get_compile_flags_Debug_MSVC Output)
    set(${Output}
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
        /Od
        /W4
        /WX
        /MP
        /Zi
        /DEBUG:FULL
        /permissive-
    PARENT_SCOPE)
endmacro()

# MSVC Release
macro(get_compile_flags_Release_MSVC Output)
    set(${Output}
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
        /O3
        /W4
        /WX
        /MP
        /fp:fast
        /arch:AVX2
    PARENT_SCOPE)
endmacro()

# MSVC RelWithDebInfo
macro(get_compile_flags_RelWithDebInfo_MSVC Output)
    set(${Output}
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
        /O3
        /W4
        /WX
        /MP
        /Zi
        /DEBUG:FULL
        /fp:fast
        /arch:AVX2
    PARENT_SCOPE)
endmacro()

# MSVC MinSizeRel
macro(get_compile_flags_MinSizeRel_MSVC Output)
    set(${Output}
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
        /O3
        /W4
        /WX
        /MP
        /Zi
        /DEBUG:FULL
        /Os                      # Favor smaller code
    PARENT_SCOPE)
endmacro()

# Clang Debug
macro(get_compile_flags_Debug_Clang Output)
    set(${Output}
        -fmodules
        -O0
        -g
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
    PARENT_SCOPE)
endmacro()

# Clang Release
macro(get_compile_flags_Release_Clang Output)
    set(${Output}
        -fmodules
        -O3
        -flto
        -ffast-math
        -fstrict-aliasing
        -funroll-loops
        -fmerge-all-constants
        -fomit-frame-pointer
        -fno-plt
        -fno-semantic-interposition
        -fvisibility=hidden
        -mavx2
        -freciprocal-math
        -fassociative-math
        -fvectorize
        -fvect-cost-model=unlimited
        -ffunction-sections
        -fdata-sections
        -fno-rtti
        -fno-exceptions
        -falign-functions=32
    PARENT_SCOPE)
endmacro()

# Clang RelWithDebInfo
macro(get_compile_flags_RelWithDebInfo_Clang Output)
    set(${Output}
        -fmodules
        -O3
        -g
        -flto
        -ffast-math
        -fstrict-aliasing
        -funroll-loops
        -fmerge-all-constants
        -fomit-frame-pointer
        -fno-plt
        -fno-semantic-interposition
        -fvisibility=hidden
        -mavx2
        -freciprocal-math
        -fassociative-math
        -fvectorize
        -fvect-cost-model=unlimited
        -ffunction-sections
        -fdata-sections
        -fno-rtti
        -fno-exceptions
    PARENT_SCOPE)
endmacro()

# Clang MinSizeRel
macro(get_compile_flags_MinSizeRel_Clang Output)
    set(${Output}
        -fmodules
        -Os
        -ffunction-sections
        -fdata-sections
        -fvisibility=hidden
    PARENT_SCOPE)
endmacro()

# GCC Debug
macro(get_compile_flags_Debug_GNU Output)
    set(${Output}
        -fmodules-ts
        -O0
        -g
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
        -std=gnu++23
    PARENT_SCOPE)
endmacro()

# GCC Release
macro(get_compile_flags_Release_GNU Output)
    set(${Output}
        -fmodules-ts
        -O3
        -flto
        -ffast-math
        -fstrict-aliasing
        -funroll-loops
        -fmerge-all-constants
        -fomit-frame-pointer
        -fno-plt
        -fno-semantic-interposition
        -fvisibility=hidden
        -mavx2
        -freciprocal-math
        -fassociative-math
        -fvectorize
        -fvect-cost-model=unlimited
        -ffunction-sections
        -fdata-sections
        -fno-rtti
        -fno-exceptions
        -falign-functions=32
        -std=gnu++23
    PARENT_SCOPE)
endmacro()

# GCC RelWithDebInfo
macro(get_compile_flags_RelWithDebInfo_GNU Output)
    set(${Output}
        -fmodules-ts
        -_ClangO3
        -g
        -flto
        -ffast-math
        -fstrict-aliasing
        -funroll-loops
        -fmerge-all-constants
        -fomit-frame-pointer
        -fno-plt
        -fno-semantic-interposition
        -fvisibility=hidden
        -mavx2
        -freciprocal-math
        -fassociative-math
        -fvectorize
        -fvect-cost-model=unlimited
        -ffunction-sections
        -fdata-sections
        -fno-rtti
        -fno-exceptions
        -std=gnu++23
    PARENT_SCOPE)
endmacro()

# GCC MinSizeRel
macro(get_compile_flags_MinSizeRel_GNU Output)
    set(${Output}
        -fmodules-ts
        -Os
        -ffunction-sections
        -fdata-sections
        -fvisibility=hidden
        -std=gnu++23
    PARENT_SCOPE)
endmacro()
