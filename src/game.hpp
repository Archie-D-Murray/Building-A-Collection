#pragma once
#include "player.hpp"
#include "render_data.hpp"
#include "familiar.hpp"
#include <vector>

class Game {
    Sprites::RenderData renderData;
public:
    Vector2 screenSize;
    Vector2 pixelOffset;
    Player player;
    std::vector<Familiar> familiars;
    std::vector<Enemy> enemies;
    Game(Vector2 screenSize);
    void Init();
    void Update(float dt);
    void Shutdown();
};
