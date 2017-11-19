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

### Config
set(a_TEST_EXE "atests" CACHE STRING "Name of 'a' test executable")
set(a_LIB_NAME "a" CACHE STRING "Name of 'a' library")


### Sources
set(a_LIB_SOURCES
    src/file.cxx
    src/timer.cxx)

if(WIN32)
    set(a_LIB_SOURCES
        ${a_LIB_SOURCES}
        src/windows/io.cxx
    )
endif()


### Include Directories
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


### Main Library
add_library(${a_LIB_NAME} ${a_LIB_SOURCES})


### Header Files
set(a_LIB_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/allocator.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/attributes.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/buffer.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/error.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/file.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/iconv.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/io.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/platform.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/string.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/a/utilities.hxx"
)


###############################################################################
## INSTALLATION
###############################################################################
install(TARGETS ${a_LIB_NAME} DESTINATION lib)
install(FILES ${a_LIB_HEADERS} DESTINATION include)


###############################################################################
## TESTS
###############################################################################
enable_testing()
add_subdirectory(tests)

