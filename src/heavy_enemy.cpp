#include "heavy_enemy.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "player.hpp"
#include "raylib.h"
#include "raymath.h"
#include "render_data.hpp"

HeavyEnemy::HeavyEnemy(Game* game, Vector2 position) : Enemy(position, game->config.enemyStats[Heavy].health) {
    effectable.health = &health;
    Init(game);
}

void HeavyEnemy::Init(Game* game) {
    speed = game->config.enemyStats[Heavy].speed;
    collisionRadius = game->config.enemyStats[Heavy].collisionRadius;
    animator.SetAnimations(Idle, game->config.enemyStats[Heavy].sprites);
}

void HeavyEnemy::Update(float dt, Player& player) {
    velocity = Vector2ClampValue(targetPos - position, 0.0f, speed * dt);
    position += velocity;
    if (attackTimer <= 0.0f && CheckCollisionCircles(player.position, player.collisionRadius, position, collisionRadius)) {
        player.GetHealth().Damage(damage);
        attackTimer += attackTime;
    } else if (attackTimer >= 0.0f) {
        attackTimer -= dt;
    }
    animator.Update(dt);
}

void HeavyEnemy::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position);
}

void HeavyEnemy::Fire(Game* game) {
    targetPos = game->player.position + Vector2Rotate( { 0, range }, (float) GetRandomValue(0, 360));
}

bool HeavyEnemy::CanFire(const Player& player) {
    return Vector2DistanceSqr(position, targetPos) <= 10.0f;
}

void HeavyEnemy::Destroy(Game* game) {
    
}
