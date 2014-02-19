@echo off
cls
echo EXISTING RELATIVE:
call __is_existing_rel_path.cmd .
echo rel=%RESULT%
call __is_existing_rel_path.cmd ..
echo rel=%RESULT%
call __is_existing_rel_path.cmd cmd
echo rel=%RESULT%
call __is_existing_rel_path.cmd tools\
echo rel=%RESULT%
call __is_existing_rel_path.cmd cmd/./
echo rel=%RESULT%
call __is_existing_rel_path.cmd /cmd/
echo rel=%RESULT%
echo NON-EXISTING RELATIVE:
call __is_existing_rel_path.cmd /__/
echo rel=%RESULT%
call __is_existing_rel_path.cmd something_strange
echo rel=%RESULT%
echo ABSOLUTE:
call __is_existing_rel_path.cmd c:/kaki
echo rel=%RESULT%
call __is_existing_rel_path.cmd c:\kaki
echo rel=%RESULT%
call __is_existing_rel_path.cmd c:
echo rel=%RESULT%
call __is_existing_rel_path.cmd d:\something
echo rel=%RESULT%
call __is_existing_rel_path.cmd //kaki
echo rel=%RESULT%
call __is_existing_rel_path.cmd \\kaki
echo rel=%RESULT%
echo NON-RELATIVE and NON-ABSOLUTE CRAP:
call __is_existing_rel_path.cmd d:
echo rel=%RESULT%
call __is_existing_rel_path.cmd d:.
echo rel=%RESULT%
call __is_existing_rel_path.cmd d:..\
echo rel=%RESULT%
call __is_existing_rel_path.cmd d:../thing
echo rel=%RESULT%
call __is_existing_rel_path.cmd con:
echo rel=%RESULT%
