#include "effectable.hpp"
#include "health.hpp"
#include "raylib.h"
#include <cmath>

void Effectable::Update(float dt) {
    speedModifier = 1.0f;
    for (size_t i = 0; i < effects[Slow].size();) {
        Effect* effect = &effects[Slow][i];
        effect->durationTimer += dt;
        if (effect->durationTimer >= effect->duration) {
            effects[Slow][i] = effects[Slow].back();
            effects[Slow].pop_back();
        } else {
            speedModifier = fmax(0.0f, speedModifier - effect->magnitude);
            i++;
        }
    }
    if (speedModifier != 1.0f) {
        DrawText("Player slowed", 10, 40, 18, WHITE);
    }
    stunned = false;
    for (size_t i = 0; i < effects[Stun].size();) {
        Effect* effect = &effects[Stun][i];
        effect->durationTimer += dt;
        if (effect->durationTimer >= effect->duration) {
            effects[Stun][i] = effects[Stun].back();
            effects[Stun].pop_back();
        } else {
            stunned = true;
            i++;
        }
    }

    for (size_t i = 0; i < effects[DamageOverTime].size();) {
        Effect* effect = &effects[DamageOverTime][i];
        effect->durationTimer += dt;
        effect->tickTimer += dt;
        if (effect->tickTimer >= effect->tickRate) {
            health->Damage(effect->magnitude);
            effect->tickTimer -= effect->tickRate;
        }
        if (effect->durationTimer >= effect->duration) {
            effects[DamageOverTime][i] = effects[DamageOverTime].back();
            effects[DamageOverTime].pop_back();
        } else {
            stunned = true;
            i++;
        }
    }
}

void Effectable::AcceptEffect(Effect effect) {
    effects[effect.type].push_back(effect);
}
