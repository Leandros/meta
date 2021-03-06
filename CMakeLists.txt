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

### CMake setup
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project Setup
project(meta CXX)
set(meta_VERSION_MAJOR 1)
set(meta_VERSION_MINOR 0)
set(meta_VERSION_PATCH 0)


### Options
option(BUILD_TESTS "Build the test suite" ON)
option(ENABLE_LTO "Enable LTO in RELEASE builds" OFF)
option(USE_UBSAN "Enable Undefined Behavior Sanitizer (clang only)" OFF)
option(USE_UBSAN_MINIMAL "Enable minimal Undefined Behaviour Sanitizer (clang only)" OFF)
option(USE_ADDRESSSAN "Enable Address Sanitizer (clang only)" OFF)
option(USE_THREADSAN "Enable Thread Sanitizer (clang only)" OFF)
option(USE_MEMSAN "Enable Memory Sanitizer (clang only)" OFF)
option(USE_CFISAN "Enable CFI Sanitizer (clang only)" OFF)
option(USE_STACKSAN "Enable Stack Sanitizer (clang only)" OFF)


### Specify to build RelWithDebInfo by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)


### Config
set(meta_EXE_NAME "meta")
set(meta_LIB_NAME "libmeta")
set(meta_TEST_EXE "tests")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


### Feature Testing
include(Extras.cmake)
include(FeatureTest.cmake)


### Compiler Options
if(IS_CLANG OR IS_GCC)
    include(FlagsClang.cmake)
elseif(IS_MSVC)
    include(FlagsMSVC.cmake)
endif()


### Configure File
configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/meta/config.h"
)


### External libraries
include(ext/libclang.cmake)


### Executable & Libraries
add_subdirectory(lib)
add_subdirectory(src)

### Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(BUILD_TESTS)

