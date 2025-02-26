#pragma once

#include "entity_animator.hpp"
#include "render_data.hpp"
#include "game_config.hpp"

class Player;

class Familiar {
private:

    const static int LEVEL_PER_TIER = 2;

    Sprites::SpriteID sprite;
    Vector2 velocity;
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
    float attackTime;

    Familiar(Vector2 position, FamiliarType type, Tier tier);
    void Init(FamiliarType type, Tier tier);
    void Update(float dt, const Player& player);
    void Render(Sprites::RenderData* data);
    void LevelUp(int levelIncrease);
    void Destroy();
};
