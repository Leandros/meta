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

#ifndef LIBA_IO_HXX
#define LIBA_IO_HXX
#pragma once

#include <stdlib.h>
namespace a {


/* ========================================================================= */
/* PROCESS                                                                   */
/* ========================================================================= */
class process
{
    friend struct pctx;
    process(process const &) = delete;
    process &operator=(process const &) = delete;

public:
    inline process() {}
    inline process(process &&other)
        { this->m_impl = other.m_impl; other.m_impl = nullptr; }
    inline process &operator=(process &&other)
        { this->m_impl = other.m_impl; other.m_impl = nullptr;  return *this; }
    ~process();

    /*!
     * \brief Create the process and start executing \c cmd.
     * \param[in] cmd The command to execute, will be executed in cmd.exe or bash.
     * \param[in] mode Mode string, consisting of combinations of 'r' and 'w'.
     * \return Whether the operation was succesfull.
     */
    bool create(char const *cmd, char const *mode);

    /*!
     * \brief Close the process.
     * \return The exit code of the process, as returned from 'main'.
     * \remark Will wait for termination, if required!
     */
    int close();

    /*!
     * \brief Will write to STDIN of the process.
     * \param[in] buf Pointer to allocated buffer, must be of \c nbytes size.
     * \param[in] nbytes Size of \c buf, in bytes.
     * \return Number of bytes written.
     */
    size_t write(void const *buf, size_t nbytes);

    /*!
     * \brief Will read from STDOUT & STDERR of the process.
     * \param[out] buf Allocated buffer, must be of \c nbytes size or more.
     * \param[in] nbytes Number of bytes available in the allocated \c buf buffer.
     * \return Number of bytes read and written into \c buf.
     */
    size_t read(void *buf, size_t nbytes);

    /*!
     * \brief Check if the process is still alive & running.
     * \return Whether the process is still alive.
     */
    bool alive();

    /*!
     * \brief Wait for normal process termination.
     * \return The exit code of the process, as returned from 'main'.
     * \remark Blocking operation!
     */
    int wait();


private:
    struct pctx *m_impl = nullptr;
};


} /* namespace a */
#endif /* LIBA_IO_HXX */

