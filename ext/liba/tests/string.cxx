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

#include <string.h>
#include <a/string.hxx>


TEST_CASE("string_builder", "[string]")
{
    a::string_builder<1024> builder;
    REQUIRE(builder.length() == 0);
    REQUIRE(builder.get()[0] == '\0');

    SECTION("many small appends") {
        char const *r0 = builder.append("hello");
        REQUIRE(*r0 == '\0');
        char const *r1 = builder.append(", ");
        REQUIRE(*r1 == '\0');
        char const *r2 = builder.append("world");
        REQUIRE(*r2 == '\0');
        char const *r3 = builder.append("!");
        REQUIRE(*r3 == '\0');
        REQUIRE(strcmp("hello, world!", builder.get()) == 0);

        SECTION("reset") {
            builder.reset();
            REQUIRE(builder.length() == 0);
            REQUIRE(strcmp("hello, world!", builder.get()) != 0);
        }
    }

    SECTION("append format") {
        char const *r0 = builder.append_format("%s", "hello");
        REQUIRE(*r0 == '\0');
        char const *r1 = builder.append_format("%c%c", ',', ' ');
        REQUIRE(*r1 == '\0');
        char const *r2 = builder.append_format("world");
        REQUIRE(*r2 == '\0');
        char const *r3 = builder.append_format("!");
        REQUIRE(*r3 == '\0');
        REQUIRE(strcmp("hello, world!", builder.get()) == 0);
    }

    SECTION("copy/move") {
        builder.append("hello, world!");
        REQUIRE(strcmp("hello, world!", builder.get()) == 0);

        SECTION("copy ctor") {
            a::string_builder<1024> b2(builder);
            REQUIRE(strcmp("hello, world!", b2.get()) == 0);

            b2.reset();
            REQUIRE(b2.length() == 0);
            REQUIRE(strcmp("hello, world!", b2.get()) != 0);
            REQUIRE(strcmp("hello, world!", builder.get()) == 0);
        }

        SECTION("copy assign") {
            a::string_builder<1024> b2;
            b2 = builder;
            REQUIRE(strcmp("hello, world!", b2.get()) == 0);

            b2.reset();
            REQUIRE(b2.length() == 0);
            REQUIRE(strcmp("hello, world!", b2.get()) != 0);
            REQUIRE(strcmp("hello, world!", builder.get()) == 0);
        }

        SECTION("move ctor") {
            a::string_builder<1024> b2(std::move(builder));
            REQUIRE(strcmp("hello, world!", b2.get()) == 0);

            b2.reset();
            REQUIRE(b2.length() == 0);
            REQUIRE(strcmp("hello, world!", b2.get()) != 0);
            REQUIRE(strcmp("hello, world!", builder.get()) == 0);
        }

        SECTION("move assign") {
            a::string_builder<1024> b2;
            b2 = std::move(builder);
            REQUIRE(strcmp("hello, world!", b2.get()) == 0);

            b2.reset();
            REQUIRE(b2.length() == 0);
            REQUIRE(strcmp("hello, world!", b2.get()) != 0);
            REQUIRE(strcmp("hello, world!", builder.get()) == 0);
        }
    }
}

