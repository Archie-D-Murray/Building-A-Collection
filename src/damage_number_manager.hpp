#pragma once
#include "damage_number.hpp"
#include "render_data.hpp"
#include <cstddef>
#include <vector>
class DamageNumberManager {
    const float total = 10;
    std::vector<DamageNumber> damageNumbers;
    size_t GetOldest();
public:
    void Update(float dt);
    void Render(Sprites::RenderData* data);
    void PushDamageNumber(float damage, Vector2 position);
};
