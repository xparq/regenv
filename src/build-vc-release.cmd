cl /EHsc /Fobuild.tmp\ /Fe..\test\vc\regenv.exe /GL regenv.cpp ^
/link /subsystem:console /OPT:REF /OPT:ICF /LTCG user32.lib advapi32.lib
