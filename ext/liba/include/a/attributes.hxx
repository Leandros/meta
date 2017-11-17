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

#ifndef LIBA_ATTRIBUTES_HXX
#define LIBA_ATTRIBUTES_HXX
#pragma once

/* Evaluate to zero if not supported. */
#ifndef __has_attribute
    #define __has_attribute(x) 0
#endif

/* Evaluate to zero if not supported. */
#ifndef __has_cpp_attribute
    #define __has_cpp_attribute(x) 0
#endif

/* Check if [[nodiscard]] is available. */
#if (__cplusplus >= 201703L)
    #define NODISCARD [[nodiscard]]
#elif __has_attribute(warn_unused_result)
    #define NODISCARD __attribute__((warn_unused_result))
#elif __has_cpp_attribute(nodiscard)
    #define NODISCARD [[nodiscard]]
#elif defined(_MSC_VER) && _MSC_VER >= 1911
    #define NODISCARD [[nodiscard]]
#else
    #define NODISCARD
    #pragma message("NODISCARD is not available")
#endif


/*
 * C printf diagnostics
 */
#ifndef PRINTF_DIAG
    #if defined(__GNUC__)
        #define PRINTF_DIAG(string_index, first_to_check) \
            __attribute__((format(printf, string_index, first_to_check)))
    #else
        #define PRINTF_DIAG(string_index, first_to_check)
    #endif
#endif

#endif /* LIBA_ATTRIBUTES_HXX */


