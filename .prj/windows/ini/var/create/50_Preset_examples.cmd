echo. >>	%inifile%
echo # >>	%inifile%
echo # Preset internal parameters:>>	%inifile%
echo # >>	%inifile%
echo # NOTE:>>	%inifile%
echo # The internal __PROJECT_... vars cannot be set here, as they have>>	%inifile%
echo # already been initialized (and used) before loading this file.>>	%inifile%
echo # However, they can be set from customized shortcut commands, so>>	%inifile%
echo # they are still listed here, for your reference (only!).>>	%inifile%

echo # >>	%inifile%

::  This is not enough! We have to prepend '#' to every line... ;-o
rem set __PROJECT_>>	%inifile%

::  Er, and this is not OK either, as it'd also list the really internal 
::  ones, too, which can't even be modified at all... ;-/
rem for /f "usebackq eol=# tokens=1,2 delims== " %%i in (`set __PROJECT_`) do echo # %%i=%%j>>	%inifile%

::  So, we just put them by hand...
echo # This must be a full path:>>				%inifile%
echo # [DON NOT SET] __PROJECT_ROOT=%__PROJECT_ROOT%>>				%inifile%
echo # This can be a relative (to __PROJECT_ROOT) or an absolute path:>>	%inifile%
echo # [DO NOT SET] __PROJECT_INI_FILE=%__PROJECT_INI_FILE%>>		%inifile%
echo # This can be a relative (to __PROJECT_ROOT) or an absolute path:>>	%inifile%
echo # [DO NOT SET] __PROJECT_METADATA_DIR=%__PROJECT_METADATA_DIR%>>		%inifile%
