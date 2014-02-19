:: -------------------------------------------------------------------------
::  Load the (optional) PROJECT.ini...
:: 

call __set__PROJECT_INI_FILE
if not "%ERROR%"=="" goto end

if     exist %__PROJECT_INI_FILE% call %__PROJECT_TOOLS_PLATFORM_DIR%\ini\load %__PROJECT_INI_FILE%
if not exist %__PROJECT_INI_FILE% call __print_warning No %__PROJECT_INI_FILE%. (Will generate one.)

:fixup
:: -------------------------------------------------------------------------
::  Make sure the environment is fine after loading the .INI...
::
call %__PROJECT_TOOLS_PLATFORM_DIR%\ini\fixup
if not "%ERROR%"=="" goto end

:: -------------------------------------------------------------------------
::  Create an initial PROJECT.ini...
::  
if exist "%__PROJECT_INI_FILE%" goto end
call __print_warning Creating %__PROJECT_INI_FILE% with defaults...

::  Error handling is done here internally:
call %__PROJECT_TOOLS_PLATFORM_DIR%\ini\create %__PROJECT_INI_FILE%


:end

