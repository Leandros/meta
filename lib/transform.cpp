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
#include <a/buffer.hxx>
#include <meta/transform.hpp>

static char const *code_before =
"    #include <stdio.h>\n"
"    int\n"
"    main(int argc, char **argv)\n"
"    {\n"
;

static char const *code_after =
"    }\n"
;

namespace meta
{

a::err_code
transform::init(a::range<char> const &range)
{

    a::buffer_builder<char> builder;
    builder.append(code_before, strlen(code_before));
    builder.append(range.begin(), range.size());
    builder.append(code_after, strlen(code_after));
    a::buffer<char> buf = builder.to_buffer();
    printf("%s\n", buf.get());

    return true;
}


} /* namespace meta */

