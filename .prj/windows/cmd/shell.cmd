@echo off
if not "%1"=="__fallthrough__" cmd /k %0 __fallthrough__ %* & goto end

shift /1

PROMPT [%PROJECT_TAG%] %PROMPT%
doskey e=exit

if "%PROJECT_SHELL_INIT_FILE%"=="" goto end
if exist "%PROJECT_SHELL_INIT_FILE%" (
rem  Cannot use %* (as that's not adjusted by shift):
	call %PROJECT_SHELL_INIT_FILE% %1 %2 %3 %4 %5 %6 %7 %8 %9
) else (
	__print_warning Project shell init script "%PROJECT_SHELL_INIT_FILE%" doesn't exist.
)

:end
