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


### Include Directories
include_directories(
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/include"
    "${CLANG_INCLUDE_PATH}"
)


### Main Executable
add_executable(${meta_EXE_NAME}
    main.cpp
)

### Precompiled header
add_precompiled_header(${meta_EXE_NAME} stdafx.hpp FORCEINCLUDE)


### Link Libraries
target_link_libraries(${meta_EXE_NAME}
    ${meta_LIB_NAME}
    ${CLANG_LIB_NAME}
)



###############################################################################
## INSTALLATION
###############################################################################
install(TARGETS ${meta_EXE_NAME} DESTINATION bin)

