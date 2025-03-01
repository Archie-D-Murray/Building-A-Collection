#include "damage_number_manager.hpp"
#include <cstdint>
#include "damage_number.hpp"

void DamageNumberManager::Update(float dt) {
    uint8_t alpha = 255;
    for (size_t i = 0; i < damageNumbers.size();) {
        damageNumbers[i].Update(dt);
        if (damageNumbers[i].colour.a == 0) {
            damageNumbers[i] = damageNumbers.back();
            damageNumbers.pop_back();
        } else {
            i++;
        }
    }
}

void DamageNumberManager::Render(Sprites::RenderData* data) {
    for (DamageNumber& number : damageNumbers) {
        number.Render(data);
    }
}

void DamageNumberManager::PushDamageNumber(float damage, Vector2 position) {
    damageNumbers.push_back(DamageNumber { .damage = damage, .position = position });
}

size_t DamageNumberManager::GetOldest() {
    uint8_t alpha = 255;
    size_t oldest = damageNumbers.size();
    for (size_t i = 0; i < damageNumbers.size(); i++) {
        if (damageNumbers[i].colour.a < alpha) {
            oldest = i;
            alpha = damageNumbers[i].colour.a;
        }
    }
    return oldest;
}
