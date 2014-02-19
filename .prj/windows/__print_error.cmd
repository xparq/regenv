set msg=%ERROR_MESSAGE%
if not "%*"=="" set msg=%*
if not "%msg%"=="" echo --ERROR: %msg%

set msg=
