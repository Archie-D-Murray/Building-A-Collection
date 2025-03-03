#include "heavy_enemy.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "player.hpp"
#include "raylib.h"
#include "raymath.h"
#include "render_data.hpp"

HeavyEnemy::HeavyEnemy(Game* game, Vector2 position) : Enemy(position, game->config.enemyStats[Heavy].health, game->difficulty) {
    effectable.health = &health;
    Init(game);
}

void HeavyEnemy::Init(Game* game) {
    effectable.Init(game);
    speed = game->config.enemyStats[Heavy].speed;
    collisionRadius = game->config.enemyStats[Heavy].collisionRadius;
    animator.SetAnimations(Idle, game->config.enemyStats[Heavy].sprites);
}

void HeavyEnemy::Update(float dt, Game* game) {
    if (attackTimer >= 0.0f) {
        attackTimer -= dt;
    }
    if (dashTimer >= 0.0f) {
        dashTimer -= dt;
        velocity = dashDir * speed * dt;
        if (CheckCollisionCircles(game->player.position, game->player.collisionRadius, position, collisionRadius) && attackTimer <= 0.0f) {
            game->player.Damage(game, damage);
            attackTimer += attackTime;
        }
    } else {
        velocity = { 0 };
    }
    position += velocity;
}

void HeavyEnemy::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position);
    effectable.Render(data, position);
}

void HeavyEnemy::Fire(Game* game) {
    dashDir = Vector2Normalize(game->player.position - position);
    dashTimer = dashTime;
}

bool HeavyEnemy::CanFire(const Player& player) {
    return Vector2Distance(position, player.position) <= targetRange && dashTimer <= 0.0f;
}

void HeavyEnemy::Destroy(Game* game) {
    
}
