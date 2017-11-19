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

#ifndef LIBA_TIMER_HXX
#define LIBA_TIMER_HXX
#pragma once

#include <a/platform.hxx>
#include <stdint.h>

namespace a
{

namespace timer
{

/*!
 * \brief Sleep for \c ms.
 * \param[in] ms Milliseconds to sleep.
 */
void sleep(int ms);


/*!
 * \brief Get the current system time.
 * \return Milliseconds since epoch.
 */
int64_t time();


/*!
 * \brief Get the current monotonic clock time.
 * \return Milliseconds.
 */
int64_t mono();


/*!
 * \brief Get the current monotonic clock time.
 * \return Microseconds.
 */
int64_t usecs();


} /* namespace timer */

} /* namespace a */


#endif /* LIBA_TIMER_HXX */

