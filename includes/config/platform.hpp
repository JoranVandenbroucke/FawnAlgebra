//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once

#if defined(_AIX) || defined(__TOS_AIX__)
#define BALBINO_OS_AIX 1
#error "AIX is not supported"
#endif
#if defined(__ANDROID__)
#define BALBINO_OS_ANDROID 1
#error "Android is not supported"
#endif
#if defined(UTS)
#define BALBINO_OS_UTS 1
#error "Amdahl UTS is not supported"
#endif
#if defined(AMIGA)|| defined(__amigaos__)
#define BALBINO_OS_ANIGA 1
#error "AmigaOS is not supported"
#endif
#if defined aegis
#define BALBINO_OS_AEGIS 1
#error "Apollo AEGIS is not supported"
#endif
#if defined apollo
#define BALBINO_OS_APOLLO 1
#error "Apollo Domain is not supported"
#endif
#if defined __BEOS__
#define BALBINO_OS_BEOS 1
#error "BeOS is not supported"
#endif
#if defined(__bg__) || defined(__THW_BLUEGENE__)
#define BALBINO_OS_BLUE_GENE 1
#error "Blue Gene is not supported"
#endif
#if defined( 	__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__) || defined(_SYSTYPE_BSD)
#define BALBINO_OS_BSD 1
#error "BSD Environment is not supported"
#endif
#if defined(__bsdi__)
#define BALBINO_OS_BSDI 1
#error "BSD/OS is not supported"
#endif
#if defined(__convex__)
#define BALBINO_OS_CONVEX 1
#error "ConvexOS is not supported"
#endif
#if defined(__CYGWIN__)
#define BALBINO_OS_WINDOWS 1
#endif
#if defined(DGUX) || defined(__DGUX__) || defined(__dgux__)
#define BALBINO_OS_DGUX 1
#error "DG/UX is not supported"
#endif
#if defined(_DragonFly__)
#define BALBINO_OS_DRAGONFLY 1
#error "DragonFly is not supported"
#endif
#if defined(_SEQUENT_) || defined(sequent)
#define BALBINO_OS_DYNIX 1
#error "DYNIX/ptx is not supported"
#endif
#if defined(__ECOS)
#define BALBINO_OS_ECOS 1
#error "eCos is not supported"
#endif
#if defined(__EMX__)
#define BALBINO_OS_EMX 1
#error "EMX Environment is not supported"
#endif
#if defined(__FreeBSD__)
#define BALBINO_OS_FREEBSD 1
#error "FreeDSB is not supported"
#endif
#if defined(__GNU__) || defined(__gnu_hurd__)
#define BALBINO_OS_GNU_HURD 1
#error "GNU aka GNU/Hurd is not supported"
#endif
#if defined(__FreeBSD_kernel__) && defined(__GLIBC__)
#define BALBINO_OS_GNU_FREE 1
#error "GNU/kFreeBSD is not supported"
#endif
#if defined(__gnu_linux__)
#define BALBINO_OS_GNU_LINUX 1
#error "GNU/Linux is not supported"
#endif
#if defined(__hiuxmpp)
#define BALBINO_OS_HIUXMPP 1
#error "HI-UX MPP is not supported"
#endif
#if defined(_hpux) || defined(hpux) || defined(__hpux)
#define BALBINO_OS_HP 1
#error "HP-UX is not supported"
#endif
#if defined(__OS400__)
#define BALBINO_OS_OS400 1
#error "IBM OS/400 is not supported"
#endif
#if defined(__INTEGRITY)
#define BALBINO_OS_INTEGRITY 1
#error "INTEGRITY is not supported"
#endif
#if defined(__INTERIX)
#define BALBINO_OS_INTEGREX 1
#error "Interix Environment is not supported"
#endif
#if defined(__linux__) || defined(linux) || defined(__linux)
#define BALBINO_OS_LINUX 1
#endif
#if defined(__Lynx__)
#define BALBINO_OS_LYNX 1
#error "Linux kernel is not supported"
#endif
#if defined(macintosh) || defined(Macintosh) || defined(__APPLE__) && defined (__MACK_)
#define BALBINO_OS_MAC 1
#endif
#if defined(_OS9000) || defined(_OSK)
#define BALBINO_OS_OS9 1
#error "Microware OS-9 is not supported"
#endif
#if defined(__minix)
#define BALBINO_OS_MINIX 1
#error "MINIX is not supported"
#endif
#if defined(__MORPHOS__)
#define BALBINO_OS_MORPHOS 1
#error "MorphOS is not supported"
#endif
#if defined(mpeix) || defined(__mpexl)
#define BALBINO_OS_MPEIX 1
#error "MPE/iX is not supported"
#endif
#if defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#define BALBINO_OS_MSDOS 1
#error "MSDOS is not supported"
#endif
#if defined(__NetBSD__)
#define BALBINO_OS_NETBSD 1
#error "NetBSD is not supported"
#endif
#if defined(__OpenBDS__)
#define BALBINO_OS_OPENBSD 1
#error "OpenBSD is not supported"
#endif
#if defined(OS2) || defined(_OS2) ||defined(__OS2__) || defined(__TOS_OS2__)
#define BALBINO_OS_OS2 1
#error "OS/2 is not supported"
#endif
#if defined(__palmos__)
#define BALBINO_OS_PALMOS 1
#error "Palm OS is not supported"
#endif
#if defined(EPLAN9)
#define BALBINO_OS_PLAN9 1
#error "Plan 9 is not supported"
#endif
#if defined(pyr)
#define BALBINO_OS_PYRAMID 1
#error "Pyramid DC/OSx is not supported"
#endif
#if defined(__QNX__) || defined(__QNXNTO__)
#define BALBINO_OS_QNX 1
#error "QNX is not supported"
#endif
#if defined(sinux)
#define BALBINO_OS_RELIANT 1
#error "Reliant UNIX is not supported"
#endif
#if defined(M_I386) || defined(M_XENIX) || defined(_SCO_DS)
#define BALBINO_OS_SCOOPENSERVER 1
#error "SCO OpenServer is not supported"
#endif
#if defined(sun) || defined(__sun)
#if defined(__SVR4) || defined(__svr4__)
    #define BALBINO_OS_SOLARIS 1
    #error "SoLaris is not supported"
