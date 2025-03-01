#include "raylib.h"
#include "src/game.hpp"
#include "src/render_data.hpp"
#include <string.h>

Vector2 SetWindowDefaults(bool capFPS = true) {
    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor);
    int height = GetMonitorHeight(monitor);
    SetWindowSize(width * 0.75f, height * 0.75f);
    SetWindowPosition(width * 0.5f - width * 0.375f, height * 0.5f - height * 0.375f);
    if (capFPS) {
        int maxFPS = GetMonitorRefreshRate(monitor);
        SetTargetFPS(maxFPS);
    }
    TraceLog(LOG_INFO, "Initialized window of size: %dx%d", (int)(width * 0.75f), (int)(height * 0.75f));
    return { width * 0.75f, height * 0.75f };
}

bool CapFPS(int argc, char* argv[]) {
    if (argc < 2) {
        return true;
    }
    return strcmp(argv[1], "--no-fps-cap") != 0;
}

int main(int argc, char* argv[]) {
    bool capFPS = CapFPS(argc, argv);
    InitWindow(0, 0, "Making a Collection");
    Sprites::RenderData data = Sprites::RenderData("./atlas.png", "./Alagard.ttf");
    Vector2 screenSize = SetWindowDefaults(capFPS);
    InitAudioDevice();
    std::vector<Game> sceneStack;
    sceneStack.push_back(Game(Menu, screenSize, &data));
    sceneStack.back().Init();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10, 10);
        State newState = sceneStack.back().Update(GetFrameTime());
        if (newState != None) {
            TraceLog(LOG_INFO, "Switching scene");
            sceneStack.back().Shutdown();
            sceneStack.pop_back();
            sceneStack.push_back(Game(newState, screenSize, &data));
            sceneStack.back().Init();
        }
        EndDrawing();
    }

    sceneStack.back().Shutdown();
    data.Unload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
