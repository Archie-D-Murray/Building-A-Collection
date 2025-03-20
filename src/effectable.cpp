#include "effectable.hpp"
#include "entity_animator.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "health.hpp"
#include "render_data.hpp"
#include <cmath>


Effect Effect::CreateSlow(float magnitude, float duration, Animation animation) {
    Effect effect = {
        .type = Slow,
        .magnitude = magnitude,
        .duration = duration,
    };
    if (animation != Idle) {
        effect.animation = animation;
        effect.hasAnimation = true;
    }
    return effect;
}

Effect Effect::CreateStun(float duration, Animation animation) {
    Effect effect = {
        .type = Stun,
        .duration = duration,
    };
    if (animation != Idle) {
        effect.animation = animation;
        effect.hasAnimation = true;
    }
    return effect;
}

Effect Effect::CreateDoT(float magnitude, float tickRate, float duration, Animation animation) {
    Effect effect = {
        .type = DamageOverTime,
        .magnitude = magnitude,
        .tickRate = tickRate,
        .duration = duration,

    };
    if (animation != Idle) {
        effect.animation = animation;
        effect.hasAnimation = true;
    }
    return effect;
}

void Effectable::Init(Game* game) {
    for (const ParticleSetting& particle : game->config->particleSettings) {
        animator.SetAnimations(particle.animation, particle.sprites);
    }
}

void Effectable::Update(Game* game, Vector2 position, float dt) {
    Animation animation;
    renderParticle = false;
    speedModifier = 1.0f;
    for (size_t i = 0; i < effects[Slow].size();) {
        Effect* effect = &effects[Slow][i];
        if (effect->hasAnimation) {
            animation = effect->animation;
            renderParticle = true;
        }
        effect->durationTimer += dt;
        if (effect->durationTimer >= effect->duration) {
            effects[Slow][i] = effects[Slow].back();
            effects[Slow].pop_back();
        } else {
            speedModifier = fmax(0.0f, speedModifier - effect->magnitude);
            i++;
        }
    }
    stunned = false;
    for (size_t i = 0; i < effects[Stun].size();) {
        Effect* effect = &effects[Stun][i];
        if (effect->hasAnimation) {
            animation = effect->animation;
            renderParticle = true;
        }
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
        if (effect->hasAnimation) {
            renderParticle = true;
            animation = effect->animation;
        }
        effect->durationTimer += dt;
        effect->tickTimer += dt;
        if (effect->tickTimer >= effect->tickRate) {
            health->Damage(game, effect->magnitude, position);
            effect->tickTimer -= effect->tickRate;
            effect->damageCount++;
        }
        if (effect->durationTimer >= effect->duration) {
            effects[DamageOverTime][i] = effects[DamageOverTime].back();
            effects[DamageOverTime].pop_back();
        } else {
            i++;
        }
    }
    if (renderParticle) {
        animator.Play(animation);
        animator.Update(dt);
    }
}

void Effectable::Render(Sprites::RenderData* data, Vector2 position) {
    if (renderParticle) {
        data->DrawSprite(animator.GetSprite(), position);
    }
}

void Effectable::AcceptEffect(Effect effect) {
    effects[effect.type].push_back(effect);
}
