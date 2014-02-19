::  This should be run *after* .\*_PROJECT_TAG.cmd!

if not "%PROJECT_NAME%"=="" goto end

set ERROR=
if "%PROJECT_TAG%"=="" (
	call __print_error PROJECT_TAG should not be empty! ^(How did this happen??^)
	set ERROR=err_noprjtag
) else (
	set PROJECT_NAME=%PROJECT_TAG%
)

:end
