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

#ifndef LIBA_CSV_HXX
#define LIBA_CSV_HXX
#pragma once

#include <stddef.h>

namespace a {

class csv
{
#define IS_SEPARATOR(c) ((c) == m_separator)
#define IS_NEWLINE(c)   ((c) == '\n' || (c) == '\r')
public:
    inline csv(char separator) : m_separator(separator) {}

    template<class F>
    inline void
    parse(char const *buf, F cb)
    {
        int col = 0, row = 0;

        /* TODO: Does not handle paranthesized values. */
        char const *begin = buf;
        char const *current = buf;
        for (; *current != '\0'; ++current) {
            while (!IS_SEPARATOR(*current) && !IS_NEWLINE(*current))
                current++;
            cb(begin, current - begin, row, col);
            begin = current + 1;

            col += 1;
            if (IS_NEWLINE(*current)) {
                row += 1;
                col = 0;
            }
        }
    }


#undef IS_SEPARATOR
#undef IS_NEWLINE
private:
    char const m_separator;
};


} /* namespace a */

#endif /* LIBA_CSV_HXX */

