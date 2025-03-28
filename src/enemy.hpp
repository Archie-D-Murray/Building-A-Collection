#pragma once

#include "effectable.hpp"
#include "entity_animator.hpp"
#include "health.hpp"
#include "raylib.h"
#include "render_data.hpp"
#include "game_config.hpp"

class Game;
class Player;
class Projectile;

class Enemy {
protected:
    Enemy(Vector2 position, float health, float difficulty);
    Vector2 velocity;
    Sprites::SpriteID sprite;
    float attackTimer = 0.0f;
    Health health;
    Effectable effectable;
    EntityAnimator animator;
    float difficulty;
public:
    Vector2 position;
    EnemyType type;
    float speed = 100.0f;
    float damage = 1;
    float collisionRadius = 10.0f;
    float range = 350.0f;
    float projectileSpeed = 150.0f;
    float attackTime = 1.5f;
    
    virtual void Init(Game* game) = 0;
    virtual void Update(float dt, Game* game) = 0;
    virtual void Render(Sprites::RenderData* data) = 0;
    virtual void Destroy(Game* game) = 0;
    virtual void Fire(Game* game) = 0;
    virtual bool CanFire(const Player& player);
    virtual bool DoCollision(Game* game, Projectile* projectile);
    Health& GetHealth() { return health; };
    Vector2& GetVelocity() { return velocity; }
};
