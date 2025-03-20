#include <cmath>
#include "health.hpp"
#include "game.hpp"

Health::Health(float maxHealth) : 
    currentHealth(maxHealth),
    maxHealth(maxHealth)
{}

void Health::Damage(Game* game, float damage, Vector2 source) {
    damageTimer = damageTime;
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

void Health::Update(float dt) {
    damageTimer = fmaxf(0.0f, damageTimer - dt);
}

Color Health::DamageTint() {
    if (damageTimer == 0.0f) {
        return WHITE;
    } else {
        return ColorLerp(WHITE, damageColour, damageTimer / damageTime);
    }
}
