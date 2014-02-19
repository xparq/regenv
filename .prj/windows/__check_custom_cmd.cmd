::  We are in __PROJECT_ROOT now!
::  No lookup if PROJECT_CUSTOM_CMD_DIR is not set!

if not "%1"=="noprint" goto noprint_ok
set __noprint=%1
shift
:noprint_ok
set __cmd=%1

if not "%PROJECT_CUSTOM_CMD_DIR%"=="" goto do_check
::  This is not really an error, but prj.cmd is too stupid now, so...
::  But at least we don't print anything. ;)
set ERROR=err_nocustomcmddir
goto end

:do_check

if exist "%PROJECT_CUSTOM_CMD_DIR%\%__cmd%.cmd" goto ok
if exist "%PROJECT_CUSTOM_CMD_DIR%\%__cmd%.bat" goto ok
if exist "%PROJECT_CUSTOM_CMD_DIR%\%__cmd%.exe" goto ok

set ERROR=err_badcmd
set ERROR_MESSAGE=Unknown custom command "%__cmd%" (not found in %__PROJECT_ROOT%%PROJECT_CUSTOM_CMD_DIR%).
if not "%__noprint%"=="" goto end
call __print_error
goto end

:ok

:end
set __cmd=
set __noprint=
