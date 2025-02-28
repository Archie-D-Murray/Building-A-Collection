#pragma once
#include "damage_number.hpp"
#include "render_data.hpp"
class DamageNumberManager {
    const float total = 10;
    DamageNumber damageNumbers[10];
    size_t count = 0;
    size_t oldest = 0;

    void GetOldest();
public:
    void Update(float dt);
    void Render(Sprites::RenderData* data);
    void PushDamageNumber(float damage, Vector2 position);
};
