#pragma once
#include "raylib.h"
#include "render_data.hpp"

class Game;

class Player {
    Vector2 velocity;
    float speed;
    Sprites::SpriteID sprite;

public:
    Vector2 position;
    void Init(Game* game);
    void Update(float dt);
    void Render(Sprites::RenderData* data);
    void Destroy();
    Vector2 Velocity() const;
};
