/* ========================================================================= */
/* Copyright (C) 2017-2017 Arvid Gerstmann                                   */
/*                                                                           */
/* This file is part of liba.                                                */
/*                                                                           */
/* liba is free software: you can redistribute it and/or modify              */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, either version 3 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* liba is distributed in the hope that it will be useful,                   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with liba.  If not, see <http://www.gnu.org/licenses/>.             */
/* ========================================================================= */

#ifndef LIBA_WARNINGS_HXX
#define LIBA_WARNINGS_HXX
#pragma once

#include <a/platform.hxx>


/*!
 * \def DISABLE_WARNING
 * \param gopt GCC warning flag
 * \param copt clang warning flag
 * \param mopt MSVC warning code
 * \brief Temporarily disable a warning.
 *
 * If you encounter a warning option which is not supported by either compiler,
 * and you get a warning for this, just disable this warning instead. In case
 * of GCC you have to disable "-Wpragmas". Empty options are not supported!
 *
 * Example of unused call:
 *      DISABLE_WARNING(pragmas,pragmas,4444)
 */
/*!
 * \def ENABLE_WARNING
 * \param gopt GCC warning flag
 * \param copt clang warning flag
 * \param mopt MSVC warning code
 * \brief Re-enable a temporarily disabled warning.
 */
#define A_WARN_CONCAT_STR(s) #s
#define A_WARN_CONCAT(x, y) A_WARN_CONCAT_STR(x ## y)

#if USING(COMPILER_MSVC)
    #define A_DO_PRAGMA(x) __pragma (x)
    #define A_PRAGMA(cc, x) A_DO_PRAGMA(warning(x))
#else
    #define A_DO_PRAGMA(x) _Pragma (#x)
    #define A_PRAGMA(cc, x) A_DO_PRAGMA(cc diagnostic x)
#endif

#if USING(COMPILER_MSVC)
    #define DISABLE_WARNING(gopt, copt, mopt) \
        A_PRAGMA(msvc, push) A_DO_PRAGMA(warning(disable:##mopt))
    #define ENABLE_WARNING(gopt, copt, mopt) \
        A_PRAGMA(msvc, pop)
#elif USING(COMPILER_CLANG)
    #define DISABLE_WARNING(gopt, copt, mopt) \
        A_PRAGMA(clang, push) A_PRAGMA(clang, ignored A_WARN_CONCAT(-W, copt))
    #define ENABLE_WARNING(gopt, copt, mopt) \
        A_PRAGMA(clang, pop)
#elif USING(COMPILER_GCC)
    #if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
        #define DISABLE_WARNING(gopt, copt, mopt) \
            A_PRAGMA(GCC,push) A_PRAGMA(GCC,ignored A_WARN_CONCAT(-W, gopt))
        #define ENABLE_WARNING(gopt, copt, mopt) \
            A_PRAGMA(GCC, pop)
    #else
        #define DISABLE_WARNING(gopt, copt, mopt) \
            A_PRAGMA(GCC, ignored CONCAT(-W, gopt))
        #define ENABLE_WARNING(gopt, copt, mopt) \
            A_PRAGMA(GCC, warning CONCAT(-W, gopt))
    #endif
#else
    #define DISABLE_WARNING(gopt, copt, mopt)
    #define ENABLE_WARNING(gopt, copt, mopt)
#endif



/*!
 * \def DISABLE_ALL_WARNINGS
 * \def ENABLE_ALL_WARNINGS
 * \brief Disable/Enable all warnings
 * \remark Currently only available on MSVC
 */
#if USING(COMPILER_MSVC)
    #define DISABLE_ALL_WARNINGS    __pragma(warning(push, 0))
    #define ENABLE_ALL_WARNINGS     __pragma(warning(pop))
#elif USING(COMPILER_CLANG)
    #define DISABLE_ALL_WARNINGS
    #define ENABLE_ALL_WARNINGS
#elif USING(COMPILER_GCC)
    #define DISABLE_ALL_WARNINGS
    #define ENABLE_ALL_WARNINGS
#else
    #define DISABLE_ALL_WARNINGS
    #define ENABLE_ALL_WARNINGS
#endif

#endif /* LIBA_WARNINGS_HXX */

