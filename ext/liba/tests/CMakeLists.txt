###############################################################################
## Copyright (C) 2017-2017 Arvid Gerstmann                                   ##
##                                                                           ##
## This file is part of liba.                                                ##
##                                                                           ##
## liba is free software: you can redistribute it and/or modify              ##
## it under the terms of the GNU General Public License as published by      ##
## the Free Software Foundation, either version 3 of the License, or         ##
## (at your option) any later version.                                       ##
##                                                                           ##
## liba is distributed in the hope that it will be useful,                   ##
## but WITHOUT ANY WARRANTY; without even the implied warranty of            ##
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             ##
## GNU General Public License for more details.                              ##
##                                                                           ##
## You should have received a copy of the GNU General Public License         ##
## along with liba.  If not, see <http://www.gnu.org/licenses/>.             ##
###############################################################################


### Include Directories
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)


### Main Executable
add_definitions(-DUSING_CATCH=1)
add_executable(${a_TEST_EXE}
    driver.cxx
    buffer.cxx
    string.cxx
    csv.cxx
)


### Precompiled header
add_precompiled_header(${a_TEST_EXE} stdafx.hxx stdafx.cxx FORCEINCLUDE)


### Link Libraries
target_link_libraries(${a_TEST_EXE}
    ${a_LIB_NAME}
)


### Add CTest
##############################################################################
add_test(NAME Run COMMAND ${a_TEST_EXE})

