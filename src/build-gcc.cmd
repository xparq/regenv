@set exe=..\test\gcc\regenv.exe
g++ -std=c++11 -static -o%exe% -Os regenv.cpp -luser32 -ladvapi32
strip %exe%
