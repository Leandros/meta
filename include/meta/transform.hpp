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

#ifndef LIBMETA_TRANSFORM_HPP
#define LIBMETA_TRANSFORM_HPP
#pragma once

#include <a/error.hxx>
#include <a/buffer.hxx>
#include <a/range.hxx>

namespace meta
{

class transform
{

public:
    inline transform() {}

    /*!
     * \brief
     * \param[in] range
     * \return
     */
    a::err_code
    init(a::range<char> const &range);

private:
    a::buffer<char> const *m_buf = nullptr;
};

} /* namespace meta */

#endif /* LIBMETA_TRANSFORM_HPP */

