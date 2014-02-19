:: but as it is NOT the same as the current tool version
:: (that would negate the whole point of it), it would
:: require some semi-automatic tracking mechanism, like
:: .
echo # Which is the first tool version that can handle this config?>> %inifile%
echo PROJECT_TOOLS_CFG_VERSION=%__PROJECT_TOOLS_CFG_VERSION%>> %inifile%
echo. >> %inifile%
