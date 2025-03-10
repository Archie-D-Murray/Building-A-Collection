## Theme
Game made in a week with the theme of 'Building a Collection'.
Player collects familiars to help them defeat a swarm of enemies.
Just for fun, running with the flag `--no-fps-cap` removes frame cap

## Controls
WASD to move
Space to jump
Q to quit

## Screenshot:
![Demo](./share/Demo.png)

## Building From Source:
Due to developing on different platforms, the raylib library must be downloaded separatley
after downloading the library you should have the following directory structure:
![Downloaded library](./share/Project_Folder.png)
#### Linux:
 - Download appropriate raylib 5.5 library (https://github.com/raysan5/raylib/releases/tag/5.5)
 - For most this is (https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz)
 - Untar library to raylib-5.5/ in root project folder
 - Execute build command (requires clang++):
    ```command
    ./build.sh
    ```

#### Windows:
 - Download appropriate raylib 5.5 library (https://github.com/raysan5/raylib/releases/tag/5.5)
 - For most this is (https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_msvc16.zip)
 - Unzip library to raylib-5.5/ in root project folder
 - Execute build command (requires clang++) from a developer command prompt:
    ```command
    .\build.bat
    ```
 - Can optionally compile with CL compiler
    ```command
    .\build_cl.bat
    ```

## Third Party Assets:
#### SFX
 - Skull Enemy Hit - https://pixabay.com/sound-effects/metal-hit-cartoon-7118/
 - Skull Enemy Death - https://pixabay.com/sound-effects/rock-break-183794/
 - Player Hit, Bat Enemy Hit, Both Death (cut from) - https://pixabay.com/sound-effects/three-flesh-impacts-82553/
 - UI Click - https://pixabay.com/sound-effects/ui-click-43196/
 - UI Hover - https://pixabay.com/sound-effects/click-buttons-ui-menu-sounds-effects-button-7-203601/
#### BGM
The background audio tracks were compressed to be normalised but are otherwise unchanged
 - Combat: https://incompetech.com/music/royalty-free/mp3-royaltyfree/Stay%20the%20Course.mp3
   ```md
   "Stay the Course" Kevin MacLeod (incompetech.com)
   Licensed under Creative Commons: By Attribution 4.0 License
   http://creativecommons.org/licenses/by/4.0/   ```

 - Passive: https://incompetech.com/music/royalty-free/mp3-royaltyfree/Floating%20Cities.mp3
   ```md
   "Floating Cities" Kevin MacLeod (incompetech.com)
   Licensed under Creative Commons: By Attribution 4.0 License
   http://creativecommons.org/licenses/by/4.0/
   ```

#### Font
Alagard font: https://www.dafont.com/alagard.font
