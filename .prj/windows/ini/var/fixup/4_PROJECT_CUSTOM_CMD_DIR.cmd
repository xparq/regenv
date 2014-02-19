::  If empty, we need to check that in prj.cmd and __check_custom_cmd_dir, 
::  otherwise we'd end up looking for \cmd...
::  Also, if it's just ".", at least issue a warning... (There can be many
::  other incorrect names of the __PROJECT_ROOT, though...)

if "%PROJECT_CUSTOM_CMD_DIR%"=="." goto warn
if "%PROJECT_CUSTOM_CMD_DIR%"=="%__PROJECT_ROOT%" goto warn
goto end

:warn
__print_warning Proxy commands would cause infinite loops if CUSTOM_CMD_DIR = PROJECT_ROOT!

:end
