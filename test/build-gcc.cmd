@set exe=%TEST_DIR%\gcc\regenv.exe
g++ -std=c++11 -static -o%exe% -Os %SRC_DIR%\regenv.cpp -luser32 -ladvapi32
strip %exe%
