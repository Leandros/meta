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

#include <a/timer.hxx>
#include <string.h>
#include <assert.h>

#if USING(OS_WINDOWS)
    #include <a/win32/misc.h>
#elif USING(OS_POSIX)
    #include <sys/time.h>
    #if USING(OS_MAC)
        #include <mach/clock.h>
        #include <mach/mach.h>
    #endif
#endif


namespace a
{

namespace timer
{

/* ========================================================================= */
/* INTERNAL                                                                  */
/* ========================================================================= */
namespace internal
{

#if USING(OS_WINDOWS)
static inline int64_t
filetime_to_msec(FILETIME const *ft)
{
    ULARGE_INTEGER dateTime;
    memcpy(&dateTime, ft, sizeof(dateTime));
    return (int64_t)(dateTime.QuadPart / 10000);
}

static inline int64_t
filetime_to_usec(FILETIME const *ft)
{
    ULARGE_INTEGER dateTime;
    memcpy(&dateTime, ft, sizeof(dateTime));
    return (int64_t)(dateTime.QuadPart / 10);
}
#endif

} /* namespace internal */


/* ========================================================================= */
/* IMPLEMENTATION                                                            */
/* ========================================================================= */
void
sleep(int ms)
{
#if USING(OS_MAC) || USING(OS_LINUX)
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&t, NULL);
#elif USING(OS_WINDOWS)
    /*
     * Starting with Windows Server 2003, passing 0 causes the current thread
     * to give up it's remaining time slice.
     */
#if defined(NTDDI_VERSION) && defined(NTDDI_WS03) && (NTDDI_VERSION >= NTDDI_WS03)
    Sleep(ms);
#else
    if (ms > 0)
        Sleep(ms);
#endif
#endif /* defined(_WIN32) */
}


int64_t
epoch(void)
{
#if USING(OS_MAC) || USING(OS_LINUX)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)((int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000);

#elif USING(OS_WINDOWS)
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return internal::filetime_to_msec(&ft) + INT64_C(11644473600000);
#endif
}

int64_t
time(void)
{
#if USING(OS_MAC) || USING(OS_LINUX)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return int64_t(INT64_C(tv.tv_sec) * INT64_C(1'000'000) + INT64_C(tv.tv_usec));

#elif USING(OS_WINDOWS)
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    /*
     * The epoch on Windows is 1601-01-01 00:00:00 UTC.
     * The epoch on Unix is 1970-01-01 00:00:00 UTC.
     * That's a difference of 134774 days, or 11644473600 seconds.
     * The following will convert the windows epoch to unix epoch.
     */
    return internal::filetime_to_usec(&ft) + INT64_C(11644473600000000);
#endif
}


int64_t
mono(void)
{
#if USING(OS_MAC)
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return (int64_t) ((int64_t)mts.tv_sec * 1000 + (int64_t)mts.tv_nsec / 1000000);

#elif USING(OS_LINUX)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)((int64_t)ts.tv_sec * 1000 + (int64_t)ts.tv_nsec / 1000000);

#elif USING(OS_WINDOWS)
    /*  System frequency does not change at run-time, cache it. */
    static int64_t frequency = 0;
    if (frequency == 0) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        /* Windows documentation says that XP and later will always return non-zero. */
        assert(freq.QuadPart != 0);
        frequency = freq.QuadPart;
    }

    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return (int64_t)(count.QuadPart * 1000) / frequency;
#endif
}


int64_t
usecs(void)
{
#if USING(OS_MAC)
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return (int64_t)((int64_t)mts.tv_sec * 1000000 + (int64_t)mts.tv_nsec / 1000);

#elif USING(OS_LINUX)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)((int64_t) ts.tv_sec * 1000000 + (int64_t)ts.tv_nsec / 1000);

#elif USING(OS_WINDOWS)
    //  System frequency does not change at run-time, cache it
    static int64_t frequency = 0;
    if (frequency == 0) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        // Windows documentation says that XP and later will always return non-zero
        assert(freq.QuadPart != 0);
        frequency = freq.QuadPart;
    }

    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return (int64_t)(count.QuadPart * 1000000) / frequency;
#endif
}


} /* namespace timer */

} /* namespace a */

