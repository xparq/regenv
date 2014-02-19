::  The project metadata dir is where the PROJECT.ini file is, or 
::  will be created at, unless preset via __PROJECT_METADATA_DIR.
::
::  NOTE: as PROJECT.ini is optional, we can't rely on its location.
::  If not found, we'll just use PROJECT_ROOT/{.prj-metadata,.prj}.
::
::  0. If already preset (perhaps in a shortcut command), use that. 
::  Check if the dir exists, and bail out if doesn't.
::  - Do not try to create it!
::  - DO NOT CHECK if PROJECT.ini is really there or not. The whole
::    point of a preset metadata dir option is to be able to set it
::    without a PROJECT.ini. (See [fix-stubborn-metadir].)
::    However, the INI locator assumes that it's in the meta dir,
::    so a different location can be configured by presetting both
::    __PROJECT_METADATA_DIR and __PROJECT_INI_FILE.
::  - Don't warn if no PROJECT.ini there: that will be dealt with 
::    later by the .ini loading/saving stuff.)
::
::  1. Do a recursive scan in __PROJECT_ROOT for PROJECT.ini.
::  If found, use the containing dir as the metadata subdir.
:: 
::  2. If PROJECT.ini is not found, use __PROJECT_ROOT.

:: 
:: 0
:: 
if "%__PROJECT_METADATA_DIR%"=="" goto 1
if exist %__PROJECT_METADATA_DIR% goto end
set ERROR=err_badmetadir
goto end

:1
::  
::  1
:: 
for /r %__PROJECT_ROOT% %%D in (PROJECT.ini) do if exist "%%D" (
	set __PROJECT_METADATA_DIR=%%~dpD
	:: NOTE: will be "" if found in the current dir (i.e. __PROJECT_METADATA_DIR == __PROJECT_ROOT)!
	goto found
)
goto no_PROJECT_ini

:found
::  Remove '__PROJECT_ROOT\' from the beginning to make it a relative path:
::  (For escaping a setlocal block, see: http://stackoverflow.com/questions/3852563/in-cmd-exe-how-can-you-get-one-variable-to-escape-the-setlocal-command)
::  NOTE however: if the metadata dir *is* the __PROJECT_ROOT, removing
::  "that part" would render __PROJECT_METADATA_DIR empty, which is an error.
::  So, only do it if the two are not equal. (See [fix-no-metadir]...)
::  Also note that we can get here with an empty __PROJECT_METADATA_DIR! (See (1).)
if "%__PROJECT_METADATA_DIR%"=="%__PROJECT_ROOT%" goto end
SETLOCAL ENABLEDELAYEDEXPANSION
set s=!__PROJECT_METADATA_DIR:%__PROJECT_ROOT%\=!
ENDLOCAL & set __PROJECT_METADATA_DIR=%s%
::  Remove trailing backslash, if any:
if not "%__PROJECT_METADATA_DIR%"=="" (
	if "%__PROJECT_METADATA_DIR:~-1%"=="\" (
		set __PROJECT_METADATA_DIR=%__PROJECT_METADATA_DIR:~0,-1%
	)
)
goto end

:2
:no_PROJECT_ini
::  
::  Just use __PROJECT_ROOT...
:: 
set __PROJECT_METADATA_DIR=%__PROJECT_ROOT%
goto end

:end
if "%__PROJECT_METADATA_DIR%"=="" if "%ERROR%"=="" set __PROJECT_METADATA_DIR=%__PROJECT_ROOT%
