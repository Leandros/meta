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

#ifndef LIBA_ICONV_HXX
#define LIBA_ICONV_HXX
#pragma once

#include <a/platform.hxx>
#if USING(OS_WINDOWS)
#include <a/win32/misc.h>
#endif

namespace a {


#if USING(OS_WINDOWS)

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

#endif /* using(OS_WINDOWS) */

} /* namespace a */

#endif /* LIBA_ICONV_HXX */

