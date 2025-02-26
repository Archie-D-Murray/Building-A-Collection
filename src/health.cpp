#include <cmath>
#include "health.hpp"

Health::Health(float maxHealth) : 
    currentHealth(maxHealth),
    maxHealth(maxHealth),
    damageReceivers() {
}

void Health::Damage(float amount) {
    if (amount == 0.0f) {
        return;
    }
    if (amount >= currentHealth || currentHealth <= 0.0f) {
        isDead = true;
        return;
    }
    currentHealth -= fmax(amount, 0.0f);
    for (DamageReciever* receiver : damageReceivers) {
        receiver->OnDamage(this);
    }
}

void Health::BindDamageReceiver(DamageReciever* receiver) {
    damageReceivers.push_back(receiver);
}
