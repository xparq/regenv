::  See __is_rel_path.txt.

set __somepath=%1
set RESULT=

::  Empty is relative...
if "%__somepath%"=="" goto rel

::  One that starts with a (back)slash may be (considered) relative...
set __c=%__somepath:~0,1%
if "%__c%"=="\" goto maybe_rel
if "%__c%"=="/" goto maybe_rel
goto noslash
:maybe_rel
set __c=%__somepath:~1,1%
if "%__c%"=="\" goto end
if "%__c%"=="/" goto end
goto rel

:noslash
::  One with a colon (probably a drive letter) is considered not relative...
for /f "tokens=1,* delims=:" %%d in ("%__somepath%") do if not "%%d%%e"=="%__somepath%" goto end

:rel
set RESULT=1

:end
set __somepath=
set __c=
