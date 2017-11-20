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

#ifndef LIBA_RANGE_HXX
#define LIBA_RANGE_HXX
#pragma once

#include <stddef.h>

namespace a
{


/* ========================================================================= */
/* Range                                                                     */
/* ========================================================================= */
template<class T>
class range
{
public:
    using value_type = T *;
    using size_type = size_t;

public:
    inline range()
        : m_begin(nullptr), m_end(nullptr) {}
    inline range(value_type begin, value_type end)
        : m_begin(begin), m_end(end) {}


    /* Operators */
    explicit inline
    operator bool() const
    {
        return m_begin && m_end;
    }


    /* Accessor */
    inline value_type begin() { return m_begin; }
    inline value_type end() { return m_end; }
    inline value_type begin() const { return m_begin; }
    inline value_type end() const { return m_end; }
    inline size_type size() const { return static_cast<size_type>(m_end - m_begin); }


private:
    value_type m_begin, m_end;
};

} /* namespace a */

#endif /* LIBA_RANGE_HXX */

