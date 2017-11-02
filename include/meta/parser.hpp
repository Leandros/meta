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

#ifndef LIBMETA_PARSER_HPP
#define LIBMETA_PARSER_HPP
#pragma once

#include <stddef.h>


namespace meta {


class parser
{
    struct range {
        char *begin, *end;
        inline range()
            : begin(nullptr), end(nullptr) {}
        inline range(char *_begin, char *_end)
            : begin(_begin), end(_end) {}

        explicit inline operator bool()
        {
            return begin && end;
        }
    };

public:
    inline parser() {}
    ~parser();

    /*!
     * \brief Create the parser by reading a file from disk.
     * \param[in] path Absolute or relative path to file on disk.
     * \return Whether the parser was initialized successfully.
     */
    bool init_from_file(char const *path);

    /*!
     * \brief Create the parser by reading a buffer
     * \param[in] path Pointer to allocated buffer.
     * \param[in] nbytes Size of buffer, in bytes. Without zero terminator.
     * \return Whether the parser was initialized successfully.
     * \remark The parser will \b NOT take ownership!
     */
    bool init_from_buffer(char const *buffer, size_t nbytes);


/* Utilities: */
private:
    char *next_token(char *s);
    bool match(char **s, char const *m);
    bool cmp(char *s, char const *m);
    char *closing_bracket(char **s);

/* Rules: */
private:
    void start();
    range empty_token(char **s);
    range constexpr_block(char **s);

/* Excution: */
private:
    void use_constexpr_block(range r);

private:
    char *m_buf = nullptr;
    size_t m_nbytes = 0;
};



} /* namespace meta */

#endif /* LIBMETA_PARSER_HPP */

