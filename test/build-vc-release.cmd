cl /EHsc /MT /Fo%BUILD_DIR%\tmp\ /Fe%TEST_DIR%\vc\regenv.exe %SRC_DIR%\regenv.cpp ^
/link /subsystem:console /OPT:REF /OPT:ICF user32.lib advapi32.lib
