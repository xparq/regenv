:: Need to make sure cscript can find the script!

for /f "usebackq" %%t in (`cscript /nologo %~dp0__get_timestamp.js`) do set __timestamp=%%t
