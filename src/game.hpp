#pragma once
#include "player.hpp"
#include "render_data.hpp"

class Game {
    Sprites::RenderData renderData;
public:
    Vector2 screenSize;
    Vector2 pixelOffset;
    Player player;
    Camera2D worldCamera;
    Game(Vector2 screenSize);
    void Init();
    void Update(float dt);
    void Shutdown();
};
