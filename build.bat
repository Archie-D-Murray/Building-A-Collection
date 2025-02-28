@echo off
echo Starting compile at %TIME%...
clang++ -o Building-A-Collection.exe .\main.cpp .\src\*.cpp -I .\raylib-5.5\include\ .\raylib-5.5\lib\raylib.lib -lmsvcrt -lshell32 -luser32 -lopengl32 -lwinmm -lgdi32 -nostdlib -std=c++20 -g -O0 -Wno-c99-designator
if %ERRORLEVEL% == 0 goto compile_success
exit

:compile_success
echo Sucessful compile at %TIME%
if '%~1' == '' goto capped_fps_run
echo Starting with uncapped FPS
.\Building-A-Collection.exe --no-fps-cap

:capped_fps_run
echo Starting with capped FPS
.\Building-A-Collection.exe
