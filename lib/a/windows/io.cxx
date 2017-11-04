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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <a/win32/io.h>
#include <a/win32/file.h>
#include <a/win32/threads.h>
#include <a/win32/process.h>

#include <a/io.hxx>
#include <a/iconv.hxx>
#include <a/platform.hxx>
#include <a/utilities.hxx>

namespace a {

/* ========================================================================= */
/* Cross-platform                                                            */
/* ========================================================================= */
#if USING(OS_WINDOWS)
    #define ftell64     _ftelli64
    typedef int64_t off_t;
#elif USING(OS_LINUX)
    #include <unistd.h>
    #define ftell64 ftello64
#elif USING(OS_MAC)
    #include <unistd.h>
    #define ftell64 ftello
#endif


/* ========================================================================= */
/* PROCESS                                                                   */
/* ========================================================================= */
#define POPEN_READ      1
#define POPEN_WRITE     2
struct pctx {
    int finished;
    int exitcode;
    unsigned mode;
    char *cmdline;
    HANDLE process;
    HANDLE out_r, out_w;
    HANDLE in_r, in_w;

    inline
    ~pctx()
    {
        if (cmdline)
            delete[] cmdline;
    }
};

process::~process()
{
    close();
    if (m_impl != nullptr)
        delete m_impl;
}

bool
process::create(char const *cmd, char const *mode)
{
    if (m_impl == nullptr)
        m_impl = new pctx;
    memset(m_impl, 0x0, sizeof(pctx));

    /* == Dynamic Destructor == */
    bool success = false;
    Auto(
        if (success) return;
        if (m_impl->in_r) CloseHandle(m_impl->in_r);
        if (m_impl->in_w) CloseHandle(m_impl->in_w);
        if (m_impl->out_r) CloseHandle(m_impl->out_r);
        if (m_impl->out_w) CloseHandle(m_impl->out_w);
    );

    /* == Parse Mode == */
    if (mode) {
        for (; (*mode) != '\0'; ++mode) {
            if ((*mode) == 'r')
                m_impl->mode |= POPEN_READ;
            else if ((*mode) == 'w')
                m_impl->mode |= POPEN_WRITE;
        }
    }

    /* == Security Attributes == */
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.bInheritHandle = TRUE;
    attr.lpSecurityDescriptor = NULL;

    /* == STDOUT pipe == */
    if ((m_impl->mode & POPEN_READ)) {
        if (!CreatePipe(&m_impl->out_r, &m_impl->out_w, &attr, 0))
            return false;
        if (!SetHandleInformation(m_impl->out_r, HANDLE_FLAG_INHERIT, 0))
            return false;
    }

    /* == STDIN pipe == */
    if ((m_impl->mode & POPEN_WRITE)) {
        if (!CreatePipe(&m_impl->in_r, &m_impl->in_w, &attr, 0))
            return false;
        if (!SetHandleInformation(m_impl->in_r, HANDLE_FLAG_INHERIT, 0))
            return false;
    }


    /* == Create child process == */
    {
        wchar_t *wcmd = nullptr;
        size_t lcmd;
        PROCESS_INFORMATION proc;
        STARTUPINFOW sinfo;

        lcmd = snprintf(NULL, 0, "cmd.exe /S /C \"%s\"", cmd) + 1;
        if (m_impl->cmdline != nullptr)
            delete[] m_impl->cmdline;
        m_impl->cmdline = new char[lcmd];
        wcmd = new wchar_t[lcmd];
        snprintf(m_impl->cmdline, lcmd, "cmd.exe /S /C \"%s\"", cmd);

        Auto(delete[] wcmd;);
        if (!widen(wcmd, m_impl->cmdline, lcmd))
            return false;

        memset(&proc, 0x0, sizeof(PROCESS_INFORMATION));
        memset(&sinfo, 0x0, sizeof(STARTUPINFOW));
        sinfo.cb = sizeof(STARTUPINFOW);
        sinfo.dwFlags |= STARTF_USESTDHANDLES;

        if ((m_impl->mode & POPEN_READ)) {
            sinfo.hStdError = m_impl->out_w;
            sinfo.hStdOutput = m_impl->out_w;
        }

        if ((m_impl->mode & POPEN_WRITE))
            sinfo.hStdInput = m_impl->in_r;

        /*
         * Create a process using:
         *  - 'wcmd' as command line argument
         *  - inherit handles
         *  - parents environment
         *  - parents current directory
         */
        success = CreateProcessW(
                NULL,
                wcmd,
                NULL,
                NULL,
                TRUE,
                0,
                NULL,
                NULL,
                &sinfo,
                &proc);
        if (!success)
            return false;

        CloseHandle(proc.hThread);
        m_impl->process = proc.hProcess;
    }

    return true;
}

int
process::close()
{
    if (m_impl->process)
        wait();

    /* Close all remaining input/output pipes. */
    if (m_impl->in_w)
        CloseHandle(m_impl->in_w), m_impl->in_w = NULL;
    if (m_impl->out_r)
        CloseHandle(m_impl->out_r), m_impl->out_r = NULL;

    return m_impl->exitcode;
}

size_t
process::write(void const *buf, size_t nbytes)
{
    /* writing to in_w */
    BOOL success;
    DWORD write, written;
    DWORD exitcode = 0;

    /* Can't write to processes not open for write. */
    if ((m_impl->mode & POPEN_WRITE) == 0)
        return 0;

    /* Wait till process is fully initialized. */
    if (WaitForInputIdle(m_impl->process, INFINITE))
        return 0;

    /* Do not write to already dead processes. */
    GetExitCodeProcess(m_impl->process, &exitcode);
    if (exitcode != STILL_ACTIVE)
        return 0;

    write = (DWORD)nbytes;
    success = WriteFile(m_impl->in_w, buf, write, &written, NULL);
    if (!success)
        return 0;
    return (size_t)written;
}

size_t
process::read(void *buf, size_t nbytes)
{
    /* reading from out_r */
    BOOL success;
    DWORD length, read;

    /* Can't read from processes not opened for read. */
    if ((m_impl->mode & POPEN_READ) == 0)
        return 0;

    /* Do not read from pipes with no bytes waiting, to avoid deadlocking. */
    if (!PeekNamedPipe(m_impl->out_r, NULL, 0, NULL, &read, NULL))
        return 0;
    if (read <= 0)
        return 0;

    length = (DWORD)nbytes;
    success = ReadFile(m_impl->out_r, buf, length, &read, NULL);
    if (!success)
        return 0;
    return (size_t)read;
}

bool
process::alive()
{
    DWORD exitcode;

    if (m_impl->finished)
        return false;

    if (GetExitCodeProcess(m_impl->process, &exitcode)) {
        if (exitcode == STILL_ACTIVE)
            return true;
        m_impl->exitcode = exitcode;
        m_impl->finished = 1;
    }

    return false;
}

int
process::wait()
{
    DWORD exitcode;
    char discard[4096];
    size_t bytes;

    /*
     * Remove all bytes still in the pipes, to force the process to
     * exit cleanly, without having us to kill it.
     */
    if ((m_impl->mode & POPEN_READ))
        while (alive())
            while ((bytes = read(discard, 4096)) > 0)
                fprintf(stderr, "read left over %d bytes\n", (int)bytes);

    /* TODO: How to handle cases where a program expects input? */

    /* Wait till process has terminated. */
    WaitForSingleObject(m_impl->process, INFINITE);
    if (!m_impl->finished) {
        if (!GetExitCodeProcess(m_impl->process, &exitcode))
            exitcode = 0x1U << 31U;
        m_impl->exitcode = (int)exitcode;
        m_impl->finished = 1;
    }

    /* Close handle to process. */
    if (!CloseHandle(m_impl->process))
        fprintf(stderr, "error: closing process: %d\n", GetLastError());
    m_impl->process = NULL;

    /* Close stdin/stdout/stderr handles of process. */
    if (m_impl->in_r)
        CloseHandle(m_impl->in_r), m_impl->in_r = NULL;
    if (m_impl->out_w)
        CloseHandle(m_impl->out_w), m_impl->out_w = NULL;

    return m_impl->exitcode;
}



/* ========================================================================= */
/* BUFFER                                                                    */
/* ========================================================================= */
char *
fbuf(char const *path, size_t *nbytes)
{
    FILE *fh;
    off_t off;
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

} /* namespace a */

