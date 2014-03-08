cl /Zi /EHsc /GL /Fo%BUILD_DIR%\tmp\ /Fe%TEST_DIR%\vc\regenv.exe %SRC_DIR%\regenv.cpp ^
/link /subsystem:console /OPT:REF /OPT:ICF /LTCG user32.lib advapi32.lib
