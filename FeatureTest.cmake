###############################################################################
## Copyright (C) 2017-2017 Arvid Gerstmann                                   ##
##                                                                           ##
## This file is part of meta.                                                ##
##                                                                           ##
## meta is free software: you can redistribute it and/or modify              ##
## it under the terms of the GNU General Public License as published by      ##
## the Free Software Foundation, either version 3 of the License, or         ##
## (at your option) any later version.                                       ##
##                                                                           ##
## meta is distributed in the hope that it will be useful,                   ##
## but WITHOUT ANY WARRANTY; without even the implied warranty of            ##
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             ##
## GNU General Public License for more details.                              ##
##                                                                           ##
## You should have received a copy of the GNU General Public License         ##
## along with meta.  If not, see <http://www.gnu.org/licenses/>.             ##
###############################################################################

### Setup. See all available modules here:
### https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html
include(CheckFunctionExists)
include(CheckSymbolExists)


### Check for functions (check_function_exists or check_symbol_exists)
check_symbol_exists(snprintf "stdio.h;stdlib.h" HAVE_SNPRINTF)


### Check System
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(IS_64BIT 0)
else()
    set(IS_64BIT 1)
endif()

