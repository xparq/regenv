@set module=%1
@shift
cl /DTEST_%module% /EHsc /Fe%module%.exe ..\%module%.cpp %1 %2 %3 %4 %5 user32.lib advapi32.lib
