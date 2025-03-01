#include "normal_enemy.hpp"
#include "game.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "render_data.hpp"

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
    animator.SetAnimations(Idle, game->config.enemyStats[Normal].sprites);
}

void NormalEnemy::Update(float dt, Player& player) {
    velocity = Vector2ClampValue(player.position - position, 0.0f, speed * dt);
    position += velocity;
    if (attackTimer > 0.0f) {
        attackTimer -= dt;
    }
    animator.Update(dt);
}

void NormalEnemy::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position);
}

void NormalEnemy::Fire(Game* game) {
    game->enemyProjectiles.push_back(
        new Projectile(
            position, 
            Vector2Normalize(game->player.position - position),
            projectileSpeed, 
            projectileRadius, 
            damage, 
            game->config.enemyStats[Normal].projectileSprites
        )
    );
    attackTimer += attackTime;
}

void NormalEnemy::Destroy(Game* game) {
    
}
