#include "familiar.hpp"

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
