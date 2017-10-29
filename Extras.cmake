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

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "(gcc|GNU)")
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
function(add_precompiled_header target headername)
    # Compile the precompiled header object file
    strip_suffix("${headername}" filename)
    set(srcfile "${filename}.cpp")
    set(pch_added FALSE)

    # Append a property
    macro(append_property _filename _name _val)
        set_property(SOURCE ${_filename} APPEND PROPERTY ${_name} "${_val}")
    endmacro()

    # Check whether the source file exists
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}")
        message(FATAL_ERROR "${srcfile} does not exist")
    endif()

    if(IS_MSVC)
        set(objfile "${filename}.pch")
        set(objpath "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${objfile}")

        # Raise the memory limit for PCHs
        target_compile_options(${target} PRIVATE /Zm200)

        get_target_property(sources ${target} SOURCES)
        foreach(source ${sources})
            set(flags "")
            if(source STREQUAL "${srcfile}")
                set(pch_added TRUE)
                set(flags "${flags} /Fp\"${objpath}\"")
                set(flags "${flags} /Yc\"${headername}\"")
                append_property(${source} COMPILE_FLAGS "${flags}")
                if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio .*")
                    append_property(${source} OBJECT_OUTPUTS "${objpath}")
                endif()
            elseif(source MATCHES ".+(cpp|cxx|cc)")
                set(flags "${flags} /Fp\"${objpath}\"")
                set(flags "${flags} /Yu\"${headername}\"")
                set(flags "${flags} /FI\"${headername}\"")
                append_property("${source}" COMPILE_FLAGS "${flags}")
                if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio .*")
                    append_property(${source} OBJECT_DEPENDS "${objpath}")
                endif()
            endif()
        endforeach()

    elseif(IS_CLANG OR IS_GCC)
        get_target_property(sources ${target} SOURCES)
        foreach(source ${sources})
            set(flags "")
            if(source STREQUAL "${srcfile}")
                set(pch_added TRUE)
                set(flags "${flags} -x c++-header")
                append_property("${source}" COMPILE_FLAGS "${flags}")
            elseif(source MATCHES ".+(cpp|cxx|cc)")
                set(flags "${flags} -include \"${headername}\"")
                append_property("${source}" COMPILE_FLAGS "${flags}")
            endif()
        endforeach()

    else()
        message(FATAL_ERROR "Compiler does not support precompiled headers")
    endif()

    # Error Checking
    if(NOT pch_added)
        message(FATAL_ERROR "${srcfile} was not added to ${target}")
    endif()
endfunction()

