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

