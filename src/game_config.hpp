#pragma once

#include "render_data.hpp"
enum FamiliarType { Fire, Water, Earth, Lightning, FamiliarCount };
enum Tier { Common, Uncommon, Rare, Epic, TierCount };
enum EnemyType { Normal, Heavy, EnemyCount };
enum ProjectileType { Linear, AoE, Chain };

struct PlayerStats {
    float speed;
    float health;
    float collisionRadius;
};

struct FamiliarStats {
    float damage;
    float effectMagnitude;
    float effectDuration;
    float effectTickRate;
    float speed;
    float collisionRadius;
    float projectileRadius;
    float projectileSpeed;
    float attackTime;
    float attackRange;
    int arcCount;
    ProjectileType projectileType;
    Sprites::SpriteID projectileSprite;
    Sprites::SpriteID sprite;
};

struct EnemyStats {
    float health;
    float damage;
    float projectileSpeed;
    float collisionRadius;
    float projectileRadius;
    float speed;
    Sprites::SpriteID sprite;
};

struct GameConfig {
    PlayerStats playerStats;
    EnemyStats enemyStats[EnemyType::EnemyCount];
    FamiliarStats familiarStats[FamiliarType::FamiliarCount];
};

// TODO: Control familiar tier up
