#!/bin/bash
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

# Default Configuration
GENERATOR="Ninja"


# Create out-of-source build
if [ -d "build/" ]; then
    rm -rf build/*
else
    mkdir build
fi

# What to generate
if [ "$1" = "make" ]; then
    GENERATOR="Unix Makefiles"
fi

# Run CMake
pushd build/
cmake .. -G"$GENERATOR" \
    -DBUILD_TESTS:BOOL=YES

popd
echo "success!"
