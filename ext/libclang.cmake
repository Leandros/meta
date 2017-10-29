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

set(PATH_TO_LIBCLANG "" CACHE PATH "Path to installation of libclang")


### If we're not using system libclang, download latest.
if(NOT PATH_TO_LIBCLANG)
    set(CLANG_VERSION 5.0.0)
    set(CLANG_BASE "http://releases.llvm.org/${CLANG_VERSION}")
    set(CLANG_DL 1)

    if(APPLE)
        set(CLANG_DIR "clang+llvm-${CLANG_VERSION}-x86_64-apple-darwin")
        set(CLANG_FILE "${CLANG_DIR}.tar.xz")

    elseif(WIN32)
        if(IS_64BIT)
            set(CLANG_DIR "llvm-${CLANG_VERSION}-win64")
            set(CLANG_FILE "${CLANG_DIR}.exe")
        else()
            set(CLANG_DIR "llvm-${CLANG_VERSION}-win32")
            set(CLANG_FILE "${CLANG_DIR}.exe")
        endif()

    elseif(UNIX)
        if(NOT IS_64BIT)
            message(FATAL_ERROR "No precompiled libclang binaries available")
        else()
            set(CLANG_DIR "clang+llvm-${CLANG_VERSION}-x86_64-linux-gnu-debian8")
            set(CLANG_FILE "${CLANG_DIR}.tar.xz")
        endif()
    endif()
endif()

### Check whether libclang already exists, skip if it does.
set(CLANG_LOCAL "${CMAKE_SOURCE_DIR}/ext/${CLANG_FILE}")
if(EXISTS "${CLANG_LOCAL}")
    set(CLANG_DL 0)
endif()

### Download libclang, if required.
if(CLANG_DL)
    message(STATUS "Downloading libclang v${CLANG_VERSION}")
    set(CLANG_REMOTE "${CLANG_BASE}/${CLANG_FILE}")
    file(DOWNLOAD "${CLANG_REMOTE}" "${CLANG_LOCAL}" SHOW_PROGRESS)
endif()

### Extract Archive.
set(CLANG_FILE "${CLANG_LOCAL}")
if(NOT EXISTS "${CLANG_DIR}")
    message(STATUS "Extracting libclang v${CLANG_VERSION} ...")
    if(CLANG_FILE MATCHES ".+bz2")
        execute_process(COMMAND tar -xjf "${CLANG_FILE}")
    elseif(CLANG_FILE MATCHES ".+xz")
        execute_process(COMMAND tar -xJf "${CLANG_FILE}")
    elseif(CLANG_FILE MATCHES ".+exe")
        # Windows .exe archive requires extraction with 7Zip
        find_program(7Z_EXECUTABLE 7z PATHS [HKEY_LOCAL_MACHINE\\SOFTWARE\\7-Zip;Path])
        if(NOT 7Z_EXECUTABLE)
            message(FATAL_ERROR "7-Zip is required to extract windows archive")
        endif()
        execute_process(COMMAND ${7Z_EXECUTABLE} x -o${CLANG_DIR} ${CLANG_FILE} OUTPUT_QUIET)
    else()
        execute_process(COMMAND tar -xzf "${CLANG_FILE}")
    endif()
endif()

### Set path
if(EXISTS "${CLANG_DIR}")
    set(PATH_TO_LIBCLANG "${CMAKE_BINARY_DIR}/${CLANG_DIR}")
endif()

### Libraries to link
find_library(TMP
    NAMES clang libclang
    PATHS ${PATH_TO_LIBCLANG}/lib
    NO_DEFAULT_PATH)
set(CLANG_LIB_NAME
    ${TMP}
)

set(CLANG_INCLUDE_PATH
    ${PATH_TO_LIBCLANG}/include
)

