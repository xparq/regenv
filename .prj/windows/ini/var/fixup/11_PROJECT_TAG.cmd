if not "%PROJECT_TAG%"=="" goto end
for /f %%I in ("%__PROJECT_ROOT%") do set PROJECT_TAG=%%~nxI

:end
