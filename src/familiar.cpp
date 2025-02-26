#include "familiar.hpp"
#include "raylib.h"
#include "player.hpp"
#include "raymath.h"

Familiar::Familiar(Vector2 position, FamiliarType type, Tier tier) {
    Init(type, tier);
}

void Familiar::Init(FamiliarType type, Tier tier) {
    switch (type) {
    case Fire:
        damage = 10.0f;
        attackTime = 1.0f;
    case Water:
        damage = 2.0f;
        attackTime = 0.25f;
    case Earth:
        damage = 15.0f;
        attackTime = 2.0f;
    case Lightning:
        damage = 7.5f;
        attackTime = 1.5f;
      break;
    }
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
