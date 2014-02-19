@echo off
setlocal
if "%CMDEXTVERSION%"=="" goto err_nocmdext

:: -------------------------------------------------------------------------
::  Set __PROJECT_TOOLS_ROOT and __PROJECT_TOOLS_PLATFORM_DIR...
:: 
::  As we don't yet know where we are, where the other scripts are, we need
::  to figure it out ourselves, before trying to call anything.
::  
::  In the current version (from 0.043), the __PROJECT_TOOLS_PLATFORM_DIR 
::  is where the internal scripts (including this!) are, and the 
::  __PROJECT_TOOLS_ROOT is the parent dir of that.
:: 
set __PROJECT_TOOLS_PLATFORM_DIR=%~dp0
PATH=%__PROJECT_TOOLS_PLATFORM_DIR%;%__PROJECT_TOOLS_PLATFORM_DIR%tools;%PATH%
::
::  Phew. :) Now we can call other components...
::
call __get_parent_dir
if  "%__PROJECT_TOOLS_ROOT%"=="" set __PROJECT_TOOLS_ROOT=%__parent_fullpath_noslash%

:: -------------------------------------------------------------------------
::  Set __PROJECT_TOOLS_NAME...
:: 
::  (Although the product name will be set as PROJECT_NAME in PROJECT.ini,
::  do *NOT* use that, but get it from this internal file:
::  __PROJECT_TOOLS_ROOT/PRODUCT_NAME.
::  Otherwise we would incorrectly get the name of the *host* project if
::  PRJ =/= prjtools, obviously... ;) )
::  Note:: we couldn't just set it in PROJECT.ini anyway, as we are too far 
::  from being able to just load it here -- and, er, well, also because that 
::  would (again) belong to the *host* project! :) Let alone the fact that
::  PROJECT.ini may be completely missing (e.g. first run).
::
for /f "delims=" %%v in (%__PROJECT_TOOLS_ROOT%\tool-metadata\PRODUCT_NAME) do set __PROJECT_TOOLS_NAME=%%v

:: -------------------------------------------------------------------------
::  Set __PROJECT_TOOLS_VERSION... 
:: 
::  (Although the tool version is stored in PROJECT_VERSION_FILE during the
::  development of this tool, do *NOT* take it from PROJECT_VERSION_FILE, 
::  but from the internally known, direct, fixed location:: 
::  __PROJECT_TOOLS_ROOT/VERSION. 
::  Otherwise we would incorrectly get the version of the *host* project 
::  if PRJ =/= prjtools (obviously ;) )...)
::
for /f %%v in (%__PROJECT_TOOLS_ROOT%\tool-metadata\VERSION) do set __PROJECT_TOOLS_VERSION=%%v

:: -------------------------------------------------------------------------
::  Set __PROJECT_TOOLS_CFG_VERSION... 
:: 
::  See [add-cfg-ver].
::
for /f %%v in (%__PROJECT_TOOLS_ROOT%\tool-metadata\CFG_VERSION) do set __PROJECT_TOOLS_CFG_VERSION=%%v

:: =========================================================================
::  Now we can print the banner...
:: =========================================================================
::  !!FIXME:: The silent UNIX way would be better; make it optional!
call __print_tool_banner

:: -------------------------------------------------------------------------
::  Set __PROJECT_ROOT... (Not called "PROJECT_ROOT", because this cannot 
::  really be configured by the user, only implicitly determined.)
::
::  Unless it's pre-set, use the parent's parent of this script's dir,
::  as we can assume being underneath the project dir, which is two floors
::  above us in this version (0.043)...
::  (!!FIXME:: This will become incorrect in case of shared (absolute) 
::  tool dirs, as soon as that gets supported.)
:: 
call __set__PROJECT_ROOT

:: =========================================================================
::  Now we can CD to __PROJECT_ROOT (something the internals assume)...
:: =========================================================================
set __PROJECT_LAST_CWD=%CD%
pushd %__PROJECT_ROOT%

::  -------------------------------------------------------------------------
::   Find and set __PROJECT_METADATA_DIR...
::  
call __set__PROJECT_METADATA_DIR
if not "%ERROR%"=="" goto err_badmetadir

::  -------------------------------------------------------------------------
::   Load & fixup project env. (possibly also saving it if not yet done).
::  
call __setup_prj_env
if not "%ERROR%"=="" goto err_env_broken

::  =========================================================================
::   Now we are OK to create a command frame...
::  =========================================================================

::  -------------------------------------------------------------------------
::   OK, do what's commanded...
::
if not "%1"=="" goto use_given_cmd
set cmd=help
goto begin_cmd_frame

:use_given_cmd
set cmd=%1
shift

:begin_cmd_frame

::  !!FIXME:: Is this hack useful (here)?! Should it be moved to shell.cmd?
::   Add PROJECT_PATH_PREPEND_DIR & ..._APPEND_DIR (if set) to the PATH.
if not "%PROJECT_PATH_PREPEND_DIR%"=="" PATH=%PROJECT_PATH_PREPEND_DIR%;%PATH%
if not "%PROJECT_PATH_APPEND_DIR%"==""  PATH=%PATH%;%PROJECT_PATH_APPEND_DIR%

call __check_custom_cmd noprint %cmd%

if not "%ERROR%"=="" goto check_builtin
::   DO NOT USE 'call' here (to ensure a new command frame and to allow .EXEs, too)!
::   DO NOT APPEND .cmd here (to let Windows load any other executable, too).
::   DO NOT USE %* here, as the shift command above did not update it!
::  ****
::  This %cmd% is user-supplied, so it may just blow up everything (if at all exists...)!
::  ****
%PROJECT_CUSTOM_CMD_DIR%\%cmd% %1 %2 %3 %4 %5 %6 %7 %8 %9
goto end_frame

:check_builtin
set ERROR=
set ERROR_MESSAGE=
call __check_cmd %cmd%

if not "%ERROR%"=="" goto end_frame
::   DO NOT USE 'call' here (to ensure a new command frame and to allow .EXEs, too)!
::   DO NOT USE %* here, as the shift command above did not update it!
pushd %__PROJECT_ROOT%
%__PROJECT_TOOLS_PLATFORM_DIR%cmd\%cmd%.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9

:end_frame
if not "%ERROR%"=="" goto %ERROR%

::  -------------------------------------------------------------------------
::  Errors, cleanup, good bye...
::

:err_badcmd
goto end

:err_nocmdext
call __print_error Windows command-line extensions (for CMD.EXE) must be enabled!
goto end

:err_env_broken
::   We are OK to call internals, but not to exec prj cmds here.
call __print_error Failed to setup project parameters.
echo The (almost surely) broken environment is:
set PROJECT_
set __PROJECT_
echo PATH=%PATH%
echo PROMPT=%PROMPT%
echo CD=%CD%
goto end

:err_badmetadir
call __print_error Requested using non-existing project metadata dir "%__PROJECT_METADATA_DIR%"!
goto end

:end
::   Make sure never going here before issuing a pushd!...
popd

::   I guess this is stupid, but maybe OK as a reminder for "some error things";) :
endlocal
exit /b %ERROR%
