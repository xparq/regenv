:: Set the version file location from the configured value, or
:: to an internal default.
:: Also create the version file, if does not yet exist.

:: Just an assert...
if "%__PROJECT_METADATA_DIR%"=="" (
	call __print_error [FIXME] __PROJECT_METADATA_DIR is empty in %0!
	set ERROR=err_nometadir
	goto end
)

set ERROR=

:: Use the configured value, if set, or 'VERSION' in the meta dir.
:: (See also ini/var/create, how the same default is set there!)
if not "%PROJECT_VERSION_FILE%"=="" (
	set prjverfile=%PROJECT_VERSION_FILE%
) else (
	set prjverfile=%__PROJECT_METADATA_DIR%\VERSION
)

call __check_prjfile noprint %prjverfile%
if "%ERROR%"=="" goto set

:create
call __print_warning Version counter "%prjverfile%" not found, creating...
echo.> %prjverfile%
set ERROR=
call __check_prjfile %prjverfile%
if not "%ERROR%"=="" goto err_noprjfile

:set
if "%PROJECT_VERSION_FILE%"=="" set PROJECT_VERSION_FILE=%prjverfile%
goto end

:err_noprjfile
call __print_error Couldn't find/create %prjverfile%. Not using it for version tracking.
goto end

:end
set prjverfile=
