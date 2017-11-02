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
#define A_USING(x)      ((1 x 1) == 2)
#define A_ON            +
#define A_OFF           -


/* ========================================================================= */
/* Platform Macros                                                           */
#define A_WINDOWS       A_OFF
#define A_MAC           A_OFF
#define A_LINUX         A_OFF
#define A_BSD           A_OFF

/* ------------------------------------------------------------------------- */
/* Windows                                                                   */
#if defined(_WIN64) \
    || defined(_WIN32) \
    || defined(__WIN32__) \
    || defined(__WINDOWS__)

#undef  A_WINDOWS
#define A_WINDOWS       A_ON

#endif

/* ------------------------------------------------------------------------- */
/* macOS                                                                     */
#if defined(__APPLE__)

#undef  A_MAC
#define A_MAC           A_ON

#endif

/* ------------------------------------------------------------------------- */
/* Linux                                                                     */
#if defined(__linux__)

#undef  A_LINUX
#define A_LINUX         A_ON

#endif

/* ------------------------------------------------------------------------- */
/* BSD                                                                       */
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)

#undef  A_BSD
#define A_BSD           A_ON

#endif


#endif /* A_PLATFORM_HXX */

