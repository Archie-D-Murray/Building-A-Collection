#pragma once
#include "effectable.hpp"
#include "health.hpp"
#include "raylib.h"
#include "render_data.hpp"

class Game;
class Projectile;

class Player {
    Vector2 velocity;
    float speed;
    Sprites::SpriteID sprite;
    Health health;
    Effectable effectable;

public:
    Player(Game* game);
    float collisionRadius;
    Vector2 position;
    void Init(Game* game);
    void Update(float dt);
    void Render(Sprites::RenderData* data);
    void Destroy();
    Vector2 Velocity() const;
    Health& GetHealth() { return health; };
    bool DoCollision(Projectile* projectile);
    bool IsStunned() { return effectable.stunned; };
};
