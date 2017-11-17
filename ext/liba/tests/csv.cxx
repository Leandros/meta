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

#include <a/csv.hxx>


TEST_CASE("csv", "[csv]")
{
    a::csv parser('\t');

    SECTION("simple test") {
        char const *simple =
            "ADS\tDE1234\tDE5678\n"
            "ALV\tDE0123\tDE4567\n";

        auto cb = [](char const *s, size_t n, int row, int col) {
            printf("'%.*s' (%d, %d)\n", (int)n, s, row, col);
        };

        parser.parse(simple, cb);
    }
}
