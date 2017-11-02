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

#ifndef A_ICONV_HXX
#define A_ICONV_HXX
#pragma once
namespace a {

#if defined(_WIN32)
#include <a/win32/misc.h>

inline bool
narrow(char *dst, wchar_t const *src, size_t size)
{
    return WideCharToMultiByte(65001, 0, src, -1, dst, (int)size, NULL, NULL);
}

inline bool
widen(wchar_t *dst, char const *src, size_t size)
{
    return MultiByteToWideChar(65001, 0, src, -1, dst, (int)size);
}

#endif /* defined(_WIN32) */


} /* namespace a */
#endif /* A_ICONV_HXX */

