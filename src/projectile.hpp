#pragma once

#include "game_config.hpp"
#include "raylib.h"
#include "render_data.hpp"
#include <vector>

class Game;
class Effect;

class Projectile {
public:
    Projectile(Vector2 position, Vector2 direction, float speed, float collisionRadius, float damage, Sprites::SpriteID);
    Vector2 position;
    Vector2 direction;
    float angle = 0.0f;
    float speed;
    float collisionRadius;
    float damage;
    Sprites::SpriteID sprite;
    std::vector<Effect> effects;
    ProjectileType type = Linear;
    int chainCount = 0;

    void Update(float dt);
    void Render(Sprites::RenderData* data);
    bool OffScreen(Game* game);
};
