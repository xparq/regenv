::  Create a new PROJECT.ini with autodetected values.
::  The ini file full path is specified in %1.

setlocal
set inifile=%1
if exist "%inifile%" goto err_ini_exists

::  This would be really easy, but, alas, not very user-friendly...:
::
::     set PROJECT_ >  "%inifile%"
::
::  So, we have to go over each cfg. setting & process them one by one:

for /f "usebackq" %%f in (`dir /b /O:N %__PROJECT_TOOLS_PLATFORM_DIR%\ini\var\create\*.cmd`) do call %__PROJECT_TOOLS_PLATFORM_DIR%\ini\var\create\%%f %inifile%

goto end


:err_ini_exists
call __print_error .ini file "%inifile%" already exists. Not touching it!
endlocal
set ERROR=err_ini_exists
goto end


:end
endlocal
