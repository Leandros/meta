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

#ifndef LIBA_MEMORY_HXX
#define LIBA_MEMORY_HXX
#pragma once

#include <type_traits>
#include <string.h>


/*!
 * \brief
 * \param[in] from
 * \return
 */
template<class To, class From>
inline constexpr To
bit_cast(From const &from)
{
    using copyableTo = std::is_trivially_copyable<To>;
    using copyableFrom = std::is_trivially_copyable<From>;
    static_assert(copyableTo::value, "To must be trivially copyable");
    static_assert(copyableFrom::value, "From must be trivially copyable");
    static_assert(sizeof(To) == sizeof(From), "To & From must be of same size");

    std::remove_const<To>::type to{};
    memcpy(std::addressof(to), std::addressof(from), sizeof(To));
    return to;
}


/*!
 * \brief
 * \param[in] from
 * \param[in] offset
 * \return
 */
template<class To, class From>
inline constexpr To
bit_cast_offset(From const &from, size_t offset)
{
    using copyableTo = std::is_trivially_copyable<To>;
    using copyableFrom = std::is_trivially_copyable<From>;
    static_assert(copyableTo::value, "To must be trivially copyable");
    static_assert(copyableFrom::value, "From must be trivially copyable");
    static_assert((sizeof(To) + offset) <= sizeof(From),
        "To + offset must be less than or equal to From's size");

    std::remove_const<To>::type to{};
    unsigned char buf[sizeof(From)];
    memcpy(buf, std::addressof(from), sizeof(From));
    memcpy(std::addressof(to), buf + offset, sizeof(To));
    return to;
}

#endif /* LIBA_MEMORY_HXX */

