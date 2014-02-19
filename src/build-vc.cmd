cl /EHsc /Fobuild\ /Fe..\test\vc\winenv.exe /GL winenv.cpp ^
/link /subsystem:console /OPT:REF /OPT:ICF /LTCG user32.lib advapi32.lib
