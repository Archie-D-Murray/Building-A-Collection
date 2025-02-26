#include "enemy.hpp"
#include "effectable.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "raylib.h"

Enemy::Enemy(Vector2 position, float health) : 
    position(position),
    health(health) { }

bool Enemy::CanFire(const Player& player) {
    return attackTimer <= 0.0f && Vector2DistanceSqr(player.position, position) <= range * range && !effectable.stunned;
}

bool Enemy::DoCollision(Projectile* projectile) {
    if (CheckCollisionCircles(projectile->position, projectile->collisionRadius, position, collisionRadius)) {
        health.Damage(projectile->damage);
        for (Effect& effect : projectile->effects) {
            effectable.AcceptEffect(effect);
        }
        return true;
    }
    return false;
}
