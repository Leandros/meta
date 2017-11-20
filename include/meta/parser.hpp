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
#include <a/buffer.hxx>
#include <a/range.hxx>


namespace meta
{


/* ========================================================================= */
/* PARSER                                                                    */
/* ========================================================================= */
class parser
{
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

    /*!
     * \brief See \c init_from_buffer
     * \param[in] buf Initialized buffer.
     * \return See \c init_from_buffer
     */
    inline bool
    init_from_buffer(a::buffer<char> &buf)
    {
        return init_from_buffer(buf.get(), buf.size());
    }

    /*!
     * \brief Start the parsing
     */
    void run();


/* Utilities: */
private:
    char *next_token(char *s);
    char *prev_token(char *s);
    bool match(char **s, char const *m);
    char *closing_bracket(char **s);

/* Rules: */
private:
    void start();
    a::range<char> empty_token(char **s);
    a::range<char> constexpr_block(char **s);

/* Excution: */
private:
    void use_constexpr_block(a::range<char> r);

private:
    char *m_buf = nullptr;
    size_t m_nbytes = 0;
};



} /* namespace meta */

#endif /* LIBMETA_PARSER_HPP */

