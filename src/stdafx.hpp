#ifndef STDAFX_HPP
#define STDAFX_HPP

/* Disable all warnings */
#if defined(_MSC_VER)
    #pragma warning(push, 0)
#endif


/* ========================================================================= */
/* libc headers                                                              */
/* ========================================================================= */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* ========================================================================= */
/* libc++ headers                                                            */
/* ========================================================================= */
#include <new>
#include <memory>
#include <utility>


/* ========================================================================= */
/* System headers                                                            */
/* ========================================================================= */
#if defined(_WIN32)
    #define VC_EXTRALEAN
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #pragma warning(push, 0)
        #include <windows.h>
        #include <intrin.h>
    #pragma warning(pop)
#endif


/* Enable all warnings */
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#endif /* STDAFX_HPP */

