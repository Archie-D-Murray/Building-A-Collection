#pragma once
#include "effectable.hpp"
#include "entity_animator.hpp"
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
    EntityAnimator animator;

public:
    Player(Game* game);
    float collisionRadius;
    float familiarRotation;
    Vector2 position;
    void Init(Game* game);
    void Update(Game* game, float dt);
    void Render(Sprites::RenderData* data);
    void AddFamiliar(Game* game, FamiliarType type);
    void Destroy();
    Vector2 Velocity() const;
    Health& GetHealth() { return health; };
    bool Collides(Projectile* projectile);
    void Damage(Game* game, Projectile* projectile);
    bool IsStunned() { return effectable.stunned; };
};
