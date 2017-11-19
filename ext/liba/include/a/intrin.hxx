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

#ifndef LIBA_INTRIN_HXX
#define LIBA_INTRIN_HXX
#pragma once

#include <a/platform.hxx>
#include <limits.h>
#include <string.h>

namespace a
{

namespace intrin
{

/* ========================================================================= */
/* MSVC Implementation                                                       */
/* ========================================================================= */
#if USING(COMPILER_MSVC)
inline constexpr unsigned long
bsr(unsigned long v)
{
    unsigned long trail;
    if (_BitScanReverse(&trail, v))
        return trail ^ 31;
    return 0;
}

inline constexpr unsigned long
bsr_bits()
{
    return sizeof(unsigned long) * CHAR_BIT;
}

inline constexpr unsigned __int64
bsrll_bits()
{
    return sizeof(unsigned __int64) * CHAR_BIT;
}

#if USING(OS_64BIT)
inline constexpr unsigned long
bsrll(unsigned __int64)
{
    unsigned long trail;
    if (_BitScanReverse64(&trail, v))
        return trail ^ 63;
    return 0;
}

#else /* USING(OS_64BIT) */

inline constexpr unsigned long
bsrll(unsigned __int64)
{
    static_assert(
        (sizeof(unsigned long) * 2) == sizeof(unsigned __int64),
        "two longs must be one __int64");

    char buf[8];
    unsigned long
        tmp1, tmp2,
        trail1, trail2;
    memcpy(buf, &v, 8);
    memcpy(&tmp1, buf, 4);
    memcpy(&tmp2, buf + 4, 4);
    trail1 = intrin_bsr(tmp1);
    trail2 = intrin_bsr(tmp2);
    if (trail1 == 0 && trail2 == 0)
        return 0;
    return trail2 == 0 ? 32 + trail1 : trail1 + trail2;
}

#endif /* USING(OS_64BIT) */


/* ========================================================================= */
/* Clang / GCC Implementation                                                */
/* ========================================================================= */
#elif USING(COMPILER_GCC) || USING(COMPILER_CLANG)

inline constexpr unsigned long
bsr(unsigned long v)
{
    if (v == 0)
        return 0;
    return __builtin_clzl(v);
}

inline constexpr unsigned long long
bsrll(unsigned long long v)
{
    if (v == 0)
        return 0;
    return __builtin_clzll(v);
}

inline constexpr unsigned long
bsr_bits()
{
    return sizeof(unsigned long) * 8;
}

inline constexpr unsigned long long
bsrll_bits()
{
    return sizeof(unsigned long long) * 8;
}


/* ========================================================================= */
/* Non-Intrinsic Implementation                                              */
/* ========================================================================= */
#else

inline constexpr unsigned long
bsr(unsigned long v)
{
    if (v == 0)
        return 0;

    static const char debruijn32[32] = {
        0, 31, 9, 30, 3, 8, 13, 29, 2, 5, 7, 21, 12, 24, 28, 19,
        1, 10, 4, 14, 6, 22, 25, 20, 11, 15, 23, 26, 16, 27, 17, 18
    };
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return debruijn32[x * 0x076be629 >> 27];
}

inline unsigned long long
bsrll(unsigned long long v)
{
    assert(0 && "not yet implemented");
    return 0;
}

inline constexpr unsigned long
bsr_bits()
{
    return sizeof(unsigned long) * 8;
}

inline constexpr unsigned long long
bsrll_bits()
{
    return sizeof(unsigned long long) * 8;
}

#endif


} /* namespace intrin */

} /* namespace a */

#endif /* LIBA_INTRIN_HXX */

