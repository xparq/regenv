@echo off
rem 
rem  Cannot fully support [cmd-loose-coupling] yet (!!FIXME: WHY?)
rem  !!FIXME: will fail if basename(__PROJECT_ROOT) != PROJECT_TAG!
rem  !!FIXME: everything's still "pack..." here (after renamed to save.cmd).
rem 

set __pack_version_tag=%1

if not "%__PROJECT_ROOT%"=="" goto root_ok
echo Warning: __PROJECT_ROOT not set, using current dir: "%CD%"...
set __pack_root=%CD%
goto packdir
:root_ok
set __pack_root=%__PROJECT_ROOT%
pushd %__pack_root%

:packdir
if not "%PROJECT_ARCHIVE_DIR%"=="" goto arcdir_set
echo Warning: PROJECT_ARCHIVE_DIR not set, using "%__pack_root%"...
set __pack_dir=%__pack_root%
goto packname
:arcdir_set
if not exist %PROJECT_ARCHIVE_DIR% goto err_noarcdir
if not "%__PROJECT_ARCHIVE_SUBDIR%"=="" set __pack_dir=%__pack_root%\%__PROJECT_ARCHIVE_SUBDIR%
if not "%__PROJECT_ARCHIVE_ROOT%"==""   set __pack_dir=%__PROJECT_ARCHIVE_ROOT%

:packname
if not "%PROJECT_TAG%"=="" goto packname_ok
for /f %%I in ("%__pack_dir%") do set __pack_name=%%~nxI
echo Warning: PROJECT_TAG not set, using "%__pack_name%"...
goto version
:packname_ok
set __pack_name=%PROJECT_TAG%

:version
if not "%__pack_version_tag%"=="" goto version_set
if "%PROJECT_VERSION_FILE%"=="" goto no_versionfile
for /f %%v in (%PROJECT_VERSION_FILE%) do set __pack_version_tag=%%v
if not "%__pack_version_tag%"=="" goto version_set
echo Warning: No version information. Using timestamp...
goto use_timestamp

:no_versionfile
echo Warning: PROJECT_VERSION_FILE not set and/or no version given; using timestamp...

:use_timestamp
call __get_timestamp

set __pack_version_tag=%__timestamp%
if "%__pack_version_tag%"=="" goto version_empty
:version_set
set __pack_version_tag=-%__pack_version_tag%
goto version_done
:version_empty
echo Warning: Empty version label (none given and no defaults?!). Check the environment!
:version_done

set packfile=%__pack_dir%\%__pack_name%%__pack_version_tag%.zip
if exist "%packfile%" goto err_exists

pushd %__pack_root%\..
if ERRORLEVEL 1 goto err_nocdup

rem !![fix-pack-srcdir-name]: the pack source dir is not always __pack_name!
zip -rS %packfile% %__pack_name%

popd
goto end

:err_noarcdir
echo --ERROR: Couldn't find %PROJECT_ARCHIVE_DIR%. No archive dir!
goto end

:err_exists
echo --ERROR: Archive file "%packfile%" already exists!
set __msg=         Duplicate use of version tag "%__pack_version_tag%"?
echo %__msg%
goto end

:err_noname
echo --ERROR: Project name not set, using default.
goto end

:err_nodir
echo --ERROR: Archive subdir not set.
goto end

:err_noroot
echo --ERROR: Project root dir not set.
goto end

:err_nocdup
echo --ERROR: Could not chdir to '..'.
goto end


:end
popd
set __savecd=
set __pack_version_tag=
set __pack_name=
set __pack_dir=
set __pack_root=
set __msg=
exit /b ERRORLEVEL
