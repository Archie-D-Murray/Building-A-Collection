#pragma once

#include "entity_animator.hpp"
#include "render_data.hpp"
#include <vector>
enum FamiliarType { Fire, Water, Earth, Lightning, FamiliarCount };
enum Tier { Common, Uncommon, Rare, Epic, TierCount };
enum EnemyType { Normal, Heavy, EnemyCount };
enum ProjectileType { Linear, AoE, Chain };
enum SFXType { SFXNone, PlayerHit, PlayerDeath, EnemyHit, EnemyDeath, SFXCount };
enum BGMType { BGMNone, Passive, Combat, BGMCount };


struct PlayerStats {
    float speed;
    float health;
    float collisionRadius;
    std::vector<Sprites::SpriteID> idle;
    std::vector<Sprites::SpriteID> move;
    std::vector<Sprites::SpriteID> dashParticles;
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
    float visualDuration;
    int arcCount;
    ProjectileType projectileType;
    std::vector<Sprites::SpriteID> projectileSprites;
    std::vector<Sprites::SpriteID> sprites;
    std::vector<Sprites::SpriteID> visualEffectSprites;
};

struct EnemyStats {
    float health;
    float damage;
    float projectileSpeed;
    float collisionRadius;
    float projectileRadius;
    float speed;
    std::vector<Sprites::SpriteID> sprites;
    std::vector<Sprites::SpriteID> projectileSprites;
};

struct SFXFile {
    SFXType type;
    const char* file;
};

struct BGMFile {
    BGMType type;
    const char* file;
};

struct SoundSettings {
    std::vector<SFXFile> sfxFiles;
    std::vector<BGMFile> bgmFiles;
};

struct ParticleSetting {
    Animation animation;
    std::vector<Sprites::SpriteID> sprites;
};

struct GameConfig {
    PlayerStats playerStats;
    EnemyStats enemyStats[EnemyType::EnemyCount];
    FamiliarStats familiarStats[FamiliarType::FamiliarCount];
    SoundSettings soundSettings;
    std::vector<ParticleSetting> particleSettings;
};


constexpr static Color tierColour[TierCount] = {
    ENUM_INDEX(Common)     WHITE,
    ENUM_INDEX(Uncommon)   GREEN,
    ENUM_INDEX(Rare)       BLUE,
    ENUM_INDEX(Epic)       PURPLE
};
