clang++ -o Building-A-Collection.exe .\main.cpp .\src\*.cpp -I .\raylib-win\include\ .\raylib-win\lib\raylib.lib -lmsvcrt -lshell32 -luser32 -lopengl32 -lwinmm -lgdi32 -nostdlib -std=c++20 -g -O0
if %ERRORLEVEL% == 0 Building-A-Collection.exe
