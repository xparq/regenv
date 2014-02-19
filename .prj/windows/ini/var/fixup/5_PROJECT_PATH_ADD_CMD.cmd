if "%PROJECT_PATH_ADD_CMD%"=="" goto end

if not "%PROJECT_PATH_ADD_CMD%"=="prepend" goto check_append
set PATH=%__PROJECT_TOOLS_PLATFORM_DIR%cmd;%PATH%
goto end

:check_append
if not "%PROJECT_PATH_ADD_CMD%"=="append" goto warn
set PATH=%PATH%;%__PROJECT_TOOLS_PLATFORM_DIR%cmd
goto end

:warn
call __print_warning PROJECT_PATH_ADD_CMD can be "append", "prepend" or empty (unset).

:end
