#include "enemy.hpp"
#include "player.hpp"

bool Enemy::CanFire(const Player& player) {
    return attackTimer <= 0.0f && Vector2DistanceSqr(player.position, position) <= range * range;
}
