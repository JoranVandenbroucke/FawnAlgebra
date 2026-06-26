function(set_compile_flags target)
    set(IS_WIN "$<PLATFORM_ID:Windows>")
    set(NOT_WIN "$<NOT:${IS_WIN}>")

    set(IS_MSVC  "$<CXX_COMPILER_ID:MSVC>")
    set(IS_CLANG "$<CXX_COMPILER_ID:Clang,AppleClang>")
    set(IS_GCC   "$<CXX_COMPILER_ID:GNU>")

    set(CFG_DBG  "$<CONFIG:Debug>")
    set(CFG_RWD  "$<CONFIG:RelWithDebInfo>")
    set(CFG_REL  "$<CONFIG:Release>")
    set(CFG_MSR  "$<CONFIG:MinSizeRel>")

    set(MSVC_DBG  "$<AND:${IS_MSVC},${CFG_DBG}>")
    set(MSVC_RWD  "$<AND:${IS_MSVC},${CFG_RWD}>")
    set(MSVC_REL  "$<AND:${IS_MSVC},${CFG_REL}>")
    set(MSVC_MSR  "$<AND:${IS_MSVC},${CFG_MSR}>")

    set(GNU_DBG   "$<AND:${IS_GCC},${CFG_DBG}>")
    set(GNU_RWD   "$<AND:${IS_GCC},${CFG_RWD}>")
    set(GNU_REL   "$<AND:${IS_GCC},${CFG_REL}>")
    set(GNU_MSR   "$<AND:${IS_GCC},${CFG_MSR}>")

    set(CLG_DBG   "$<AND:${IS_CLANG},${CFG_DBG}>")
    set(CLG_RWD   "$<AND:${IS_CLANG},${CFG_RWD}>")
    set(CLG_REL   "$<AND:${IS_CLANG},${CFG_REL}>")
    set(CLG_MSR   "$<AND:${IS_CLANG},${CFG_MSR}>")

    # Sanitizer guards — all gated on BALBINO_USE_SANITIZERS
    if (BALBINO_USE_SANITIZERS)
        set(SAN_MSVC_DBG  "${MSVC_DBG}")
        set(SAN_GNU_DBG_NWIN  "$<AND:${GNU_DBG},${NOT_WIN}>")
        set(SAN_GNU_DBG_WIN   "$<AND:${GNU_DBG},${IS_WIN}>")
        set(SAN_CLG_DBG_NWIN  "$<AND:${CLG_DBG},${NOT_WIN}>")
        set(SAN_CLG_DBG_WIN   "$<AND:${CLG_DBG},${IS_WIN}>")
        set(SAN_GNU_RWD   "${GNU_RWD}")
        set(SAN_CLG_RWD   "${CLG_RWD}")
    else()
        # Empty string — generator expressions that are never true
        set(SAN_MSVC_DBG  "$<AND:${IS_MSVC},$<BOOL:0>>")
        set(SAN_GNU_DBG_NWIN  "$<AND:${IS_GCC},$<BOOL:0>>")
        set(SAN_GNU_DBG_WIN   "$<AND:${IS_GCC},$<BOOL:0>>")
        set(SAN_CLG_DBG_NWIN  "$<AND:${IS_CLANG},$<BOOL:0>>")
        set(SAN_CLG_DBG_WIN   "$<AND:${IS_CLANG},$<BOOL:0>>")
        set(SAN_GNU_RWD   "$<AND:${IS_GCC},$<BOOL:0>>")
        set(SAN_CLG_RWD   "$<AND:${IS_CLANG},$<BOOL:0>>")
    endif()

    # =========================================================================
    # COMPILE OPTIONS
    # =========================================================================
    target_compile_options(${target} PRIVATE

        # ── MSVC ──────────────────────────────────────────────────────────────
        $<${IS_MSVC}:
            /W4 /WX /MP /permissive-
            /Zc:__cplusplus /Zc:inline
            /wd4996
        >

        $<${MSVC_DBG}:  /Od /Zi /RTC1>
        $<${SAN_MSVC_DBG}: /fsanitize=address>

        $<${MSVC_RWD}:  /O2 /Oi /Zi /GS /EHsc /fp:fast /Gy>

        $<${MSVC_REL}:
            /O2 /Oi /Ot /Ox
            /GL
            /Gy /Gw
            /Qpar
            /fp:fast
            /GS-
            /arch:AVX2
            /favor:blend
            /EHs-c-
            /GR-
        >

        $<${MSVC_MSR}:
            /O1 /Os /Oy /Gy /Gw /GL
            /GS- /GR- /EHs-c-
            /fp:precise
            /arch:SSE2
            /Oi-
            /Zc:threadSafeInit-
        >

        # ── GCC / Clang shared ────────────────────────────────────────────────
        $<$<OR:${IS_GCC},${IS_CLANG}>:
            -Wall -Wextra -Wpedantic -Wconversion -Werror
            -fno-omit-frame-pointer
            -fvisibility=hidden
            -fvisibility-inlines-hidden
        >

        $<${IS_CLANG}: -fcolor-diagnostics -fansi-escape-codes>
        $<${IS_GCC}:   -fdiagnostics-color=always>

        # ── GCC/Clang Debug — base (no sanitizers) ────────────────────────────
        $<$<OR:${GNU_DBG},${CLG_DBG}>:
            -O0 -fno-inline -g
            -fstack-protector-strong
            -fno-omit-frame-pointer
        >

        # ── Sanitizers — GCC/Clang Debug ──────────────────────────────────────
        $<${SAN_GNU_DBG_NWIN}: -fsanitize=address,leak>
        $<${SAN_GNU_DBG_WIN}:  -fsanitize=address>
        $<${SAN_CLG_DBG_NWIN}: -fsanitize=address,leak>
        $<${SAN_CLG_DBG_WIN}:  -fsanitize=address>

        $<${GNU_DBG}: -ggdb -D_GLIBCXX_ASSERTIONS>

        # ── GCC/Clang RelWithDebInfo ──────────────────────────────────────────
        $<$<OR:${GNU_RWD},${CLG_RWD}>:
            -O2 -g
            -march=native
            -fno-omit-frame-pointer
            -fno-strict-aliasing
        >
        $<${SAN_GNU_RWD}: -fsanitize=undefined>
        $<${SAN_CLG_RWD}: -fsanitize=undefined>

        # ── GCC/Clang Release ─────────────────────────────────────────────────
        $<$<OR:${GNU_REL},${CLG_REL}>:
            -O3
            -march=native -mtune=native
            -funroll-loops
            -ffast-math
            -fomit-frame-pointer
            -flto=auto
            -fno-stack-protector
            -fno-exceptions
            -fno-rtti
        >

        # ── GCC/Clang MinSizeRel ──────────────────────────────────────────────
        $<$<OR:${GNU_MSR},${CLG_MSR}>:
            -Os
            -fdata-sections -ffunction-sections
            -fmerge-all-constants
            -flto=auto
            -fno-exceptions
            -fno-rtti
            -fno-unwind-tables
            -fno-asynchronous-unwind-tables
        >
    )

    # =========================================================================
    # LINK OPTIONS
    # =========================================================================
    target_link_options(${target} PRIVATE

        # ── MSVC Debug ────────────────────────────────────────────────────────
        $<${MSVC_DBG}:
            /DEBUG:FULL /INCREMENTAL
            /OPT:NOREF /OPT:NOICF
            /PDBALTPATH:%_PDB%
        >

        $<${MSVC_RWD}:
            /DEBUG:FULL /INCREMENTAL
            /OPT:NOREF /OPT:NOICF
        >

        $<${MSVC_REL}:
            /LTCG /INCREMENTAL:NO
            /OPT:REF /OPT:ICF=5
            /NODEFAULTLIB:msvcrtd
        >

        $<${MSVC_MSR}:
            /LTCG /INCREMENTAL:NO
            /OPT:REF /OPT:ICF=5
            /MERGE:.rdata=.text /MERGE:.data=.text
            /NODEFAULTLIB:msvcrtd
            /RELEASE
        >

        # ── GCC Debug ─────────────────────────────────────────────────────────
        $<$<AND:${GNU_DBG},${NOT_WIN}>: -g -fuse-ld=gold>
        $<$<AND:${GNU_DBG},${IS_WIN}>:  -g -fuse-ld=lld>
        $<${SAN_GNU_DBG_NWIN}: -fsanitize=address,undefined,leak>
        $<${SAN_GNU_DBG_WIN}:  -fsanitize=address,undefined>

        # ── GCC RelWithDebInfo ────────────────────────────────────────────────
        $<${GNU_RWD}: -g -fuse-ld=gold -Wl,-O1,--as-needed>
        $<${SAN_GNU_RWD}: -fsanitize=undefined>

        # ── GCC Release & MinSizeRel ──────────────────────────────────────────
        $<$<OR:${GNU_REL},${GNU_MSR}>:
            -flto=auto -fuse-ld=gold
            -Wl,--gc-sections -Wl,-O2 -Wl,--as-needed
        >

        # ── Clang Debug ───────────────────────────────────────────────────────
        $<$<AND:${CLG_DBG},${NOT_WIN}>: -g -fuse-ld=lld>
        $<$<AND:${CLG_DBG},${IS_WIN}>:  -g -fuse-ld=lld>
        $<${SAN_CLG_DBG_NWIN}: -fsanitize=address,undefined,leak>
        $<${SAN_CLG_DBG_WIN}:  -fsanitize=address,undefined>

        # ── Clang RelWithDebInfo ──────────────────────────────────────────────
        $<${CLG_RWD}: -g -fuse-ld=lld -Wl,-O1,--as-needed>
        $<${SAN_CLG_RWD}: -fsanitize=undefined>

        # ── Clang Release & MinSizeRel ────────────────────────────────────────
        $<$<OR:${CLG_REL},${CLG_MSR}>:
            -flto=thin -fuse-ld=lld
            -Wl,--gc-sections -Wl,-O2 -Wl,--as-needed
        >
    )

endfunction()