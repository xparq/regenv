@set module=%1
@shift
::cl /DTEST_%module% /EHsc /Fe%module%.exe ..\src\%module%.cpp %1 %2 %3 %4 %5 user32.lib advapi32.lib
g++ -std=c++11 -static -DTEST_%module% -o%module%.exe ..\src\%module%.cpp %1 %2 %3 %4 %5 -luser32 -ladvapi32

