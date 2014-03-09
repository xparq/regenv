@echo off

if not "%1" == "" (
	set list=%1
) else (
	set list=*List*.cpp
)

for %%X in (%list%) do (
	pushd test
	call test_module %%~nX
	popd
	if errorlevel 1 goto failure
	if not errorlevel 0 goto failure
)
goto end

:failure
echo BAILING OUT...
goto end

:end
