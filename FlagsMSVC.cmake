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

## Macros
##########
macro(flag f)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${f}")
endmacro()
macro(flag_debug f)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${f}")
endmacro()
macro(flag_debugrelease f)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${f}")
endmacro()
macro(flag_release f)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${f}")
endmacro()

macro(link f)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${f}")
endmacro()
macro(link_debug f)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${f}")
endmacro()
macro(link_debugrelease f)
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ${f}")
endmacro()
macro(link_release f)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${f}")
endmacro()



##############################################################################
## Linker
##############################################################################
link_debug("/DEBUG")

link_debugrelease("/DEBUG")
link_debugrelease("/OPT:REF,ICF")
link_debugrelease("/INCREMENTAL:NO")

link_release("/RELEASE")
link_release("/LTCG")
link_release("/OPT:REF,ICF")
link_debugrelease("/INCREMENTAL:NO")



##############################################################################
## Default
##############################################################################
## We override the default compiler flags.
set(CMAKE_CXX_FLAGS "/DWIN32 /D_WIN32 /DWINDOWS /D_WINDOWS /DUNICODE /D_UNICODE")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")


## Link static runtime
## Available options:
##  /MD     : DLL
##  /MDd    : DEBUG DLL
##  /MT     : Static
##  /MTd    : DEBUG Static
flag("/MT")



##############################################################################
## General
##############################################################################
flag("/std:c++14")              # Enable C++14
flag("/utf-8")                  # Source is in utf-8
flag("/MP12")                   # Use up to 12 cores
flag("/FS")                     # Serialize writing PDBs
flag("/GS- /Gs0x7fffffff")      # Remove buffer security checks
flag("/analyze-")               # Disable code analysis
flag("/GR-")                    # Disable RTTI
flag("/GT")                     # Fiber-safe TLS
flag("/Gy")                     # Enable function-level linking
flag("/EHsc-")                  # Disable C++ exceptions
flag("/Zc:wchar_t")             # Enable wchar_t type
flag("/Zc:forScope")            # Conforming for-loops
flag("/Zc:inline")              # Dont emit syms for unreferenced funcs
flag("/Zc:strictStrings")       # Enforce strings as const
flag("/Zc:implicitNoexcept")    # Mark functions as no-except
flag("/Zc:throwingNew-")        # Assume new never throws
flag("/Zc:ternary")             # Use C++ iso ternary behavior
flag("/Zc:rvalueCast")          # Enforce explicit type conversion
flag("/volatile:iso")           # Do not treat volatile as atomic
flag("/fp:except-")             # No floating point exceptions
flag("/errorReport:prompt")     # Prompt for errors
flag("/diagnostics:column")     # Output classic diagnostics
flag("/FC")                     # Use full path in diagnostics
flag("/permissive-")            # Enforce standard behavior
flag("/Gw")                     # Put functions in it"s own sections



##############################################################################
## Optimizations
##############################################################################
flag_debug("/Od")               # Disable optimizations
flag_debug("/Ob0")              # Disable inline-expansion
flag_debug("/Oi")               # Generate intrinsics
flag_debug("/Z7")               # Generate C 7.0 compatible debug info
flag_debug("/Oy-")              # Do not emit frame-pointer
flag_debug("/fp:precise")       # Precise floating point (other opt: fast)

flag_debugrelease("/O2")        # Enable mild optimizations
flag_debugrelease("/Oi")        # Generate intrinsics
flag_debugrelease("/Ob2")       # Allow full inline expansion
flag_debugrelease("/Z7")        # Generate C 7.0 compatible debug info
flag_debugrelease("/Oy-")       # Do not emit frame-pointer
flag_debugrelease("/fp:precise")# Precise floats (other opt: fast)
flag_debugrelease("/wd4710")    # Disable unused function warning
flag_debugrelease("/wd4711")    # Disable unused variable warning
flag_debugrelease("/wd4100")    # Disable unreferenced param warning
flag_debugrelease("/wd4189")    # Variable initialized by not ref'd

flag_release("/Ox")             # Full optimizations
flag_release("/Oi")             # Generate intrinsics
flag_release("/Ob2")            # Allow full inline expansion
flag_release("/Zi")             # Generate PDBs, instead of inline debug info
flag_release("/Oy")             # Omit frame-pointer
flag_release("/GL")             # Full program optimization
flag_release("/fp:precise")     # Precise floats (other opt: fast)



##############################################################################
## Warnings
##############################################################################
flag("/W4")                     # Enable warnings, level 4
flag("/WX-")                    # Don"t treat warnings as errors

# Ignored warnings
flag("/wd4290")                 # C++ exception spec ignored
flag("/wd4577")                 # noexcept used despite no exceptions
flag("/wd4505")                 # Unreferenced local function removed
flag("/wd4365")                 # Warn on signed/unsigned mismatch
flag("/wd4619")                 # pragma warning: unknown warning
flag("/wd4127")                 # conditional expression is constant

# Enabled warnings
flag("/we4062")                 # Not all enum values in switch
flag("/we4242")                 # Narrowing conversion
flag("/we4905")                 # wide string casted to "LPSTR"
flag("/we4906")                 # string casted to "LPWSTR"
flag("/we4263")                 # member func does not override base func
flag("/we4265")                 # class has virtual, but no virtual dtor
flag("/we4296")                 # expression is always "false"
flag("/we4826")                 # conversion is sign-extended
flag("/we4836")                 # non-standard extension used: "type"

# Disable MSVC CRT warnings
flag("/D_CRT_SECURE_NO_DEPRECATE")
flag("/D_CRT_SECURE_NO_WARNINGS")
flag("/D_CRT_NONSTDC_NO_DEPRECATE")
flag("/D_CRT_NONSTDC_NO_WARNINGS")
flag("/D_SCL_SECURE_NO_DEPRECATE")
flag("/D_SCL_SECURE_NO_WARNINGS")

# Disable warnings in Windows headers
flag("/D_STL_EXTRA_DISABLED_WARNINGS=\"4242 4365\"")

# Disable exceptions in STL
flag("/D_HAS_EXCEPTIONS=0")

