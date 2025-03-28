#include "normal_enemy.hpp"
#include "game.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "raylib.h"
#include "render_data.hpp"

NormalEnemy::NormalEnemy(Game* game, Vector2 position) : Enemy(position, game->config->enemyStats[Normal].health, game->difficulty) {
    effectable.health = &health;
    Init(game);
}

void NormalEnemy::Init(Game* game) {
    effectable.Init(game);
    projectileSpeed = game->config->enemyStats[Normal].projectileSpeed;
    speed = game->config->enemyStats[Normal].speed * difficulty;
    range *= difficulty;
    collisionRadius = game->config->enemyStats[Normal].collisionRadius;
    projectileRadius = game->config->enemyStats[Normal].projectileRadius;
    damage = game->config->enemyStats[Normal].damage;
    animator.SetAnimations(Idle, game->config->enemyStats[Normal].sprites);
}

void NormalEnemy::Update(float dt, Game* game) {
    health.Update(dt);
    effectable.Update(game, position, dt);
    if (effectable.stunned) {
        return;
    }
    velocity = Vector2ClampValue(game->player.position - position, 0.0f, speed * effectable.speedModifier * dt);
    position += velocity;
    if (attackTimer >= 0.0f) {
        attackTimer -= dt;
    }
    animator.Update(dt);
}

void NormalEnemy::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position, 0.0f, health.DamageTint());
    effectable.Render(data, position);
}

void NormalEnemy::Fire(Game* game) {
    game->enemyProjectiles.push_back(
        new Projectile(
            position, 
            Vector2Normalize(game->player.position - position),
            projectileSpeed, 
            projectileRadius, 
            damage, 
            game->config->enemyStats[Normal].projectileSprites
        )
    );
    attackTimer += attackTime;
}

void NormalEnemy::Destroy(Game* game) {
    
}
