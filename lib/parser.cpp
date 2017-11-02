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
#include <a/utilities.hxx>
#include <meta/parser.hpp>

namespace meta {

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
    if ((m_buf = a::fbuf(path, &m_nbytes)) == nullptr)
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
/* Utilities                                                                 */
/* ========================================================================= */
char *
parser::next_token(char *s)
{
    while (isspace(*s) || *s == '\r' || *s == '\n')
        ++s;
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

bool
parser::cmp(char *s, char const *m)
{
    size_t l = strlen(m);
    if (strncmp(s, m, l) == 0)
        return true;
    return false;
}

char *
parser::closing_bracket(char **s)
{
    int level = 1;
    char *next, *orig;

    orig = *s, next = *s;
    do {
        if (cmp(next, "{"))
            level += 1;
        else if (cmp(next, "}"))
            level -= 1;
        else if (level == 0)
            break;
    } while ((next = next_token(next))[0]);

    if (!next[0])
        return nullptr;

    *s = next;
    return next;
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
        }

        ptr = orig;
        if (empty_token(&ptr))
            /* yes */;
    }
}

parser::range
parser::empty_token(char **s)
{
    char *begin = *s;
    char *end = next_token(*s);
    *s = end;
    return range(begin, end);
}

parser::range
parser::constexpr_block(char **s)
{
    char *orig = *s;
    if (match(s, "constexpr")
     && match(s, "{")) {
        char *begin = next_token(*s);
        char *last = closing_bracket(s);
        return range(begin, last);
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
    printf("use_constexpr_block()\n");
}

} /* namespace meta */

