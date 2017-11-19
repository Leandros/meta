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

#ifndef LIBA_THREAD_HXX
#define LIBA_THREAD_HXX
#pragma once

#include <atomic>
#include <a/platform.hxx>


#if USING(COMPILER_MSVC)
    #define A_THREAD_YIELD() _mm_pause()
#elif USING(OS_LINUX)
    #include <sched.h>
    #define A_THREAD_YIELD() sched_yield()
#else
    #define A_THREAD_YIELD()
#endif


/* ========================================================================= */
/* Spin Lock                                                                 */
/* ========================================================================= */
class spin_lock
{
public:
    inline spin_lock() {}
    spin_lock(spin_lock const &) = delete;
    spin_lock &operator=(spin_lock const &) = delete;


    inline void
    lock()
    {
        while (m_lock.test_and_set(std::memory_order_acquire))
            A_THREAD_YIELD();
    }

    inline void
    unlock()
    {
        m_lock.clear(std::memory_order_release);
    }


private:
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
};

#endif /* LIBA_THREAD_HXX */

