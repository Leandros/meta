@echo off
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Copyright (C) 2017-2017 Arvid Gerstmann                                   ::
::                                                                           ::
:: This file is part of meta.                                                ::
::                                                                           ::
:: meta is free software: you can redistribute it and/or modify              ::
:: it under the terms of the GNU General Public License as published by      ::
:: the Free Software Foundation, either version 3 of the License, or         ::
:: (at your option) any later version.                                       ::
::                                                                           ::
:: meta is distributed in the hope that it will be useful,                   ::
:: but WITHOUT ANY WARRANTY; without even the implied warranty of            ::
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             ::
:: GNU General Public License for more details.                              ::
::                                                                           ::
:: You should have received a copy of the GNU General Public License         ::
:: along with meta.  If not, see <http://www.gnu.org/licenses/>.             ::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: CONFIGURATION
set "LLVMPath=C:\Program Files\LLVM"
set "MSVCPath=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community"

echo "Cleanup old build ..."
rd /s /q build
::if not %errorLevel%==1 goto fail
mkdir build


if "%1"=="llvm" goto llvm
if "%1"=="msvc" goto msvc
goto help


:: LLVM
:llvm
echo "Configure LLVM ..."
echo "Path: %LLVMPath%"
echo.

:: Setup MSVC environment
call "%MSVCPath%\VC\Auxiliary\Build\vcvarsall.bat" amd64

:: Override CL.EXE with clang-cl.exe
set "PATH=%LLVMPath%\msbuild-bin;%PATH%"

:: Run CMake
cd build
cmake .. -G"NMake Makefiles JOM"

goto end


:: MSVC
:msvc
echo "Configure MSVC ..."
echo "Path: %MSVCPath%"
echo.

:: Setup MSVC environment
call "%MSVCPath%\VC\Auxiliary\Build\vcvarsall.bat" amd64

:: Run CMake
cd build
cmake .. -G"NMake Makefiles JOM"

goto end


:end
echo "Success ..."
goto quit

:fail
echo "ERROR"
goto quit

:help
echo "USAGE: configure.bat [llvm/msvc]"
:quit

