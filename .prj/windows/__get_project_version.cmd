for /f %%v in (%PROJECT_VERSION_FILE%) do set __project_version=%%v

if not "%__project_version%"=="" goto end
call __get_timestamp
set __project_version=%__timestamp%
goto end

:end
