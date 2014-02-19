if not "%1"=="noprint" goto noprint_ok
set __noprint=%1
shift
:noprint_ok
set __fullpath=%1

if exist "%__fullpath%" goto ok

set ERROR=err_nofile
set ERROR_MESSAGE=File not found: "%__fullpath%".
if not "%__noprint%"=="" goto end
call __print_error
goto end

:ok

:end
set __fullpath=
set __noprint=
