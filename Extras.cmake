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

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(gcc|GNU)")
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


## Export compiler flags
###############################################################################
function(export_all_flags _filename _target)
  set(_include_directories "$<TARGET_PROPERTY:${_target},INCLUDE_DIRECTORIES>")
  set(_compile_definitions "$<TARGET_PROPERTY:${_target},COMPILE_DEFINITIONS>")
  set(_compile_flags "$<TARGET_PROPERTY:${_target},COMPILE_FLAGS>")
  set(_compile_options "$<TARGET_PROPERTY:${_target},COMPILE_OPTIONS>")
  set(_include_directories "$<$<BOOL:${_include_directories}>:-I$<JOIN:${_include_directories},\n-I>\n>")
  set(_compile_definitions "$<$<BOOL:${_compile_definitions}>:-D$<JOIN:${_compile_definitions},\n-D>\n>")
  set(_compile_flags "$<$<BOOL:${_compile_flags}>:$<JOIN:${_compile_flags},\n>\n>")
  set(_compile_options "$<$<BOOL:${_compile_options}>:$<JOIN:${_compile_options},\n>\n>")
  file(GENERATE OUTPUT "${_filename}" CONTENT "${_compile_definitions}${_include_directories}${_compile_flags}${_compile_options}\n")
endfunction()


## Precompiled Headers
###############################################################################
function(add_precompiled_header target headername)
    strip_suffix("${headername}" filename)
    set(srcfile "${filename}.cpp")
    set(objfile "${filename}.pch")
    set(objpath "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${objfile}")

    # Append a property
    macro(append_property _filename _name _val)
        set_property(SOURCE ${_filename} APPEND PROPERTY ${_name} "${_val}")
    endmacro()

    if(IS_MSVC)
        # Check whether the source file exists
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${srcfile}")
            message(FATAL_ERROR "${srcfile} does not exist")
        endif()

        # Raise the memory limit for PCHs
        target_compile_options(${target} PRIVATE /Zm200)

        # Compile the precompiled header object file
        target_sources(${target} PRIVATE ${srcfile})

        # Iterate all sources
        get_target_property(sources ${target} SOURCES)
        foreach(source ${sources})
            set(flags "")
            if(source STREQUAL "${srcfile}")
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
        get_filename_component(headerpath "${headername}" ABSOLUTE)

        # Gather all flags
        set(flagpath "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${target}.pch.flags")
        export_all_flags("${flagpath}" ${target})
        set(flags "@${flagpath}")

        # Precompile the header
        add_custom_command(
            OUTPUT "${objpath}"
            COMMAND "${CMAKE_CXX_COMPILER}" ${flags} -x c++-header -o "${objpath}" "${headerpath}"
            DEPENDS ${headerpath}
            BYPRODUCTS ${flagpath}
            COMMENT "Precompiling ${headername} for ${target}")

        # Iterate all sources
        get_target_property(sources ${target} SOURCES)
        foreach(source ${sources})
            set(cxxflags "")
            if(source MATCHES ".+(cpp|cxx|cc)")
                set(cxxflags "${cxxflags} -include \"${headername}\"")
                append_property(${source} COMPILE_FLAGS "${cxxflags}")
                if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio .*")
                    append_property(${source} OBJECT_DEPENDS "${objpath}")
                endif()
            endif()
        endforeach()
    else()
        message(FATAL_ERROR "Compiler does not support precompiled headers")
    endif()
endfunction()

