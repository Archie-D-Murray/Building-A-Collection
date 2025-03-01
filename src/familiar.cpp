#include "familiar.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "projectile.hpp"
#include "raylib.h"
#include "player.hpp"
#include "raymath.h"
#include "debug.h"

const float DAMAGE_MODIFIERS[TierCount] = {
    [Common] = 1.0f,
    [Uncommon] = 1.25f,
    [Rare] = 1.5f,
    [Epic] = 2.0f
};

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
    projectileRadius = config.familiarStats[type].projectileRadius;
    projectileSpeed = config.familiarStats[type].projectileSpeed;
    attackTime = config.familiarStats[type].attackTime;
    attackRange = config.familiarStats[type].attackRange;
    arcCount = config.familiarStats[type].arcCount;
    animator.SetAnimations(Idle, config.familiarStats[type].sprites);
}

void Familiar::AdvanceTier() {
    if (tier == Epic) { return; }
    switch (tier) {
        case Common:
            tier = Uncommon;
            break;
        case Uncommon:
            tier = Rare;
            break;
        case Rare:
            tier = Epic;
            break;
        default:
            break;
    }
    damageModifier = DAMAGE_MODIFIERS[tier];
}

void Familiar::DropTier() {
    switch (tier) {
        case Uncommon:
            tier = Common;
            break;
        case Rare:
            tier = Uncommon;
            break;
        case Epic:
            tier = Rare;
            break;
        default:
            break;
    }
    damageModifier = DAMAGE_MODIFIERS[tier];
}

void Familiar::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position, 0.0f);
}

void Familiar::Update(float dt, const Player& player, float offset) {
    position = player.position + Vector2Rotate({ 0, followRadius}, player.familiarRotation + PI * 2.0f * offset);
    if (attackTimer >= 0.0f) {
        attackTimer -= dt;
    }
    animator.Update(dt);
}

Enemy* Familiar::GetTarget(Game* game) {
    if (attackTimer >= 0.0f) {
        return nullptr;
    }
    for (Enemy* enemy : game->enemies) {
        if (!enemy->GetHealth().IsDead() && CheckCollisionCircles(enemy->position, enemy->collisionRadius, position, attackRange)) {
            return enemy;
        }
    }
    return nullptr;
}

void Familiar::Attack(Game* game, Enemy* target) {
    attackTimer += attackTime;
    game->familiarProjectiles.push_back(
        new Projectile(
            position, 
            Vector2Normalize(target->position + target->GetVelocity() - position),
            projectileSpeed, 
            projectileRadius, 
            damage * damageModifier, 
            game->config.familiarStats[type].projectileSprites
        )
    );
    if (damage == 0.0f) {
        DebugTrap();
    }
    Projectile* projectile = game->familiarProjectiles.back();
    switch (type) {
    case Fire:
        break;
    case Water:
        projectile->type = Linear;
        break;
    case Earth:
        projectile->type = AoE;
    case Lightning:
        projectile->chainCount = this->arcCount;
        projectile->type = Chain;
        break;
    default:
        break;
    }
}
