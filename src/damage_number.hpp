#pragma once

#include "raylib.h"
#include "render_data.hpp"
class DamageNumber {
    static constexpr Color DEFAULT = LIGHTGRAY;
public:
    float damage;
    Vector2 position;
    Color colour = DEFAULT;

    void Update(float dt);
    void Render(Sprites::RenderData* data);
    void Reset(float damage, Vector2 position);
};
