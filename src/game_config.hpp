#pragma once

#include "entity_animator.hpp"
#include "render_data.hpp"
#include <vector>
enum FamiliarType { Fire, Water, Earth, Lightning, FamiliarCount };
enum Tier { Common, Uncommon, Rare, Epic, TierCount };
enum EnemyType { Normal, Heavy, EnemyCount };
enum ProjectileType { Linear, AoE, Chain };
enum SFXType { SFXNone, PlayerHit, PlayerDeath, NormalHit, NormalDeath, HeavyHit, HeavyDeath, UIHover, UIClick, SFXCount };
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


inline GameConfig CreateConfig() {
    return GameConfig {
        .playerStats = PlayerStats {
            .speed = 150.0f,
            .health = 100.f,
            .collisionRadius = 6.0f,
            .idle = { Sprites::Player0 },
            .move = { Sprites::Player0, Sprites::Player1, Sprites::Player2 },
            .dashParticles = { Sprites::DashParticle0, Sprites::DashParticle1, Sprites::DashParticle2 },
        },
        .enemyStats = {
            ENUM_INDEX(Normal) EnemyStats {
               .health = 50.0f,
               .damage = 5.0f,
               .projectileSpeed = 125.0f,
               .collisionRadius = 8.0f,
               .projectileRadius = 4.0f,
               .speed = 100.0f,
               .sprites = { Sprites::NormalTypeEnemy0, Sprites::NormalTypeEnemy1, Sprites::NormalTypeEnemy2 },
               .projectileSprites = { Sprites::EnemyProjectile0, Sprites::EnemyProjectile1 },
            },
            ENUM_INDEX(Heavy) EnemyStats {
               .health = 200.0f,
               .damage = 15.0f,
               .projectileSpeed = 0.0f,
               .collisionRadius = 16.0f,
               .projectileRadius = 0.0f,
               .speed = 225.0f,
               .sprites = { Sprites::HeavyTypeEnemy0, Sprites::HeavyTypeEnemy1, Sprites::HeavyTypeEnemy2 },
               .projectileSprites = { Sprites::Count, Sprites::Count },
            }
        },
        .familiarStats = {
            ENUM_INDEX(Fire) FamiliarStats {
                .damage = 30.0f,
                .effectMagnitude = 3.0f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.25f,
                .speed = 175.0f,
                .projectileRadius = 8.0f,
                .projectileSpeed = 425.0f,
                .attackTime = 1.0f,
                .attackRange = 160.0f,
                .arcCount = 0,
                .projectileSprites = { Sprites::FireProjectile0, Sprites::FireProjectile1 },
                .sprites = { Sprites::FireFamiliar0, Sprites::FireFamiliar1, Sprites::FireFamiliar2 },
            },
            ENUM_INDEX(Water) FamiliarStats {
                .damage = 10.0f,
                .effectMagnitude = 0.6f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.0f,
                .speed = 150.0f,
                .projectileRadius = 8.0f,
                .projectileSpeed = 325.0f,
                .attackTime = 0.75f,
                .attackRange = 180.0f,
                .arcCount = 0,
                .projectileType = Linear,
                .projectileSprites = { Sprites::WaterProjectile0, Sprites::WaterProjectile1 },
                .sprites = { Sprites::WaterFamiliar0, Sprites::WaterFamiliar1, Sprites::WaterFamiliar2 },
            },
            ENUM_INDEX(Earth) FamiliarStats {
                .damage = 100.0f,
                .effectMagnitude = 0.0f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.0,
                .speed = 100.0f,
                .projectileRadius = 32.0f,
                .projectileSpeed = 200.0f,
                .attackTime = 3.0f,
                .attackRange = 200.0f,
                .visualDuration = 1.5f,
                .arcCount = 0,
                .projectileType = AoE,
                .projectileSprites = { Sprites::EarthProjectile0, Sprites::EarthProjectile1 },
                .sprites = { Sprites::EarthFamiliar0, Sprites::EarthFamiliar1, Sprites::EarthFamiliar2 },
                .visualEffectSprites = { Sprites::EarthVisual },
            },
            ENUM_INDEX(Lightning) FamiliarStats {
                .damage = 25.0f,
                .effectMagnitude = 0.0f,
                .effectDuration = 0.5f,
                .effectTickRate = 0.0f,
                .speed = 125.0f,
                .projectileRadius = 24.0f,
                .projectileSpeed = 500.0f,
                .attackTime = 2.0f,
                .attackRange = 100.0f,
                .visualDuration = 0.5f,
                .arcCount = 3,
                .projectileType = Chain,
                .projectileSprites = { Sprites::LightningProjectile0, Sprites::LightningProjectile1 },
                .sprites = { Sprites::LightningFamiliar0, Sprites::LightningFamiliar1, Sprites::LightningFamiliar2 },
                .visualEffectSprites = { Sprites::LightningVisual0, Sprites::LightningVisual1, Sprites::LightningVisual2 },
            },
        },
        .soundSettings = {
            .sfxFiles = {
                SFXFile { .type = PlayerHit,   .file = "./assets/audio/player_hit.mp3" },
                SFXFile { .type = PlayerDeath, .file = "./assets/audio/flesh_death.mp3" },
                SFXFile { .type = NormalHit,   .file = "./assets/audio/normal_hit.mp3" },
                SFXFile { .type = NormalDeath, .file = "./assets/audio/flesh_death.mp3" },
                SFXFile { .type = HeavyHit,    .file = "./assets/audio/heavy_hit.mp3" },
                SFXFile { .type = HeavyDeath,  .file = "./assets/audio/heavy_death.mp3" },
                SFXFile { .type = UIClick,     .file = "./assets/audio/ui_click.mp3" },
                SFXFile { .type = UIHover,     .file = "./assets/audio/ui_hover.mp3" },
            },
            .bgmFiles = {
                BGMFile { .type = Passive, .file = "./assets/audio/passive.mp3" },
                BGMFile { .type = Combat, .file = "./assets/audio/combat.mp3" },
            }
        },
        .particleSettings = {
            ParticleSetting { .animation = OnFire, .  sprites = { Sprites::FireParticle0, Sprites::FireParticle1, Sprites::FireParticle2 } },
            ParticleSetting { .animation = Iced,     .sprites = { Sprites::IceParticle } },
            ParticleSetting { .animation = Grounded, .sprites = { Sprites::EarthParticle0, Sprites::EarthParticle1 } },
            ParticleSetting { .animation = Zapped,   .sprites = { Sprites::LightningParticle0, Sprites::LightningParticle1 } },
        },
    };
};
