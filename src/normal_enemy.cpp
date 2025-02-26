#include "normal_enemy.hpp"
#include "game.hpp"
#include "player.hpp"
#include "projectile.hpp"

NormalEnemy::NormalEnemy(Game* game, Vector2 position) : Enemy(position, 40.0f) {
    effectable.health = &health;
    Init(game);
}

void NormalEnemy::Init(Game* game) { }

void NormalEnemy::Update(float dt, Player& player) {
    velocity = Vector2ClampValue(player.position - position, 0.0f, speed * dt);
    position += velocity;
    if (attackTimer > 0.0f) {
        attackTimer -= dt;
    }
}

void NormalEnemy::Render(Sprites::RenderData* data) {
    DrawCircleV(position, 40.0f, DARKBLUE);
}

void NormalEnemy::Fire(Game* game) {
    game->enemyProjectiles.push_back(new Projectile(position, Vector2Normalize(game->player.position - position), projectileSpeed, 20.0f, damage, Sprites::Player));
    attackTimer += attackTime;
}

void NormalEnemy::Destroy(Game* game) {
    
}
