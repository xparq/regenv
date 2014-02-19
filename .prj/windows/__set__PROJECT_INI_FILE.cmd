::  Set the variable __PROJECT_INI_FILE.
::
::  NOTE: this relies on __PROJECT_METADATA_DIR having been initialized,
::  unless __PROJECT_INI_FILE has been preset.
::
::  Although the location of PROJECT.ini may have alredy been precisely 
::  determined by the metadata dir detection, we cannot be sure here if
::  that was really done, or a PROJECT.ini does not even exists yet.


if not "%__PROJECT_INI_FILE%"=="" goto end
:: Just an assert...
if "%__PROJECT_METADATA_DIR%"=="" (
	set ERROR=err_nometadir
	call __print_error [FIXME] __PROJECT_METADATA_DIR must be set before touching PROJECT.ini!
	goto end
)

set __project_ini=PROJECT.ini

call __is_rel_path %__PROJECT_METADATA_DIR%
if "%RESULT%"=="1" (
	set __PROJECT_INI_FILE=%__PROJECT_ROOT%\%__PROJECT_METADATA_DIR%\%__project_ini%
) else (
	set __PROJECT_INI_FILE=%__PROJECT_METADATA_DIR%\%__project_ini%
)

if not exist %__PROJECT_INI_FILE% goto err_noprjini

goto end

:err_noprjini
set WARNING=err_noprjini
goto end

:end
set __project_ini=
