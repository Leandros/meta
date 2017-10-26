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


### Main Executable
add_library(${meta_LIB_NAME}
    lib.cpp
)


### Header Files
set(meta_LIB_HEADERS
    "${PROJECT_SOURCE_DIR}/include/meta/meta.hpp"
)


###############################################################################
## INSTALLATION
###############################################################################
install(TARGETS ${meta_LIB_NAME} DESTINATION lib)
install(FILES ${meta_LIB_HEADERS} DESTINATION include)

