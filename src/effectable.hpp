#pragma once

#include "entity_animator.hpp"
#include "render_data.hpp"
#include <vector>
enum EffectType { Slow, Stun, DamageOverTime, Count };

class Health;
class Game;

class Effect {
public:
    EffectType type;
    float magnitude;
    float tickRate = 0.25f;
    float tickTimer = 0.0f;
    float durationTimer = 0.0f;
    float duration = 1.0f;
    bool hasAnimation;
    Animation animation;

    static Effect CreateSlow(float magnitude, float duration, Animation animation = Idle);
    static Effect CreateStun(float duration, Animation animation = Idle);
    static Effect CreateDoT(float magnitude, float tickRate, float duration, Animation animation = Idle);
};

class Effectable {
public:
    float speedModifier = 1.0f;
    bool stunned = false;
    std::vector<Effect> effects[EffectType::Count];
    Health* health;
    EntityAnimator animator;
    bool renderParticle = false;

    void Init(Game* game);
    void AcceptEffect(Effect effect);
    void Update(Game* game, Vector2 position, float dt);
    void Render(Sprites::RenderData* data, Vector2 position);
};
