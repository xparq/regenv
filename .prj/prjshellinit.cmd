@echo off
rem echo Args: %*

PATH=V:\cmd;%PATH%


echo Adding VC10...
call vc10-2003-mixed __fallthrough__

echo Adding GCC...
call mingw __fallthrough__

:: Misc. project env. vars
set SRC_DIR=%__PROJECT_ROOT%\src
set BUILD_DIR=%__PROJECT_ROOT%\build
set TEST_DIR=%__PROJECT_ROOT%\test
set INCLUDE=%SRC_DIR%;%INCLUDE%


start %__PROJECT_METADATA_DIR%\farshell.lnk /w /rc %__PROJECT_ROOT%/ %*
exit