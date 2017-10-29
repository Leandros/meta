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

### When running in MSVC compat mode, forward the flags to clang.
### When not running in MSVC compat mode, use flags as is.
if(MSVC)
    macro(flag f)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang ${f}")
    endmacro()
    macro(flag_debug f)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Xclang ${f}")
    endmacro()
    macro(flag_debugrelease f)
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -Xclang ${f}")
    endmacro()
    macro(flag_release f)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Xclang ${f}")
    endmacro()
else()
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
endif()


##############################################################################
## Optimizations
##############################################################################
flag("-std=c++11")                      # C++ standard
flag("-fpic")                           # Position independent code
flag("-fno-math-errno")                 # Math funcs never set errno
flag("-fno-fast-math")                  # Disable fast-math
flag("-fno-exceptions")                 # Disable exceptions
flag("-fno-rtti")                       # Disable RTTI
flag("-freciprocal-math")               # Allow div to be optimized
flag("-fstrict-enums")                  # Assume enums are always in range
flag("-fno-signed-zeros")               # Assume floats have no signed zeros
flag("-ffunction-sections")             # Put functions in own sections
flag("-fms-extensions")                 # Enable Microsoft extensions
flag("-fvisibility-ms-compat")          # Global func/var hidden by default

### Optimizer Levels
flag_debug("-march=native")                     # Native architecture
flag_debug("-fno-omit-frame-pointer")           # Improved debuggability
flag_debug("-fstack-protector-all")             # Enable stack protector

flag_debugrelease("-march=native")              # Native architecture
flag_debugrelease("-flto=thin")                 # Enable LTO (thin)
flag_debugrelease("-fno-omit-frame-pointer")    # Improved debuggability
flag_debugrelease("-fstack-protector-all")      # Enable stack protector
flag_debugrelease("-UNDEBUG")                   # Enable asserts

flag_release("-march=ivybridge")                # Set IvyBridge as earliest arch
flag_release("-flto")                           # Enable LTO (full)
flag_release("-fwhole-program-vtables")         # Enable whole program vtable opt
flag_release("-fno-stack-protector")            # Disable stack protector

### Clang only flags
if(IS_CLANG)
    flag("-fcolor-diagnostics")                 # Colored diagnostics
    flag("-fstrict-vtable-pointers")            # Enable strict C++ vtable rules
endif()

### MSVC Specific
if(NOT MSVC)
    flag("-ftrapv")                             # Trap on integer overflow
endif(NOT MSVC)


##############################################################################
## Warnings
##############################################################################
flag("-Wall")
flag("-Wextra")
flag("-Wuninitialized")
flag("-Winit-self")
flag("-Wfloat-equal")
flag("-Wundef")
flag("-Wshadow")
flag("-Wno-cast-qual")
flag("-Wcast-align")
flag("-Wconversion")
flag("-Wno-multichar")
flag("-Wno-packed")
flag("-Wstrict-overflow")
flag("-Wvla")
flag("-Wformat")
flag("-Wno-format-zero-length")
flag("-Winvalid-pch")
flag("-Wswitch-enum")
flag("-Wunused-variable")
flag("-Wunused-function")

### Warnings as errors
flag("-Werror=unused-result")

### Disabled warnings
flag("-Wno-sign-conversion")
flag("-Wno-cast-align")

### Clang only
if(IS_CLANG)
    ### Warnings
    flag("-Wunused-private-field")

    ### Disabled warnings
    flag("-Wno-unknown-warning-option")
    flag("-Wno-microsoft-anon-tag")
    flag("-Wno-unused-parameter")
    flag("-Wno-tautological-constant-out-of-range-compare")
endif()


##############################################################################
## Definitions
##############################################################################
add_definitions(-D_REENTRANT)

### Platform Dependent
if(APPLE)
    add_definitions(-D_DARWIN_FEATURE_64_BIT_INODE)
elseif(UNIX)
    add_definitions(-D_GNU_SOURCE)
    add_definitions(-D_POSIX_C_SOURCE=200809L)
    add_definitions(-D_FILE_OFFSET_BITS=64)
    add_definitions(-D_LARGEFILE64_SOURCE)
    add_definitions(-D_XOPEN_SOURCE=700)
endif()


##############################################################################
## Sanitizers
##############################################################################
if(USE_UBSAN)
    flag("-fsanitize=undefined,integer")
endif()
if(USE_UBSAN_MINIMAL)
    flag("-fsanitize=undefined -fsanitize-minimal-runtime")
endif()
if(USE_ADDRESSSAN)
    flag("-fsanitize=address")
endif()
if(USE_THREADSAN)
    flag("-fsanitize=thread")
endif()
if(USE_MEMSAN)
    flag("-fsanitize=memory")
    flag("-fsanitize-address-use-after-scope")
    flag("-fsanitize-memory-use-after-dtor")
endif()
if(USE_CFISAN)
    flag("-fsanitize=cfi")
endif()
if(USE_STACKSAN)
    flag("-fsanitize=safe-stack")
endif()

