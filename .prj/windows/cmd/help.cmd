@echo off

set cmd=%1
if not "%cmd%"=="" goto cmdhelp

::
:: List the available commands...
:: !!FIXME: Should be a separate command that help would just invoke.
:: !!FIXME: This is anyway just a toy here, not a real implementation!
::

echo Built-in commands:
:: Only .cmd at the moment...
for %%c in (%__PROJECT_TOOLS_PLATFORM_DIR%\cmd\*.cmd) do call :print_name %%c
echo.

if "%PROJECT_CUSTOM_CMD_DIR%"=="" goto end
if not exist "%PROJECT_CUSTOM_CMD_DIR%" (
	call __print_warning Custom command dir '%PROJECT_CUSTOM_CMD_DIR%' doesn't exist.
	goto end
)

echo Custom commands (located at: %PROJECT_CUSTOM_CMD_DIR%)^:
:: List only the basic executable types. (Should use %PATHEXT%, ideally...)
::
:: Note: as this shit cannot do non-recursive dir walking,
:: I need to check the for every item and skip those not
:: in the top dir. And hope that no users will set c:\ for 
:: the custom command dir... ;)
:: !!FIXME: This trick is BROKEN!
::         Well, at least very-very fragile, failing even if one (like me)
::         uses forward slashes instead of '\' in PROJECT_CUSTOM_CMD_DIR
::         (since CMD will use '\' anyway).
:: Ehh, now I know why I used 'dir' in this 'for' command originally...
:: OK, rid this crap, and do a separate for for each file type... :-/
::for /r %PROJECT_CUSTOM_CMD_DIR% %%D in (*.cmd *.bat *.exe *.js) do (
::	if not "%%~dpD"=="%PROJECT_CUSTOM_CMD_DIR%" call :print_name %%D
::)
for %%c in (%PROJECT_CUSTOM_CMD_DIR%\*.cmd) do call :print_name %%c
for %%c in (%PROJECT_CUSTOM_CMD_DIR%\*.bat) do call :print_name %%c
for %%c in (%PROJECT_CUSTOM_CMD_DIR%\*.exe) do call :print_name %%c

goto end

:: Print the items indented and with no extension.
:print_name
setlocal
set _indent_=^ ^ ^ ^ 
echo %_indent_%%~n1
endlocal
goto end


:cmdhelp
::
::  Print help about a specific command.
::

call __check_cmd %cmd%
if not "%ERROR%"=="" goto end

echo Using the "%1" command:
echo (No information is avaialble yet, sorry.)
goto end


:end
