if not "%1"=="noprint" goto noprint_ok
set __noprint=%1
shift
:noprint_ok
set __prjrelpath=%1

if exist "%__prjrelpath%" goto ok

set ERROR=err_noprjfile
set ERROR_MESSAGE=Project file not found: "%__prjrelpath%".
if not "%__noprint%"=="" goto end
call __print_error
goto end

:ok

:end
set __prjrelpath=
set __noprint=
