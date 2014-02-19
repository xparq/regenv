::  If no path has been specified, use "." (which means the __PROJECT_ROOT).
::  If the path is relative, it's also stored in __PROJECT_ARCHIVE_SUBDIR, 
::  else in __PROJECT_ARCHIVE_ROOT.

set projectarchivedir=%PROJECT_ARCHIVE_DIR%
if "%projectarchivedir%"=="" set projectarchivedir=.

:check
if not exist %projectarchivedir% goto err_noarcdir

:set
if "%PROJECT_ARCHIVE_DIR%"=="" set PROJECT_ARCHIVE_DIR=%projectarchivedir%

set __PROJECT_ARCHIVE_SUBDIR=
set __PROJECT_ARCHIVE_ROOT=
call __is_rel_path %PROJECT_ARCHIVE_DIR%
if "%RESULT%"=="1"   set __PROJECT_ARCHIVE_SUBDIR=%PROJECT_ARCHIVE_DIR%
if not "%RESULT%"=="1" set __PROJECT_ARCHIVE_ROOT=%PROJECT_ARCHIVE_DIR%
goto end

:err_noarcdir
call __print_error Couldn't find %projectarchivedir%. No archive dir!
set ERROR=err_noarcdir
goto end

:end
set projectarchivedir=
