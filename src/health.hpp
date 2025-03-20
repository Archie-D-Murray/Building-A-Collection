#pragma once

#include "raylib.h"
class Health;

class Game;

const Color damageColour = RED;
const float damageTime = 0.25f;

class Health {
private:
    float currentHealth = 1.0f;
    float maxHealth = 1.0f;
    float damageTimer = 0.0f;
    bool isDead = false;
public:

    Health(float maxHealth);

    inline float CurrentHealth() { return currentHealth; };
    inline float MaxHealth() { return maxHealth; };
    inline float PercentHealth() { return maxHealth == 0.0f ? 1.0f : currentHealth / maxHealth; };
    void Update(float dt);
    void Damage(Game* game, float amount, Vector2 source);
    Color DamageTint();
    inline bool IsDead() { return isDead; };
};
