#pragma once

#include "entity_animator.hpp"
#include "raylib.h"
#include "render_data.hpp"
#include "game_config.hpp"

class Player;
class Game;
class Enemy;

class Familiar {
private:

    const static int LEVEL_PER_TIER = 2;

    float followRadius = 50;
    int nextTierLevel = 2;
    float attackTimer = 0.0f;
    EntityAnimator animator;

public:
    FamiliarType type;
    Vector2 position;

    int level = 0;
    Tier tier = Common;

    float damage;
    float effectMagnitude;
    float effectDuration;
    float effectTickRate;
    float projectileRadius;
    float projectileSpeed;
    float attackTime;
    float attackRange;
    int arcCount;

    Familiar(Vector2 position, FamiliarType type, Tier tier, const GameConfig& config);
    void Init(FamiliarType type, Tier tier, const GameConfig& config);
    void Update(float dt, const Player& player, float offset);
    void Render(Sprites::RenderData* data);
    void AdvanceTier();
    void DropTier();
    void Attack(Game* game, Enemy* target);
    void Destroy();
    Enemy* GetTarget(Game* game);
};
