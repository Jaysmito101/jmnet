@echo off
REM -lws2_32 is required if not using msvc
g++ test.cpp -o test.exe -lws2_32 -lstdc++
echo "Compiled"
pause
cls
test.exe
del test.exe
pause
cls