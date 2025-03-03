#pragma once

#include <vector>
enum EffectType { Slow, Stun, DamageOverTime, Count };

class Health;

class Effect {
public:
    EffectType type;
    float magnitude;
    float tickRate = 0.25f;
    float tickTimer = 0.0f;
    float durationTimer = 0.0f;
    float duration = 1.0f;

    static Effect CreateSlow(float magnitude, float duration);
    static Effect CreateStun(float duration);
    static Effect CreateDoT(float magnitude, float tickRate, float duration);
};

class Effectable {
public:
    float speedModifier = 1.0f;
    bool stunned = false;
    std::vector<Effect> effects[EffectType::Count];
    Health* health;

    void AcceptEffect(Effect effect);
    void Update(float dt);
};
