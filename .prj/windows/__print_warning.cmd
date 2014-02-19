set msg=%WARNING_MESSAGE%
if not "%*"=="" set msg=%*
if not "%msg%"=="" echo Warning: %msg%

set msg=
