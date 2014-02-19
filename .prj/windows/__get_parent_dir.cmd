::  Script's drive:
set __parent_drive=%~d0
::  Script's dir (with trailing slash removed):
set __dir=%~p0
set __dir=%__dir:~0,-1%

if "%1"=="" goto base_dir_ok
set __dir=%1
if "%__dir:~-1%" == "\" set __dir=%__dir:~0,-1%

:base_dir_ok

if not "%__dir%"=="" goto not_root
set __parent_dir=\

:not_root
::  Parent dir (with trailing slash removed):
for /f %%i in ("%__dir%") do set __parent_dir=%%~pi

set __parent_dir_noslash=%__parent_dir:~0,-1%
set __parent_fullpath=%__parent_drive%%__parent_dir%

::  Force trailing backslash to __parent_fullpath_noslash in case of the 
::  root dir (otherwise a faulty "D:dir..." could result in hard-to-debug 
::  "catastrophic" errors):
set __parent_fullpath_noslash=%__parent_drive%%__parent_dir_noslash%
if "%__parent_dir_noslash%"=="" set __parent_fullpath_noslash=%__parent_fullpath_noslash%\

:end
set __dir=
