@echo off
echo Starting compile at %TIME%...
cl /std:c++20 /EHsc main.cpp src/*.cpp /o Making-A-Collection.exe -I .\raylib-5.5\include\  /MD /link .\raylib-5.5\lib\raylib.lib user32.lib shell32.lib gdi32.lib winmm.lib kernel32.lib opengl32.lib
if %ERRORLEVEL% == 0 goto compile_success
del *.obj
exit

:compile_success
del *.obj
echo Sucessful compile at %TIME%
if '%~1' == '' goto capped_fps_run
echo Starting with uncapped FPS
.\Making-A-Collection.exe --no-fps-cap

:capped_fps_run
echo Starting with capped FPS
.\Making-A-Collection.exe
