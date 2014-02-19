@echo off

setlocal
set mode=%1
shift
if  "%mode%"=="" set mode=show
goto %mode%


:help
endlocal

echo Usage: env [show ^| save [inifile] ^| help]
echo The default mode is: show.
echo.
goto end


:show
endlocal

rem echo Project environment:
rem echo.
set PROJECT_
set __PROJECT_
echo PATH=%PATH%
echo PROMPT=%PROMPT%
echo CD=%CD%
goto end


:save
endlocal

set inifile=%1
if "%inifile%"=="" (
	echo Warning: No file name specified, using '%__PROJECT_INI_FILE%'...
	set inifile=%__PROJECT_INI_FILE%
)
if not "%__PROJECT_LAST_CWD%"=="" pushd %__PROJECT_LAST_CWD%

::  Error handling is done here internally:
%__PROJECT_TOOLS_PLATFORM_DIR%\ini\save %inifile%

if not "%__PROJECT_LAST_CWD%"=="" popd
goto end


:end
