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
    float dashTimer = 0.0f;
    Vector2 dashDirection;

public:
    Player(Game* game);
    float collisionRadius;
    float familiarRotation;
    float dashCooldown;
    float dashSpeed;
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
    void Damage(Game* game, float damage);
    bool IsStunned() { return effectable.stunned; };
    void FamiliarDamage(Game* game);
};
