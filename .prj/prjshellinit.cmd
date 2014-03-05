@echo off
rem echo Args: %*

PATH=V:\cmd;%PATH%

echo Adding VC10...
call vc10-2003-mixed __fallthrough__

echo Adding GCC...
call mingw __fallthrough__

start %__PROJECT_METADATA_DIR%\farshell.lnk /w /rc %__PROJECT_ROOT%/ %*
exit