#pragma once
#include "effectable.hpp"
#include "game_config.hpp"
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
    float familiarRotation;
    Vector2 position;
    void Init(Game* game);
    void Update(float dt);
    void Render(Sprites::RenderData* data);
    void AddFamiliar(Game* game, FamiliarType type, Tier tier);
    void Destroy();
    Vector2 Velocity() const;
    Health& GetHealth() { return health; };
    bool Collides(Projectile* projectile);
    void Damage(Game* game, Projectile* projectile);
    bool IsStunned() { return effectable.stunned; };
};
