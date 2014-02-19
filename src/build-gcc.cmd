@set exe=..\test\gcc\winenv.exe
g++ -std=c++11 -static -o%exe% -Os winenv.cpp -luser32 -ladvapi32
strip %exe%
