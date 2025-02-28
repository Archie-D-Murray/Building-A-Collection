#include "raylib.h"
#include "src/game.hpp"

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
    Vector2 screenSize = SetWindowDefaults(capFPS);
    InitAudioDevice();

    Game game = Game(screenSize);
    game.Init();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10, 10);
        game.Update(GetFrameTime());
        EndDrawing();
    }

    game.Shutdown();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
