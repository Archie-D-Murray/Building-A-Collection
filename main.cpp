#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "src/game.hpp"

Vector2 SetWindowDefaults() {
    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor);
    int height = GetMonitorHeight(monitor);
    int maxFPS = GetMonitorRefreshRate(monitor);
    SetWindowSize(width * 0.75f, height * 0.75f);
    SetWindowPosition(width * 0.5f - width * 0.375f, height * 0.5f - height * 0.375f);
    SetTargetFPS(maxFPS);
    TraceLog(LOG_INFO, "Initialized window of size: %dx%d", (int)(width * 0.75f), (int)(height * 0.75f));
    return { width * 0.75f, height * 0.75f };
}

int main(void) {
    InitWindow(0, 0, "Making a Collection");
    Vector2 screenSize = SetWindowDefaults();
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