#else
    #define BALBINO_OS_SUNOS 1
    #error "SunOS is not supported"
#endif
#endif
#if defined(__VOS__)
    #define BALBINO_OS_STRATUSVOS 1
    #error "Stratus VOS is not supported"
#endif
#if defined(__sysv__) || defined(__SVR4) || defined(__svr4__) ||(_SYSTYPE_SVR4)
    #define BALBINO_OS_SVR4 1
    #error "SVR4 Environment is not supported"
#endif
#if defined(_SYLLABLE_)
    #define BALBINO_OS_SYLLABLE 1
    #error "Syllable is not supported"
#endif
#if defined(__SYMBIAN32__)
    #define BALBINO_OS_SYMBAINLOS 1
    #error "Symbian OS is not supported"
#endif
#if defined(__osf__) || defined(__osf)
    #define BALBINO_OS_TRU64 1
    #error "Tru64 (OSF/1) is not supported"
#endif
#if defined(ultrix) || defined(__ultrix) || defined(__ultrix__) || defined(unix) && defined(vax)
        #define BALBINO_OS_ULTRIX 1
        #error "ULTRIX is not supported"
#endif
#if defined(_UNICOS)
        #define BALBINO_OS_UNICOS 1
        #error "UNICOS is not supported"
#endif
#if defined(_CRAY) || defined(__crayx1)
        #define BALBINO_OS_UNICOS_MP 1
        #error "UNICOS/mp is not supported"
#endif
#if defined(__unix__) || defined(__unix)
        #define BALBINO_OS_UNIXENVIRONMENT 1
        #error "UNIX Environment is not supported"
#endif
#if defined(sco) || defined(_UNIXWARE7)
    #define BALBINO_OS_UNIXWARE 1
    #error "UnixWare is not supported"
#endif
#if defined(_UWIN)
    #define BALBINO_OS_UWINENVIRONMENT 1
    #error "U/Win Environment is not supported"
#endif
#if defined(VMS)||defined(__VMS)
    #define BALBINO_OS_VMS 1
    #error "VMS is not supported"
#endif
#if defined(__VXWORKS__) || defined(__vxworks)
    #define BALBINO_OS_VXWORKS 1
    #error "VxWorks is not supported"
#endif
#if defined(_WIN16) || defined(_WIN32)|| defined(_WIN64) || defined(__WIN32__)|| defined(__TOS_WIN__) || defined(__WINDOWS__)
#define BALBINO_OS_WINDOWS 1
#endif
#if defined(_WIN32_WCE)
    #define BALBINO_OS_WINDOWSCE 1
    #error "Windows CE is not supported"
#endif
#if defined(_WINDU_SOURCE)
    #define BALBINO_OS_WINDUENVIRONMENT 1
    #error "Wind/U Environment is not supported"
#endif
#if defined(__MVS__) || defined(__HOS_MVS__) || defined(__TOS_MVS__)
    #define BALBINO_OS_ZOS 1
    #error "z/OS is not supported"
#endif

#ifdef BALBINO_OS_ANDROID
#include <android/api-level.h>
#endif

#ifdef BALBINO_OS_LINUX
#include <unistd.h>
#endif

#if BALBINO_OS_WINDOWS
#define NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#endif
#define NOGDICAPMASKS
// #define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NOIME
#include <windows.h>
#endif
