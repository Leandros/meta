/* ========================================================================= */
/* Copyright (C) 2017-2017 Arvid Gerstmann                                   */
/*                                                                           */
/* This file is part of meta.                                                */
/*                                                                           */
/* meta is free software: you can redistribute it and/or modify              */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, either version 3 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* meta is distributed in the hope that it will be useful,                   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with meta.  If not, see <http://www.gnu.org/licenses/>.             */
/* ========================================================================= */
#ifndef A_PLATFORM_HXX
#define A_PLATFORM_HXX
#pragma once

/* ========================================================================= */
/* Utilities                                                                 */
#define USING(x)                ((1 x 1) == 2)
#define ON                      +
#define OFF                     -


/* ========================================================================= */
/* Platform Macros                                                           */
/* ========================================================================= */
#define OS_WINDOWS              OFF
#define OS_MAC                  OFF
#define OS_LINUX                OFF
#define OS_BSD                  OFF
#define OS_IPHONE               OFF
#define OS_ANDROID              OFF
#define OS_POSIX                OFF
/* Bitness: */
#define OS_32BIT                OFF
#define OS_64BIT                OFF

/* ------------------------------------------------------------------------- */
/* Windows                                                                   */
#if    defined(_WIN32) || defined(__WIN32__)        \
    || defined(_WINDOWS) || defined(__WINDOWS__)    \
    || defined(__TOS_WIN__)

    #undef  OS_WINDOWS
    #define OS_WINDOWS          ON

    #if defined(_WIN64)
        #undef  OS_64BIT
        #define OS_64BIT        ON
    #else
        #undef  OS_32BIT
        #define OS_32BIT        ON
    #endif
#endif

/* ------------------------------------------------------------------------- */
/* macOS                                                                     */
#if defined(__APPLE__)
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
        #undef  OS_IPHONE
        #define OS_IPHONE       ON
        #if defined(__LP64__) || defined(_LP64)
            #undef  OS_64BIT
            #define OS_64BIT    ON
        #else
            #undef  OS_32BIT
            #define OS_32BIT    ON
        #endif

    #elif TARGET_OS_MAC
        #undef  OS_MAC
        #define OS_MAC          ON
        #undef  OS_64BIT
        #define OS_64BIT        ON
    #endif
#endif

/* ------------------------------------------------------------------------- */
/* Linux                                                                     */
#if    defined(LINUX) || defined(linux) || defined(__linux__) || defined(__linux) \
    || defined(__unix__) || defined(__unix)

    #if defined(__ANDROID__)
        #undef  OS_ANDROID
        #define OS_ANDROID      ON
        /* TODO: Detect OS bitness! */
    #else
        #undef  OS_LINUX
        #define OS_LINUX        ON

        #if    defined(__amd64__) || defined(__amd64) \
            || defined(__x86_64__) || defined(__x86_64)
            #undef  OS_64BIT
            #define OS_64BIT    ON
        #else
            #undef  OS_32BIT
            #define OS_32BIT    ON
        #endif
    #endif
#endif

/* ------------------------------------------------------------------------- */
/* BSD                                                                       */
#if    defined(FreeBSD) || defined(__FreeBSD__)     \
    || defined(NetBSD) || defined(__NetBSD__)       \
    || defined(OpenBSD) || defined(__OpenBSD__)     \
    || defined(__bsdi__) || defined(__DragonFly__)
    #undef  OS_BSD
    #define OS_BSD              ON
#endif

/* ------------------------------------------------------------------------- */
/* POSIX                                                                     */
#if USING(OS_MAC) || USING(OS_LINUX) || USING(OS_BSD)
    #undef  OS_POSIX
    #define OS_POSIX            ON
#endif


/* ========================================================================= */
/* Compiler Macros                                                           */
/* ========================================================================= */
#define COMPILER_GCC            OFF
#define COMPILER_ICC            OFF
#define COMPILER_GHS            OFF
#define COMPILER_CLANG          OFF
#define COMPILER_MSVC           OFF


/* ------------------------------------------------------------------------- */
/* gcc                                                                       */
#if defined(__GNUC__) && !defined(__clang__)
    #undef  COMPILER_GCC
    #define COMPILER_GCC        ON
#endif

/* ------------------------------------------------------------------------- */
/* icc                                                                       */
#if defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ECC)
    #undef  COMPILER_ICC
    #define COMPILER_ICC        ON
#endif

/* ------------------------------------------------------------------------- */
/* green hills                                                               */
#if defined(__ghs__)
    #undef  COMPILER_GHS
    #define COMPILER_GHS        ON
#endif

/* ------------------------------------------------------------------------- */
/* clang                                                                     */
#if defined(__clang__)
    #undef  COMPILER_CLANG
    #define COMPILER_CLANG      ON
#endif

/* ------------------------------------------------------------------------- */
/* MSVC                                                                      */
#if defined(_MSC_VER)
    #undef  COMPILER_MSVC
    #define COMPILER_MSVC       ON
#endif

#endif /* A_PLATFORM_HXX */

