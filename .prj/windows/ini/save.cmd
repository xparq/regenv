::  Save a new PROJECT.ini with autodetected defaults.
::  The ini file full path is specified in %1.

set inifile=%1
if exist "%inifile%" goto err_ini_exists

:: No ini file yet, go 'create'...
call %__PROJECT_TOOLS_PLATFORM_DIR%\ini\create %*
goto end


:err_ini_exists
call __print_error .ini file "%inifile%" already exists. Updating is not yet supported!
endlocal
set ERROR=err_ini_exists
goto end


:end
set inifile=