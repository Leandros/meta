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

#ifndef LIBA_ERROR_HXX
#define LIBA_ERROR_HXX
#pragma once

#include <a/attributes.hxx>

namespace a {

/* ========================================================================= */
/* Simple Error Type                                                         */
/* ========================================================================= */
struct NODISCARD err_code
{
    int m_err = 0;
    inline err_code() {}
    inline err_code(bool err) : m_err(static_cast<int>(err)) {}
    inline err_code(int err) : m_err(err) {}
    explicit operator bool() const { return m_err; }
    operator int() const { return m_err; }
    inline int code() const { return m_err; }
};

} /* namespace a */


#endif /* LIBA_ERROR_HXX */

