#include "familiar.hpp"
#include "effectable.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "projectile.hpp"
#include "raylib.h"
#include "player.hpp"
#include "raymath.h"
#include "render_data.hpp"

const float DAMAGE_MODIFIERS[TierCount] = {
    ENUM_INDEX(Common)    1.0f,
    ENUM_INDEX(Uncommon)  1.25f,
    ENUM_INDEX(Rare)      1.5f,
    ENUM_INDEX(Epic)      2.0f
};

const float FIRE_RATE_MODIFIERS[TierCount] = {
    ENUM_INDEX(Common)    1.0f,
    ENUM_INDEX(Uncommon)  0.8f,
    ENUM_INDEX(Rare)      1.6f,
    ENUM_INDEX(Epic)      0.4f,
};

Familiar::Familiar(Vector2 position, FamiliarType type, Tier tier, Game* game) : position(position) {
    Init(type, tier, game);
}

void Familiar::Init(FamiliarType type, Tier tier, Game* game) {
    this->type = type;
    this->tier = tier;
    damage = game->config->familiarStats[type].damage;
    effectMagnitude = game->config->familiarStats[type].effectMagnitude;
    effectDuration = game->config->familiarStats[type].effectDuration;
    effectTickRate = game->config->familiarStats[type].effectTickRate;
    projectileRadius = game->config->familiarStats[type].projectileRadius;
    projectileSpeed = game->config->familiarStats[type].projectileSpeed;
    attackTime = game->config->familiarStats[type].attackTime;
    attackRange = game->config->familiarStats[type].attackRange;
    arcCount = game->config->familiarStats[type].arcCount;
    animator.SetAnimations(Idle, game->config->familiarStats[type].sprites);
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
            TraceLog(LOG_INFO, "Could not find tier up case for tier: %d", tier);
            break;
    }
}

void Familiar::DropTier() {
    if (tier == Common) { return; }
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
            TraceLog(LOG_INFO, "Could not find drop case for tier: %d", tier);
            break;
    }
}

void Familiar::Render(Sprites::RenderData* data) {
    data->DrawSprite(Sprites::FamiliarOutline, position, 0.0f, tierColour[tier]);
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
    attackTimer += attackTime * FIRE_RATE_MODIFIERS[tier];
    /*if (projectileSpeed < 0 || isnanf(projectileSpeed) || isinf(projectileSpeed)) {*/
    /*    DebugTrap();*/
    /*}*/
    game->familiarProjectiles.push_back(
        new Projectile(
            position, 
            Vector2Normalize(target->position + target->GetVelocity() - position),
            projectileSpeed, 
            projectileRadius, 
            damage * DAMAGE_MODIFIERS[tier], 
            game->config->familiarStats[type].projectileSprites
        )
    );
    Projectile* projectile = game->familiarProjectiles.back();
    switch (type) {
    case Fire:
        projectile->effects.push_back(
            Effect::CreateDoT(game->config->familiarStats[type].effectMagnitude, game->config->familiarStats[type].effectTickRate, game->config->familiarStats[type].effectDuration, OnFire)
        );
    case Water:
        projectile->type = Linear;
        projectile->effects.push_back(
            Effect::CreateSlow(game->config->familiarStats[type].effectMagnitude, game->config->familiarStats[type].effectDuration, Iced)
        );
        break;
    case Earth:
        projectile->type = AoE;
        projectile->effects.push_back(
            Effect::CreateStun(game->config->familiarStats[type].effectDuration, Grounded)
        );
        projectile->AddVFX(game->config->familiarStats[type].visualDuration, game->config->familiarStats[type].visualEffectSprites);
        break;
    case Lightning:
        projectile->chainCount = this->arcCount;
        projectile->type = Chain;
        projectile->effects.push_back(
            Effect::CreateStun(game->config->familiarStats[type].effectDuration, Zapped)
        );
        projectile->AddVFX(game->config->familiarStats[type].visualDuration, game->config->familiarStats[type].visualEffectSprites);
        break;
    default:
        break;
    }
}
