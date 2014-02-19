if "%1"=="" goto end
if not exist "%1" goto warn_notex


::  This crap would fail with the order of the space & tab inverted:
for /f "eol=# tokens=1* delims==	 " %%i in (%1) do call :assign %%i %%j
goto end

::  Don't ask about the trimming method, it's just the result of some
::  "experimental programming"... ;-o
:assign
setlocal
set key=%1
for /f "tokens=1,*" %%s in ("%*") do ( 
	set val=%%t
)
endlocal & set %key%=%val%
goto :eof


:warn_notex
call __print_warning .ini file "%1" does not exist.
goto end


:end
