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

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <a/io.hxx>
#include <a/file.hxx>
#include <a/utilities.hxx>
#include <meta/parser.hpp>

namespace meta {

#define is_whitespace(x) (isspace((x)) || ((x) == '\r') || ((x) == '\n'))

parser::~parser()
{
    a::zfree(m_buf);
    m_nbytes = 0;
}


/* ========================================================================= */
/* Initializer                                                               */
/* ========================================================================= */
bool
parser::init_from_file(char const *path)
{
    if ((m_buf = a::file::read(path, &m_nbytes)) == nullptr)
        return false;
    return true;
}

bool
parser::init_from_buffer(char const *buffer, size_t nbytes)
{
    if ((m_buf = a::zalloc<char>(nbytes + 1)) == nullptr)
        return false;
    memcpy(m_buf, buffer, nbytes);
    m_buf[nbytes] = '\0';
    return true;
}


/* ========================================================================= */
/* Member Functions                                                          */
/* ========================================================================= */
void
parser::run()
{
    start();
}


/* ========================================================================= */
/* Utilities                                                                 */
/* ========================================================================= */
char *
parser::next_token(char *s)
{
    while (is_whitespace(*s))
        ++s;
    return s;
}

char *
parser::prev_token(char *s)
{
    while (is_whitespace(*s))
        --s;
    while (!is_whitespace(*s))
        --s;
    return s;
}

bool
parser::match(char **s, char const *m)
{
    char *orig = *s;
    size_t l = strlen(m);
    *s = next_token(*s);
    if (strncmp(*s, m, l) == 0)
        return *s += l, true;
    return *s = orig, false;
}

char *
parser::closing_bracket(char **s)
{
    int level = 1;
    char *next = *s;

    while (1) {
        if (match(&next, "{")) {
            level += 1;
            continue;
        } else if (match(&next, "}")) {
            level -= 1;
            continue;
        } else if (level == 0) {
            *s = next;
            return next;
        }

        if (!empty_token(&next))
            break;
    }

    return nullptr;
}


/* ========================================================================= */
/* Rules                                                                     */
/* ========================================================================= */
void
parser::start()
{
    char *ptr = m_buf;
    char *orig;
    range r;

    while (1) {
        orig = ptr;
        if ((r = constexpr_block(&ptr))) {
            use_constexpr_block(r);
            continue;
        }
        ptr = orig;

        /* Consume an empty token. */
        if (empty_token(&ptr))
            continue;
        /* Stop once no tokens are available anymore. */
        break;
    }
}

parser::range
parser::empty_token(char **s)
{
    char *begin = *s;
    *s = next_token(*s);
    if (**s == 0)
        return {};
    while (!is_whitespace(**s))
        (*s)++;
    return range(begin, *s);
}

parser::range
parser::constexpr_block(char **s)
{
    char *orig = *s;
    if (match(s, "constexpr")
     && match(s, "{")) {
        char *begin = next_token(*s);
        char *last = closing_bracket(s);
        return range(begin, prev_token(last));
    }

    *s = orig;
    return {};
}


/* ========================================================================= */
/* Execution                                                                 */
/* ========================================================================= */
void
parser::use_constexpr_block(range r)
{
    size_t len = r.end - r.begin;
    printf("%.*s\n", (int)len, r.begin);
}

} /* namespace meta */

