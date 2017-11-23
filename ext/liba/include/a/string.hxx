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

#ifndef LIBA_STRING_HXX
#define LIBA_STRING_HXX
#pragma once

#include <string.h>
#include <stdarg.h>
#include <a/attributes.hxx>

namespace a {

/*
 * XXX: Templated for char type, to allow for wide-strings to be build?
 * Arvid: Would require template specialization, due to different internals.
 */
template<size_t N>
class string_builder
{
public:
    inline string_builder()
    {
        m_str[0] = '\0';
    }

    inline char const *
    append(char const *s, size_t nbytes = 0)
    {
        if (nbytes == 0)
            nbytes = strlen(s);

        if ((m_pos + nbytes + 1) >= N)
            return nullptr;
        memcpy(m_str + m_pos, s, nbytes);
        m_pos += nbytes;
        m_str[m_pos] = '\0';
        return m_str + m_pos;
    }

    PRINTF_DIAG(2, 3) inline char const *
    append_format(char const *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int n = vsnprintf(m_str + m_pos, N - m_pos, fmt, args);
        va_end(args);
        if (n == -1)
            return nullptr;
        m_pos += n;
        return m_str + m_pos;
    }

    inline char *get() { return m_str; }
    inline char const *get() const { return m_str; }
    inline size_t length() const { return m_pos; }
    inline void reset() { m_pos = 0; m_str[0] = '\0'; }

private:
    char m_str[N];
    size_t m_pos = 0;
};

} /* namespace a */

#endif /* LIBA_STRING_HXX */

