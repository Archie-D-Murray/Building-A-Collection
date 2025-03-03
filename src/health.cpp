#include <cmath>
#include "health.hpp"
#include "game.hpp"

Health::Health(float maxHealth) : 
    currentHealth(maxHealth),
    maxHealth(maxHealth)
{}

void Health::Damage(Game* game, float damage, Vector2 source) {
    game->damageNumberManager.PushDamageNumber(damage, source);
    if (damage == 0.0f) {
        return;
    }
    if (damage >= currentHealth || currentHealth <= 0.0f) {
        isDead = true;
        return;
    }
    currentHealth -= fmax(damage, 0.0f);
}
