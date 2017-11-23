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

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <assert.h>

#include <a/io.hxx>
#include <a/utilities.hxx>

namespace a {

/* ========================================================================= */
/* PROCESS                                                                   */
/* ========================================================================= */
#define POPEN_READ      1
#define POPEN_WRITE     2
#define PIPE_READ       0
#define PIPE_WRITE      1
#define GET_EXITCODE(status) (status & 0xFF)
#define SET_EXITCODE(status) ((status & 0xFF) | 0x100)
#define STRING_MEMBER(name, s) char name[sizeof(s)]
#define STRING_INIT(name, s) memcpy(name, s, sizeof(s))
struct pctx {
    int pid;
    int in[2];
    int out[2];
    int status;
    unsigned mode;
    char *cmdline;
    STRING_MEMBER(shell, "/bin/sh");
    STRING_MEMBER(arg0, "-c");

    inline pctx()
    {
        memset(this, 0x0, sizeof(*this));
        STRING_INIT(shell, "/bin/sh");
        STRING_INIT(arg0, "-c");
    }
    inline ~pctx() { delete[] cmdline; }
};


process::~process()
{
    close();
    delete m_impl;
}

bool
process::create(char const *cmd, char const *mode)
{
    m_impl = new pctx;
    if (mode) {
        for (; (*mode) != '\0'; ++mode) {
            if ((*mode) == 'r')
                m_impl->mode |= POPEN_READ;
            else if ((*mode) == 'w')
                m_impl->mode |= POPEN_WRITE;
        }
    }

    /* STDOUT pipe. */
    if ((m_impl->mode & POPEN_READ))
        if (pipe(m_impl->out) < 0)
            return false;
    Auto(
        if (m_impl->out[0]) ::close(m_impl->out[0]);
        if (m_impl->out[1]) ::close(m_impl->out[1]);
    );

    /* STDIN pipe. */
    if ((m_impl->mode & POPEN_WRITE))
        if (pipe(m_impl->in) < 0)
            return false;
    Auto(
        if (m_impl->in[0]) ::close(m_impl->in[0]);
        if (m_impl->in[1]) ::close(m_impl->in[1]);
    );

    /* Create commandline. */
    size_t lcmd = (size_t)snprintf(NULL, 0, "%s", cmd) + 1;
     m_impl->cmdline = new char[lcmd];
    snprintf(m_impl->cmdline, lcmd, "%s", cmd);

    char *argv[4];
    argv[0] = m_impl->shell;
    argv[1] = m_impl->arg0;
    argv[2] = m_impl->cmdline;
    argv[3] = NULL;

    char env[] = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
    char *env_arr[] = { env, NULL };

    /* Create child process. */
    m_impl->pid = fork();
    if (m_impl->pid == -1) {
        /* Error. */
        return false;
    } else if (m_impl->pid == 0) {
        /* Child. */
        ::close(STDIN_FILENO);
        ::close(STDOUT_FILENO);
        ::close(STDERR_FILENO);

        /* Assign pipes & close unused ends. */
        if ((m_impl->mode & POPEN_READ)) {
            ::close(m_impl->out[PIPE_READ]);
            if (dup2(m_impl->out[PIPE_WRITE], STDERR_FILENO) == -1)
                exit(1);
            if (dup2(m_impl->out[PIPE_WRITE], STDOUT_FILENO) == -1)
                exit(1);
        }

        if ((m_impl->mode & POPEN_WRITE)) {
            ::close(m_impl->in[PIPE_WRITE]);
            if (dup2(m_impl->in[PIPE_READ], STDIN_FILENO) == -1)
                exit(1);
        }

        execve(argv[0], argv, env_arr);

        /* execv only return on error. */
        perror("execv");
        exit(1);

    } else {
        /* Parent. */

        /* Close unused ends of our pipes. */
        if ((m_impl->mode & POPEN_READ))
            ::close(m_impl->out[PIPE_WRITE]);
        if ((m_impl->mode & POPEN_WRITE))
            ::close(m_impl->in[PIPE_READ]);

        return true;
    }
}

int
process::close()
{
    if (m_impl->pid != -1)
        wait();

    /* Close all remaining input/output pipes. */
    if (m_impl->in[PIPE_WRITE])
        ::close(m_impl->in[PIPE_WRITE]);
    if (m_impl->out[PIPE_READ])
        ::close(m_impl->out[PIPE_READ]);

    return GET_EXITCODE(m_impl->status);
}

size_t
process::write(void const *buf, size_t nbytes)
{
    /* writing to in[PIPE_WRITE] */

    /* NOT IMPLEMENTED */
    assert(0);

    /* Can't write to processes not open for write. */
    if ((m_impl->mode & POPEN_WRITE) == 0)
        return 0;
    return 0;
}

size_t
process::read(void *buf, size_t nbytes)
{
    /* reading from out[PIPE_READ] */

    /* Can't read from processes not opened for read. */
    if ((m_impl->mode & POPEN_READ) == 0)
        return 0;

    /* Do not read from pipes with no bytes waiting, to avoid deadlocking. */
    struct pollfd fds[1];
    fds[0].fd = m_impl->out[PIPE_READ];
    fds[0].events = POLLIN | POLLPRI;
    fds[0].revents = 0;
    if (poll(fds, 1, 0) != 1)
        return 0;

    ssize_t r = ::read(m_impl->out[PIPE_READ], buf, nbytes);
    if (r <= 0)
        return 0;
    return (size_t)r;
}

bool
process::alive()
{
    if (m_impl->status > 0)
        return false;

    int status;
    if (waitpid(m_impl->pid, &status, WNOHANG) == 0)
        return true;
    m_impl->status = WIFEXITED(status)
        ? SET_EXITCODE(WEXITSTATUS(status))
        : -1;

    return false;
}

int
process::wait()
{

    /*
     * Remove all bytes still in the pipes, to force the process to
     * exit cleanly, without having us to kill it.
     */
    size_t bytes;
    char discard[4096];
    if ((m_impl->mode & POPEN_READ))
        while (alive())
            while ((bytes = read(discard, 4096)) > 0)
                fprintf(stderr, "read %ld left-over bytes\n", bytes);

    /* Wait till process has terminated. */
    /* TODO: How to handle the error? */
    if (m_impl->status == 0) {
        int status;
        if (waitpid(m_impl->pid, &status, 0) == -1) {
            m_impl->status = -1;
        } else {
            m_impl->status = WIFEXITED(status)
                ? SET_EXITCODE(WEXITSTATUS(status))
                : -1;
        }
    }

    /* Close handle to process. */
    m_impl->pid = -1;

    /* Close stdin/stdout/stderr handles of process. */
    if (m_impl->in[PIPE_READ])
        ::close(m_impl->in[PIPE_READ]);
    if (m_impl->out[PIPE_WRITE])
        ::close(m_impl->out[PIPE_WRITE]);

    return GET_EXITCODE(m_impl->status);
}

} /* namespace a */

