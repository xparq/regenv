@echo off

set module=%1

cls
echo on
@call build_module %module%
@echo off
if errorlevel 1 goto build_error

echo About to test module '%module%'. Press a key...
pause > nul

cls
call run_module %module%

rem  Ehh... if ERRORLEVEL n fires if ERRORLEVEL >= n.
rem  So "not errorlevel 0" DOES NOT mean ERRORLEVEL !=0, but ERRORLEVEL < 0.
rem  BTW, ERRORLEVEL is *signed*. Also: no < = > != operators to check it...
if errorlevel 1 goto test_failed
if not errorlevel 0 goto test_failed
goto test_succeeded

:build_error
echo FAILED TO BUILD MODULE '%module%'...
goto :end

:test_failed
echo TEST FAILED!
goto :end

:test_succeeded
echo OK.
goto :end

:end
echo Press a key...
pause > nul
