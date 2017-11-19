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
#include <a/warnings.hxx>
namespace a {


/* ========================================================================= */
/* Compiler Specifics                                                        */
/* ========================================================================= */
/*!
 * \def THREAD_LOCAL
 * \brief Specify a thread local variable.
 */
#ifndef THREAD_LOCAL
    #if USING(COMPILER_MSVC)
        #define THREAD_LOCAL __declspec(thread)
    #elif USING(COMPILER_GCC) || USING(COMPILER_CLANG)
        #define THREAD_LOCAL __thread
    #elif __cplusplus >= 201103L
        #define THREAD_LOCAL thread_local
    #else
        #define THREAD_LOCAL
    #endif
#endif


/*!
 * \def OPTIMIZE_OFF
 * \def OPTIMIZE_ON
 * \brief Disable optimizations on the fly.
 */
#if !defined(OPTIMIZE_OFF) && !defined(OPTIMIZE_ON)
    #if USING(COMPILER_MSVC)
        #define OPTIMIZE_OFF    __pragma(optimize("", off))
        #define OPTIMIZE_ON     __pragma(optimize("", on))
    #elif USING(COMPILER_CLANG)
        #define OPTIMIZE_OFF    _Pragma("clang optimize off")
        #define OPTIMIZE_ON     _Pragma("clang optimize on")
    #elif USING(COMPILER_GCC)
        #define OPTIMIZE_OFF    _Pragma("GCC optimize (\"O0\")")
        #define OPTIMIZE_ON     _Pragma("GCC reset_options")
    #else
        #define OPTIMIZE_OFF
        #define OPTIMIZE_ON
    #endif
#endif


/*!
 * \def NOINLINE
 * \brief Disable inlining.
 */
#if !defined(NOINLINE)
    #if USING(COMPILER_MSVC)
        #define NOINLINE __declspec(noinline)
    #elif USING(COMPILER_CLANG) || USING(COMPILER_GCC)
        #define NOINLINE __attribute__((noinline))
    #else
        #define NOINLINE
    #endif
#endif


/*!
 * \def FORCEINLINE
 * \brief Force Inlining, at all costs.
 */
#if !defined(FORCEINLINE)
    #if USING(COMPILER_MSVC)
        #define FORCEINLINE inline __forceinline
    #elif USING(COMPILER_CLANG) || USING(COMPILER_GCC)
        #define FORCEINLINE inline __attribute__((always_inline))
    #else
        #define FORCEINLINE inline
    #endif
#endif


/*!
 * \def likely
 * \def unlikely
 * \brief Likely/Unlikely branches
 */
#if !defined(likely) && !defined(unlikely)
    #if USING(COMPILER_CLANG) || USING(COMPILER_GCC)
        #define likely(x) __builtin_expect ((x), 1)
        #define unlikely(x) __builtin_expect ((x), 0)
    #else
        #define likely(x) (x)
        #define unlikely(x) (x)
    #endif
#endif


/*!
 * \def NORETURN
 * \brief Marke a branch as unable to return from.
 */
#ifndef NORETURN
    #if USING(COMPILER_MSVC)
        #define NORETURN __declspec(noreturn)
    #elif USING(COMPILER_CLANG) || USING(COMPILER_GCC)
        #define NORETURN __attribute__((noreturn))
    #else
        #define NORETURN
    #endif
#endif


/*!
 * \def UNREACHABLE
 * \brief Unreachable code.
 */
#ifndef UNREACHABLE
    #if USING(COMPILER_MSVC)
        DISABLE_WARNING(invalid-noreturn,invalid-noreturn,4645)
        __declspec(noreturn) static void
        __custom_unreachable()
        {
            return;
        }
        ENABLE_WARNING(invalid-noreturn,invalid-noreturn,4645)

        #define UNREACHABLE __custom_unreachable()
    #elif USING(COMPILER_CLANG) || USING(COMPILER_GCC)
        #define UNREACHABLE __builtin_unreachable()
    #else
        #define UNREACHABLE
    #endif
#endif



/* ========================================================================= */
/* Utilities                                                                 */
/* ========================================================================= */
/*!
 * \def ARR_SIZE
 * \brief
 */
#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))


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

