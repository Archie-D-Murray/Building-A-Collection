#include "damage_number_manager.hpp"
#include <cstdint>

void DamageNumberManager::Update(float dt) {
    uint8_t alpha = 255;
    for (size_t i = 0; i < count;) {
        damageNumbers[i].Update(dt);
        if (damageNumbers[i].colour.a == 0) {
            damageNumbers[i] = damageNumbers[--count];
        } else {
            i++;
        }
    }
}

void DamageNumberManager::Render(Sprites::RenderData* data) {
    for (size_t i = 0; i < count; i++) {
        damageNumbers[i].Render(data);
    }
}

void DamageNumberManager::PushDamageNumber(float damage, Vector2 position) {
    if (count >= total) {
        GetOldest();
        damageNumbers[oldest].Reset(damage, position);
    } else {
        damageNumbers[count++].Reset(damage, position);
        GetOldest();
    }
}

void DamageNumberManager::GetOldest() {
    uint8_t alpha = 255;
    for (size_t i = 0; i < count; i++) {
        if (damageNumbers[i].colour.a < alpha) {
            oldest = i;
            alpha = damageNumbers[i].colour.a;
        }
    }
}
