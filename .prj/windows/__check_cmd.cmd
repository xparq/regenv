::  NOTE: We are in __PROJECT_ROOT now!

if not "%1"=="noprint" goto noprint_ok
set __noprint=%1
shift
:noprint_ok
set __cmd=%1

if exist "%__PROJECT_TOOLS_PLATFORM_DIR%cmd\%__cmd%.cmd" goto ok

set ERROR=err_badcmd
set ERROR_MESSAGE=Unknown command "%__cmd%".
if not "%__noprint%"=="" goto end
call __print_error
goto end

:ok

:end
set __cmd=
set __noprint=
