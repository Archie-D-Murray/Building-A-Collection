#include "familiar.hpp"
#include "game_config.hpp"
#include "raylib.h"
#include "player.hpp"
#include "raymath.h"

Familiar::Familiar(Vector2 position, FamiliarType type, Tier tier, const GameConfig& config) : position(position) {
    Init(type, tier, config);
}

void Familiar::Init(FamiliarType type, Tier tier, const GameConfig& config) {
    this->type = type;
    this->tier = tier;
    damage = config.familiarStats[type].damage;
    effectMagnitude = config.familiarStats[type].effectMagnitude;
    effectDuration = config.familiarStats[type].effectDuration;
    effectTickRate = config.familiarStats[type].effectTickRate;
    speed = config.familiarStats[type].speed;
    collisionRadius = config.familiarStats[type].collisionRadius;
    attackTime = config.familiarStats[type].attackTime;
    attackRange = config.familiarStats[type].attackRange;
    arcCount = config.familiarStats[type].arcCount;
}

void Familiar::LevelUp(int increase = 1) {
    if (tier == Epic) { return; }
    level += increase;
    while (level >= nextTierLevel && tier != Epic) {
        nextTierLevel += LEVEL_PER_TIER;
        float multiplier = 1.0f;
        switch (tier) {
        case Common:
            return;
        case Uncommon:
            multiplier = 1.25f;
        case Rare:
            multiplier = 1.5f;
        case Epic:
            multiplier = 2.0f;
          break;
        default:
            break;
        }
        damage = round(damage * multiplier);
    }
}

void Familiar::Render(Sprites::RenderData* data) {
    DrawText(TextFormat("Familiar at: %0.0f, %0.0f", position.x, position.y), 10, 20, 18, WHITE);
    Color colour;
    switch (type) {
    case Fire:
        colour = RED;
        break;
    case Water:
        colour = BLUE;
        break;
    case Earth:
        colour = BROWN;
        break;
    case Lightning:
        colour = PURPLE;
        break;
    }

    DrawCircleV(position, 25.0f, colour);
}

void Familiar::Update(float dt, const Player& player) {
    Vector2 followOffset = Vector2ClampValue(player.Velocity() * -1.0f, 0.0f, followRadius * dt);
    position = Vector2MoveTowards(position, player.position, 50.0f * dt);
}
