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

## Default Values
set(IS_CLANG 0)
set(IS_MSVC 0)
set(IS_GCC 0)


## Detect Compiler
###############################################################################
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(clang|Clang|AppleClang)")
    set(IS_CLANG 1)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(IS_MSVC 1)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "gcc")
    set(IS_GCC 1)
endif()

if(NOT IS_CLANG AND
   NOT IS_MSVC AND
   NOT IS_GCC)
    message(STATUS "Unknown Compiler: ${CMAKE_CXX_COMPILER_ID} - ERROR")
endif()


## Strip Suffix
###############################################################################
function(get_suffix _sourceFile _extVar)
    # get_filename_component returns extension from first occurrence of . in file name
    # this function computes the extension from last occurrence of . in file name
    string(FIND "${_sourceFile}" "." _index REVERSE)
    if(_index GREATER -1)
        math(EXPR _index "${_index} + 1")
        string(SUBSTRING "${_sourceFile}" ${_index} -1 _sourceExt)
    else()
        set(_sourceExt "")
    endif()
    set(${_extVar} "${_sourceExt}" PARENT_SCOPE)
endfunction()

function(strip_suffix _src _dst)
    get_suffix("${_src}" _suf)
    string(LENGTH "${_src}" _srclen)
    string(LENGTH "${_suf}" _suflen)
    math(EXPR _length "${_srclen} - ${_suflen} - 1")
    string(SUBSTRING "${_src}" 0 ${_length} _out)
    set(${_dst} "${_out}" PARENT_SCOPE)
endfunction()


## Precompiled Headers
###############################################################################
#function(add_precompiled_header target headername)
#    if(IS_MSVC)
#        # Compile the precompiled header object file
#        strip_suffix("${headername}" filename)
#        set(srcfile "${filename}.cpp")
#        set(objfile "${filename}${CMAKE_CXX_OUTPUT_EXTENSION}")
#
#        set(objpath "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${objfile}")
#        set_source_files_properties(${srcfile} PROPERTIES
#            COMPILE_FLAGS "/Yc\"${headername}\"")
#
#        # Add precompiled source to target
#        target_sources(${target} PRIVATE ${srcfile})
#
#        # Get property
#        get_target_property(prop ${target} COMPILE_FLAGS)
#        if(NOT prop)
#            set(prop "")
#        endif()
#
#        # Name of pch file
#        set(prop "${prop} /Yu\"${headername}\"")
#        # Name of pch object
#        set(prop "${prop} /Fp\"${objpath}\"")
#        # Always include pch
#        set(prop "${prop} /FI\"${headername}\"")
#
#        # Apply properties
#        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "${prop}")
#    elseif(IS_CLANG OR IS_GCC)
#    else()
#        message(FATAL_ERROR "Compiler does not support precompiled headers")
#    endif()
#endfunction()

