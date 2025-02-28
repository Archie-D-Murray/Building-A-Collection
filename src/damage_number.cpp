#include "damage_number.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

void DamageNumber::Update(float dt) {
    const float speed = 10.0f;
    colour.a = fmaxf(0, colour.a - floorf(255 * dt));
    position += Vector2 { 0, -speed } * dt;
}

void DamageNumber::Render(Sprites::RenderData* data) {
    DrawTextEx(data->GetFont(), TextFormat("%.0f", damage), position, 8, 1.0f, colour);
}

void DamageNumber::Reset(float damage, Vector2 position) {
    this->damage = damage;
    this->position = position;
    colour = DEFAULT;
}
