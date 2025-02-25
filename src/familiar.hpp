#pragma once

#include "entity_animator.hpp"
#include "render_data.hpp"

enum FamiliarType { Fire, Water, Earth, Lightning };
enum Tier { Common, Uncommon, Rare, Epic };

class Player;

class Familiar {
private:

    const static int LEVEL_PER_TIER = 2;

    Sprites::SpriteID sprite;
    Vector2 velocity;

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


    void Init(FamiliarType type, Tier tier);
    void Update(float dt, const Player& player);
    void LevelUp(int levelIncrease);
    void Destroy();
};
