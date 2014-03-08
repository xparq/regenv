@set module=%1
@shift
cl /Zi /DDEBUG /EHsc /GL /DTEST_%module% /EHsc /Fe%module%.exe %SRC_DIR%\%module%.cpp %1 %2 %3 %4 %5 ^
/link /subsystem:console /OPT:REF /OPT:ICF user32.lib advapi32.lib
