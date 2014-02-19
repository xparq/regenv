::
::  All __set_xxx scripts (except for __set_default_xxx) assume
::  __PROJECT_ROOT set, and only set any PROJECT_... and __... var 
::  if it's still empty.
::
::  (Any unhandled ERROR values are propagated back to prj.cmd.)
::

if "%__PROJECT_ROOT%"=="" goto err_noprjroot

for /f "usebackq" %%f in (`dir /b /O:N %__PROJECT_TOOLS_PLATFORM_DIR%\ini\var\fixup\*.cmd`) do call %__PROJECT_TOOLS_PLATFORM_DIR%\ini\var\fixup\%%f

::  Remove surrounding whitespace from the user vars:
::  !!FIXME: Not willing to do a chop-loop for every single variable,
::  I could not find a proper way to remove trailing space from lines
::  that may themselves contain spaces. (I could only remove leading 
::  spaces but not both.)
:: for /f "usebackq tokens=*" %%i in (`set PROJECT_`) do (
:: 	for /f %%j in ("%%i") do echo 'set %%j'
:: )


:: !!!!!!!!!!
:: !! FIXME: should sanity-check the prj. env here!...
:: !!!!!!!!!!


goto end


:err_noprjroot
set ERROR=err_noprjroot
call __print_error Project root dir must not be empty.
goto end


:end
