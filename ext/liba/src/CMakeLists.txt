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


### Sources
set(a_LIB_SOURCES
    file.cxx)

if(WIN32)
    set(a_LIB_SOURCES
        ${a_LIB_SOURCES}
        windows/io.cxx
    )
endif()


### Include Directories
include_directories(
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
)


### Main Executable
set(a_LIB_NAME "a" CACHE STRING "Name of 'a' library")
add_library(${a_LIB_NAME} ${a_LIB_SOURCES})


### Header Files
set(a_LIB_HEADERS
    "${PROJECT_SOURCE_DIR}/include/a/allocator.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/attributes.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/buffer.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/error.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/file.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/iconv.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/io.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/platform.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/string.hxx"
    "${PROJECT_SOURCE_DIR}/include/a/utilities.hxx"
)


###############################################################################
## INSTALLATION
###############################################################################
install(TARGETS ${a_LIB_NAME} DESTINATION lib)
install(FILES ${a_LIB_HEADERS} DESTINATION include)


