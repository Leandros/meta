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

#include <a/buffer.hxx>
#include <string.h>

TEST_CASE("buffer_builder", "[buffer]")
{
    a::buffer_builder<char, 64> builder;

    /* SECTION("many small appends") { */
    /*     /1* Setup *1/ */
    /*     char const *s0 = "hello, world!"; */
    /*     char const *s1 = "hola, mundo!"; */
    /*     char const *s2 = "hallo, welt!"; */
    /*     char const *separator = " "; */
    /*     size_t const s0len = strlen(s0); */
    /*     size_t const s1len = strlen(s1); */
    /*     size_t const s2len = strlen(s2); */
    /*     size_t const seplen = strlen(separator); */

    /*     /1* Appending *1/ */
    /*     builder.append(s0, s0len); */
    /*     builder.append(separator, seplen); */
    /*     builder.append(s1, s1len); */
    /*     builder.append(separator, seplen); */
    /*     builder.append(s2, s2len); */

    /*     /1* Size *1/ */
    /*     size_t const len = s0len + s1len + s2len + (seplen * 2); */
    /*     REQUIRE(builder.length() == len); */

    /*     /1* Buffer conversion *1/ */
    /*     auto buf = builder.to_buffer(); */
    /*     REQUIRE(buf); */
    /*     REQUIRE(buf.size() == len); */
    /*     REQUIRE(buf.size() == builder.length()); */

    /*     /1* Contents. *1/ */
    /*     size_t off = 0; */
    /*     REQUIRE(memcmp(s0, buf.get(), s0len) == 0); */
    /*     off += s0len + seplen; */
    /*     REQUIRE(memcmp(s1, buf.get() + off, s1len) == 0); */
    /*     off += s1len + seplen; */
    /*     REQUIRE(memcmp(s2, buf.get() + off, s2len) == 0); */

    /*     SECTION("reset") { */
    /*         builder.reset(); */
    /*         REQUIRE(builder.length() == 0); */

    /*         auto buf1 = builder.to_buffer(); */
    /*         REQUIRE(buf1); */
    /*         REQUIRE(buf1.size() == 0); */
    /*         REQUIRE(buf1.size() == builder.length()); */

    /*         SECTION("append after reset") { */
    /*             builder.append(s0, s0len); */
    /*             builder.append(s1, s1len); */
    /*             builder.append(s2, s2len); */

    /*             size_t const len2 = s0len + s1len + s2len; */
    /*             REQUIRE(builder.length() == len2); */

    /*             auto buf2 = builder.to_buffer(); */
    /*             REQUIRE(buf2); */
    /*             REQUIRE(buf2.size() == len2); */
    /*             REQUIRE(buf2.size() == builder.length()); */

    /*             /1* Contents. *1/ */
    /*             size_t off2 = 0; */
    /*             REQUIRE(memcmp(s0, buf2.get(), s0len) == 0); */
    /*             off2 += s0len; */
    /*             REQUIRE(memcmp(s1, buf2.get() + off2, s1len) == 0); */
    /*             off2 += s1len; */
    /*             REQUIRE(memcmp(s2, buf2.get() + off2, s2len) == 0); */
    /*         } */
    /*     } */
    /* } */

    SECTION("big appends") {
        /* Setup */
        char const *s0 =
            "aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmm"
            "nnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz";
        size_t const s0len = strlen(s0);
        char const *s1 =
            "aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmm"
            "nnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz"
            "nnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz"
            "aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmm";
        size_t const s1len = strlen(s1);
        char const *s2 =
            "aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmm";
        size_t const s2len = strlen(s2);

        /* /1* Appending *1/ */
        builder.append(s0, s0len);
        builder.append(s1, s1len);
        /* builder.append(s2, s2len); */

        /* /1* Size *1/ */
        /* size_t const len = s0len + s1len + s2len; */
        /* REQUIRE(builder.length() == len); */

        /* /1* Buffer conversion *1/ */
        /* auto buf = builder.to_buffer(); */
        /* REQUIRE(buf); */
        /* REQUIRE(buf.size() == len); */
        /* REQUIRE(buf.size() == builder.length()); */

        /* /1* Contents. *1/ */
        /* size_t off = 0; */
        /* REQUIRE(memcmp(s0, buf.get(), s0len) == 0); */
        /* off += s0len; */
        /* REQUIRE(memcmp(s1, buf.get() + off, s1len) == 0); */
        /* off += s1len; */
        /* REQUIRE(memcmp(s2, buf.get() + off, s2len) == 0); */
    }
}

