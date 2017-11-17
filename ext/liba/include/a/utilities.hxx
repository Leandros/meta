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

#ifndef LIBA_UTILITIES_HXX
#define LIBA_UTILITIES_HXX
#pragma once

#include <new>
#include <stdarg.h>
#include <stdio.h>
#include <a/platform.hxx>
namespace a {

/*!
 * \def ARR_SIZE
 * \brief
 */
#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

/*!
 * \def Auto
 * \brief Run a lambda at destruct time.
 * \remark Use with caution, as it may obstruct control flow.
 */
template<class Lambda>
class AtScopeExit {
  Lambda& m_lambda;
public:
  AtScopeExit(Lambda& action) : m_lambda(action) {}
  ~AtScopeExit() { m_lambda(); }
};
#define A_TOKEN_PASTEx(x, y) x ## y
#define A_TOKEN_PASTE(x, y) A_TOKEN_PASTEx(x, y)
#define A_Auto_INTERNAL1(lname, aname, ...) \
    auto lname = [&]() { __VA_ARGS__; }; \
    a::AtScopeExit<decltype(lname)> aname(lname);
#define A_Auto_INTERNAL2(ctr, ...) \
    A_Auto_INTERNAL1(A_TOKEN_PASTE(Auto_func_, ctr), \
                   A_TOKEN_PASTE(Auto_instance_, ctr), __VA_ARGS__)
#define Auto(...) A_Auto_INTERNAL2(__COUNTER__, __VA_ARGS__)


/*!
 * \def PATH_SEPARATOR
 * \brief
 */
#if USING(OS_WINDOWS)
    #ifndef PATH_SEPARATOR
        #define PATH_SEPARATOR '\\'
    #endif
#else
    #ifndef PATH_SEPARATOR
        #define PATH_SEPARATOR '/'
    #endif
#endif


/*!
 * \brief Debug logging, flushing output after each log.
 */
inline void
dlog(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fflush(stdout);
}


/*!
 * \brief Allocator, to satisfy C++'s crazy stupid lifetime rules.
 * \param[in] n Number of objects to allocate.
 * \return A valid pointer to the number of objects allocated.
 * \remark The canonical allocator for anything in this library.
 */
template<class T>
inline T *
zalloc(size_t n = 1)
{
    return new T[n];
}

/*!
 * \brief Deallocates previously allocated storage by \c zalloc()
 * \param[in] Pointer to storage, may be nullptr.
 */
template<class T>
inline void
zfree(T const *ptr)
{
    delete[] ptr;
}

} /* namespace a */
#endif /* LIBA_UTILITIES_HXX */

