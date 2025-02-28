#include <cmath>
#include "health.hpp"

Health::Health(float maxHealth) : 
    currentHealth(maxHealth),
    maxHealth(maxHealth)
{}

void Health::Damage(float amount) {
    if (amount == 0.0f) {
        return;
    }
    if (amount >= currentHealth || currentHealth <= 0.0f) {
        isDead = true;
        return;
    }
    currentHealth -= fmax(amount, 0.0f);
}
