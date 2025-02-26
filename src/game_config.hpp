#pragma once

enum FamiliarType { Fire, Water, Earth, Lightning, FamiliarCount };
enum Tier { Common, Uncommon, Rare, Epic, TierCount };
enum EnemyType { Normal, Heavy, EnemyCount };

struct PlayerStats {
    float speed;
    float collisionRadius;
};

struct FamiliarStats {
    float damage;
    float effectMagnitude;
    float effectDuration;
    float effectTickRate;
    float speed;
    float collisionRadius;
    float attackTime;
    float attackRange;
    int arcCount;
};

struct EnemyStats {
    float health;
    float damage;
    float projectileSpeed;
    float projectileDuration;
    float collisionRadius;
    float speed;
};

struct GameConfig {
    PlayerStats playerStats;
    EnemyStats enemyStats[EnemyType::EnemyCount];
    FamiliarStats familiarStats[FamiliarType::FamiliarCount];
};

// TODO: Control familiar tier up

inline GameConfig CreateConfig() {
    return GameConfig {
        .playerStats = {
            .speed = 150.0f,
            .collisionRadius = 64.0f,
        },
        .enemyStats = {
            [Normal] = EnemyStats {
                .health = 25.0f,
                .damage = 5.0f,
                .projectileSpeed = 125.0f,
                .projectileDuration = 5.0f,
                .collisionRadius = 80.0f,
                .speed = 100.0f
            },
            [Heavy] = EnemyStats {
                .health = 50.0f,
                .damage = 15.0f,
                .projectileSpeed = 0.0f,
                .projectileDuration = 0.0f,
                .collisionRadius = 144.0f,
                .speed = 75.0f
            }
        },
        .familiarStats = {
            [Fire] = FamiliarStats {
                .damage = 30.0f,
                .effectMagnitude = 3.0f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.25f,
                .speed = 175.0f,
                .attackTime = 1.0f,
                .attackRange = 160.0f,
                .arcCount = 0,
            },
            [Water] = FamiliarStats {
                .damage = 10.0f,
                .effectMagnitude = 0.6f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.0f,
                .speed = 150.0f,
                .attackTime = 0.75f,
                .attackRange = 180.0f,
                .arcCount = 0,
            },
            [Earth] = FamiliarStats {
                .damage = 100.0f,
                .effectMagnitude = 0.0f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.0,
                .speed = 100.0f,
                .attackTime = 3.0f,
                .attackRange = 200.0f,
                .arcCount = 0,
            },
            [Lightning] = FamiliarStats {
                .damage = 25.0f,
                .effectMagnitude = 0.0f,
                .effectDuration = 0.5f,
                .effectTickRate = 0.0f,
                .speed = 125.0f,
                .attackTime = 2.0f,
                .attackRange = 100.0f,
                .arcCount = 3,
            },
        }
    };
};
