#include "enemy.hpp"
#include "effectable.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "raylib.h"
#include "game.hpp"
#include "raymath.h"

Enemy::Enemy(Vector2 position, float health, float difficulty) : 
    position(position),
    health(health * difficulty),
    difficulty(difficulty)
{}

bool Enemy::CanFire(const Player& player) {
    return attackTimer <= 0.0f && Vector2DistanceSqr(player.position, position) <= range * range && !effectable.stunned;
}

bool Enemy::DoCollision(Game* game, Projectile* projectile) {
    if (CheckCollisionCircles(projectile->position, projectile->collisionRadius, position, collisionRadius)) {
        health.Damage(projectile->damage);
        projectile->PushVFX(game, position);
        game->damageNumberManager.PushDamageNumber(projectile->damage, projectile->position);
        game->soundManager.ContinueCombatMusic();
        for (Effect& effect : projectile->effects) {
            effectable.AcceptEffect(effect);
        }
        return true;
    }
    return false;
}
