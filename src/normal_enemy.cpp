#include "normal_enemy.hpp"
#include "game.hpp"
#include "player.hpp"
#include "projectile.hpp"

NormalEnemy::NormalEnemy(Game* game, Vector2 position) : Enemy(position, game->config.enemyStats[Normal].health) {
    effectable.health = &health;
    Init(game);
}

void NormalEnemy::Init(Game* game) {
    projectileSpeed = game->config.enemyStats[Normal].projectileSpeed;
    speed = game->config.enemyStats[Normal].speed;
    collisionRadius = game->config.enemyStats[Normal].collisionRadius;
    projectileRadius = game->config.enemyStats[Normal].projectileRadius;
    damage = game->config.enemyStats[Normal].damage;
    sprite = game->config.enemyStats[Normal].sprite;
}

void NormalEnemy::Update(float dt, Player& player) {
    velocity = Vector2ClampValue(player.position - position, 0.0f, speed * dt);
    position += velocity;
    if (attackTimer > 0.0f) {
        attackTimer -= dt;
    }
}

void NormalEnemy::Render(Sprites::RenderData* data) {
    data->DrawSprite(sprite, position);
}

void NormalEnemy::Fire(Game* game) {
    game->enemyProjectiles.push_back(new Projectile(position, Vector2Normalize(game->player.position - position), projectileSpeed, projectileRadius, damage, Sprites::EnemyProjectile));
    attackTimer += attackTime;
}

void NormalEnemy::Destroy(Game* game) {
    
}
