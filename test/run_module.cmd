@echo off

set module=%1
shift 

call %module%.exe %1 %2 %3 %4 %5 %6 %7
