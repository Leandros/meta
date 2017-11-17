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

#ifndef LIBA_FILE_HXX
#define LIBA_FILE_HXX
#pragma once

#include <stdio.h>
#include <stdint.h>
namespace a {

/* Assuming 4k accesses are best. */
#ifndef BUFSIZ
#define BUFSIZ 4096
#endif

/* Internal constants. */
enum {
    PATHSIZE = 255,
};


/* ========================================================================= */
/* FILE SYSTEM                                                               */
/* ========================================================================= */
namespace fs {

/*!
 * \brief
 * \param[in] path
 * \return
 */
bool rmdir(char const *path);

/*!
 * \brief
 * \param[in] path
 * \return
 */
bool rm(char const *path);

/*!
 * \brief
 * \param[in] from
 * \param[in] to
 * \return
 */
bool mv(char const *from, char const *to);

/*!
 * \brief
 * \param[in] from
 * \param[in] to
 * \return
 */
bool cp(char const *from, char const *to);

/*!
 * \brief
 * \param[in] from
 * \param[in] to
 * \return
 */
bool mv_safe(char const *from, char const *to);

/*!
 * \brief
 * \param[in] path
 * \return
 */
bool exists(char const *path);

/*!
 * \brief
 * \param[in] path
 * \param[out] t
 * \return
 */
int64_t time_created(char const *path, struct tm *t);

/*!
 * \brief
 * \param[out] buf
 * \param[in] nbytes
 * \return
 */
bool get_exe_path(char *buf, size_t nbytes);

} /* namespace fs */


/* ========================================================================= */
/* FILE                                                                      */
/* ========================================================================= */
namespace file {

/*!
 * \brief
 * \param[in] buf
 * \param[in] nbytes
 * \param[in] path
 * \return
 */
bool write(void const *buf, size_t nbytes, char const *path);

/*!
 * \brief Read contents of \c path into a buffer.
 * \param[in] path Pointer to path. May not be NULL.
 * \param[out] nbytes Bytes read, without zero-terminator. May be NULL.
 * \return Pointer to allocated buffer, or nullptr on failure.
 * \remark Buffer must be freed with zfree().
 */
char *read(char const *path, size_t *nbytes = nullptr);

/*!
 * \brief
 * \param[in] fh
 * \return
 */
size_t size(FILE *fh);

} /* namespace file */

} /* namespace a */
#endif /* LIBA_FILE_HXX */

