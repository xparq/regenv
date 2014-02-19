rem WTF:
rem @for /f %%P in ("%0") do @call %%~dpP\prj shell ./src %*

: prj will cd here, so ./ is ok...
%~dp0prj shell ./src %*
