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

#include <time.h>
#include <string.h>
#include <assert.h>
#include <a/file.hxx>
#include <a/iconv.hxx>
#include <a/platform.hxx>
#include <a/utilities.hxx>

/* ========================================================================= */
/* Cross-platform                                                            */
/* ========================================================================= */
#if USING(OS_LINUX) || USING(OS_MAC)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <limits.h>
    #include <dirent.h>
    #include <fcntl.h>
    #include <time.h>
    #if USING(OS_MAC)
        #include <mach-o/dyld.h>
    #endif
#endif

#if USING(OS_WINDOWS)
    #define ftell64 _ftelli64
    typedef int64_t off64_t;
    #include <a/win32/windows_base.h>
    #include <a/win32/file.h>
    #include <a/win32/io.h>

#elif USING(OS_LINUX)
    #include <unistd.h>
    #define ftell64 ftello64
    typedef off_t off64_t;
#elif USING(OS_MAC)
    #include <unistd.h>
    #define ftell64 ftello
    typedef off_t off64_t;
#endif


namespace a {
/* ========================================================================= */
/* Helper                                                                    */
/* ========================================================================= */
#if USING(OS_WINDOWS)
/* gmtime on windows is already thread-safe. */
struct tm *
gmtime_r(const time_t *timep, struct tm *result)
{
    struct tm *ret = gmtime(timep);
    return (struct tm *)memcpy(result, ret, sizeof(struct tm));
}
#endif



/* ========================================================================= */
/* FILE SYSTEM                                                               */
/* ========================================================================= */
bool
fs::rmdir(char const *path)
{
#if USING(OS_WINDOWS)
    wchar_t pathw[MAX_PATH+1];
    if (!widen(pathw, path, MAX_PATH+1))
        return false;
    return RemoveDirectoryW(pathw);
#else
    return ::rmdir(path) == 0;
#endif
}

bool
fs::rm(char const *path)
{
#if USING(OS_WINDOWS)
    wchar_t pathw[MAX_PATH+1];
    if (!widen(pathw, path, MAX_PATH+1))
        return false;
    return DeleteFileW(pathw);
#else
    return unlink(path) == 0;
#endif
}

bool
fs::mv(char const *from, char const *to)
{
#if USING(OS_WINDOWS)
    wchar_t fromw[MAX_PATH+1], tow[MAX_PATH+1];
    if (!widen(fromw, from, MAX_PATH+1))
        return false;
    if (!widen(tow, to, MAX_PATH+1))
        return false;
    return MoveFileW(fromw, tow);
#else
    return rename(from, to) == 0;
#endif
}

bool
fs::cp(char const *from, char const *to)
{
#if USING(OS_WINDOWS)
    wchar_t fromw[MAX_PATH+1], tow[MAX_PATH+1];
    if (!widen(fromw, from, MAX_PATH+1))
        return false;
    if (!widen(tow, to, MAX_PATH+1))
        return false;
    return CopyFileW(fromw, tow, FALSE);
#else
    bool success = true;
    size_t r, w;
    char buf[BUFSIZ];

    FILE *src, *dst;
    if ((src = fopen(from, "rb")) == NULL)
        return false;
    Auto(if (fclose(src)) success = false);
    if ((dst = fopen(to, "w")) == NULL)
        return false;
    Auto(if (fclose(dst)) success = false);

    /* Copy all file attributes. */
    struct stat st;
    if (fstat(fileno(src), &st) == -1)
        return false;
    if (fchown(fileno(dst), st.st_uid, st.st_gid) == -1)
        return false;
    if (fchmod(fileno(dst), st.st_mode))
        return false;

    /* Copy file contents. */
    while ((r = fread(buf, 1, BUFSIZ, src)) > 0)
        if ((w = fwrite(buf, 1, r, dst)) != r)
            return false;

    return success;
#endif
}

bool
fs::mv_safe(char const *from, char const *to)
{
#if USING(OS_WINDOWS)
    wchar_t fromw[MAX_PATH+1], tow[MAX_PATH+1];
    if (!widen(fromw, from, MAX_PATH+1))
        return false;
    if (!widen(tow, to, MAX_PATH+1))
        return false;
    if (!CopyFileW(fromw, tow, FALSE))
        return false;
    return DeleteFileW(fromw);
#else
    if (!cp(from, to))
        return false;
    return rm(from);
#endif
}

bool
fs::exists(char const *path)
{
#if USING(OS_WINDOWS)
    DWORD attrib;
    wchar_t pathw[MAX_PATH+1];
    if (!widen(pathw, path, MAX_PATH+1))
        return false;
    attrib = GetFileAttributesW(pathw);
    return attrib != INVALID_FILE_ATTRIBUTES
      && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    if (stat(path, &st) == -1)
        return false;
    return S_ISREG(st.st_mode);
#endif
}

int64_t
fs::time_created(char const *path, struct tm *t)
{
#if USING(OS_WINDOWS)
    HANDLE hFile;
    wchar_t pathw[MAX_PATH+1];
    if (!widen(pathw, path, MAX_PATH+1))
        return false;

    hFile = CreateFileW(
        pathw,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;
    Auto(CloseHandle(hFile));

    FILETIME ftCreate;
    if (!GetFileTime(hFile, &ftCreate, NULL, NULL))
        return false;

    SYSTEMTIME stUtc;
    if (!FileTimeToSystemTime(&ftCreate, &stUtc))
        return false;

    /* Fill in the struct, if passed in. */
    if (t) {
        t->tm_sec = stUtc.wSecond;
        t->tm_min = stUtc.wMinute;
        t->tm_hour = stUtc.wHour;
        t->tm_mday = stUtc.wDay;
        t->tm_mon = stUtc.wMonth - 1;
        t->tm_year = stUtc.wYear - 1900;
        t->tm_wday = stUtc.wDayOfWeek;
        t->tm_yday = -1; /* TODO: Fill in */
        t->tm_isdst = -1;
    }

    ULARGE_INTEGER li;
    memcpy(&li, &ftCreate, sizeof(ULARGE_INTEGER));
    /*
     * Convert 100-nanosecond intervals since 01.01.1601
     * to milliseconds since 01.01.1970 (unix/epoch time).
     */
    return (li.QuadPart / 10000LL) - 116444736000000000LL;
#elif USING(OS_MAC)
    struct stat st;
    if (stat(path, &st) == -1)
        return 0;

    /* Milliseconds since 01.01.1970. */
    int64_t ms = st.st_birthtimespec.tv_nsec / 1000000LL;

    /* Convert milliseconds since 01.01.1970 to tm. */
    time_t s = (ms / 1000LL);
    gmtime_r(&s, t);
    return ms;
#else
    assert(0);
    return 0;
    /* struct stat st; */
    /* if (stat(path, &st) == -1) */
    /*     return 0; */
    /* struct timespec t = get_stat_birthtime(&st); */
    /* if (t.tv_nsec == -1) */
    /*     return 0; */

    /* /1* Milliseconds since 01.01.1970. *1/ */
    /* int64_t ms = t.tv_nsec / 1000000LL; */

    /* /1* Convert milliseconds since 01.01.1970 to tm. *1/ */
    /* time_t s = (ms / 1000LL); */
    /* gmtime_r(&s, t); */
    /* return ms; */
#endif
}

bool
fs::get_exe_path(char *out, size_t size)
{
#if USING(OS_WINDOWS)
    wchar_t bufw[MAX_PATH+1];
    if (!GetModuleFileNameW(NULL, bufw, (DWORD)size))
        return false;
    if (!narrow(out, bufw, size))
        return false;

    /* Trim the executable name. */
    int i = 0, j = -1;
    while (out[i] != '\0') {
        if (out[i++] == '\\')
            j = i - 1;
    }

    if (j == -1)
        return false;

    out[j] = '\0';
    return true;
#elif USING(OS_MAC)
    uint32_t len = uint32_t(size);
    if (_NSGetExecutablePath(out, &len))
        return false;
    out[len] = '\0';

    int i, j;
    for (i = 0, j = -1; out[i] != '\0'; ++i)
        if (out[i] == '/')
            j = i;

    if (j == -1)
        return false;

    out[j] = '\0';
    return true;
#else
    char const *exe = "/proc/self/exe";

    ssize_t nlen;
    if ((nlen = readlink(exe, out, size)) == -1)
        return false;
    out[nlen] = '\0';

    int i, j;
    for (i = 0, j = -1; out[i] != '\0'; ++i)
        if (out[i] == '/')
            j = i;

    if (j == -1)
        return false;

    out[j] = '\0';
    return true;
#endif
}


/* ========================================================================= */
/* FILE                                                                      */
/* ========================================================================= */
bool
file::write(void const *buf, size_t size, char const *path)
{
    FILE *fh;
    if ((fh = fopen(path, "wb")) == NULL)
        return false;
    bool success = fwrite(buf, 1, size, fh) == size;
    if (fclose(fh))
        return false;
    return success;
}

char *
file::read(char const *path, size_t *nbytes)
{
    FILE *fh;
    off64_t off;
    size_t n, m;
    char buf[4096];
    if ((fh = fopen(path, "rb")) == NULL)
        return NULL;

    if (fseek(fh, 0, SEEK_END))
        goto e0;
    if ((off = ftell64(fh)) == -1L)
        goto e0;
    if (fseek(fh, 0, SEEK_SET))
        goto e0;

    char *ret;
    if ((ret = zalloc<char>((size_t)off + 1)) == NULL)
        goto e0;

    n = 0;
    while ((m = fread(buf, 1, 4096, fh)) != 0)
        memcpy(ret + n, buf, m), n += m;
    ret[n] = '\0';
    if (nbytes) *nbytes = n;

    if (fclose(fh))
        zfree(ret), ret = nullptr;
    return ret;

e0: fclose(fh);
    return nullptr;
}

size_t
file::size(FILE *fh)
{
    off64_t off;
    if (fseek(fh, 0, SEEK_END))
        return 0;
    if ((off = ftell64(fh)) == -1L)
        return 0;
    if (fseek(fh, 0, SEEK_SET))
        return 0;
    return (size_t)off;
}


} /* namespace a */

