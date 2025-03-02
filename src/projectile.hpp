#pragma once

#include "entity_animator.hpp"
#include "game_config.hpp"
#include "raylib.h"
#include "render_data.hpp"
#include <vector>

class Game;
class Effect;

struct VisualEffectData {
    std::vector<Sprites::SpriteID> frames;
    float duration;
    bool hasVFX = false;
};

class Projectile {
public:
    Projectile(Vector2 position, Vector2 direction, float speed, float collisionRadius, float damage, std::vector<Sprites::SpriteID> frames);
    Vector2 position;
    Vector2 direction;
    float angle = 0.0f;
    float speed;
    float collisionRadius;
    float damage;
    EntityAnimator animator;
    std::vector<Effect> effects;
    ProjectileType type = Linear;
    VisualEffectData data;
    int chainCount = 0;

    void Update(float dt);
    void Render(Sprites::RenderData* data);
    bool OffScreen(Game* game);
    void PushVFX(Game* game, const Vector2& position);
    void AddVFX(float duration, std::vector<Sprites::SpriteID> frames);
};
