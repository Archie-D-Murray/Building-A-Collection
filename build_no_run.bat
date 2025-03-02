@echo off
clang++ -o Building-A-Collection.exe .\main.cpp .\src\*.cpp -I .\raylib-5.5\include\ .\raylib-5.5\lib\raylib.lib -lmsvcrt -lshell32 -luser32 -lopengl32 -lwinmm -lgdi32 -nostdlib -std=c++20 -g -O0 -Wno-c99-designator
if %ERRORLEVEL% == 0 echo Compile Successful
