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

#ifndef LIBA_CALLBACK_HXX
#define LIBA_CALLBACK_HXX
#pragma once


namespace a
{

/* ========================================================================= */
/* Templatized Callback                                                      */
/* ========================================================================= */
template<class T, class U>
class callback
{
public:
    inline callback() : m_ptr(nullptr), m_ctx(nullptr) {}
    inline callback(T *ptr, T *ctx)
        : m_ptr(ptr)
        , m_ctx(ctx)
    {}

    template<class... Args>
    inline void
    operator()(Args... args)
    {
        m_ptr(ctx, args...);
    }

private:
    T *m_ptr;
    U *m_ctx;
};

} /* namespace a */

#endif /* LIBA_CALLBACK_HXX */

