#pragma once

#include "raylib.h"
class Health;

class Game;

class Health {
private:
    float currentHealth = 1.0f;
    float maxHealth = 1.0f;
    bool isDead = false;
public:

    Health(float maxHealth);

    inline float CurrentHealth() { return currentHealth; };
    inline float MaxHealth() { return maxHealth; };
    inline float PercentHealth() { return maxHealth == 0.0f ? 1.0f : currentHealth / maxHealth; };

    void Damage(Game* game, float amount, Vector2 source);
    inline bool IsDead() { return isDead; };
};
