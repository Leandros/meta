@echo off

:: CONFIGURATION
set "LLVMPath=C:\\Program Files\\LLVM"
set "MSVCPath=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community"

echo "Cleanup old build ..."
rd /s /q build
::if not %errorLevel%==1 goto fail
mkdir build


if "%1"=="llvm" goto llvm
if "%1"=="msvc" goto msvc
goto end


:: LLVM
:llvm
echo "Configure LLVM ..."
echo "Path: %LLVMPath%"
echo.

:: Run CMake
cd build
cmake .. -G"Ninja" ^
    -DBUILD_TESTS:BOOL=YES ^
    -DCMAKE_C_FLAGS=TRUE ^
    -DCMAKE_CXX_FLAGS=TRUE ^
    -DCMAKE_C_COMPILER="%LLVMPath%\\bin\\clang.exe" ^
    -DCMAKE_CXX_COMPILER="%LLVMPath%\\bin\\clang++.exe" ^
    -DCMAKE_LINKER="%LLVMPath%\\bin\\lld-link.exe"

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
cmake .. -G"NMake Makefiles JOM" -DBUILD_TESTS:BOOL=YES

goto end


:end
echo "Success ..."

:fail
echo "ERROR"

