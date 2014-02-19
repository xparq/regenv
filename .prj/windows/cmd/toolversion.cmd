@echo off

echo Product name: %__PROJECT_TOOLS_NAME%
echo Version: %__PROJECT_TOOLS_VERSION%
::!!FIXME: hardcoded path:
echo Change log: %__PROJECT_TOOLS_ROOT%\doc\CHANGES.txt

if not "%1"=="changes" goto arg2
echo.
echo Change log:
echo.
type %__PROJECT_TOOLS_ROOT%\doc\CHANGES.txt
goto end


:arg2
goto end


:end