#include "game.hpp"
#include "familiar.hpp"
#include "game_config.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "raylib.h"
#include "raymath.h"
#include "normal_enemy.hpp"
#include "heavy_enemy.hpp"
#include "render_data.hpp"


GameConfig CreateConfig() {
    return GameConfig {
        .playerStats = PlayerStats {
            .speed = 150.0f,
            .health = 100.f,
            .collisionRadius = 64.0f,
        },
        .enemyStats = {
            [Normal] = EnemyStats {
                .health = 25.0f,
                .damage = 5.0f,
                .projectileSpeed = 125.0f,
                .collisionRadius = 80.0f,
                .projectileRadius = 32.0f,
                .speed = 100.0f
            },
            [Heavy] = EnemyStats {
                .health = 50.0f,
                .damage = 15.0f,
                .projectileSpeed = 0.0f,
                .collisionRadius = 144.0f,
                .projectileRadius = 0.0f,
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
                .projectileRadius = 80.0f,
                .projectileSpeed = 125.0f,
                .attackTime = 1.0f,
                .attackRange = 160.0f,
                .arcCount = 0,
                .projectileSprite = Sprites::FireProjectile,
                .sprite = Sprites::FireFamiliar,
            },
            [Water] = FamiliarStats {
                .damage = 10.0f,
                .effectMagnitude = 0.6f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.0f,
                .speed = 150.0f,
                .projectileRadius = 80.0f,
                .projectileSpeed = 125.0f,
                .attackTime = 0.75f,
                .attackRange = 180.0f,
                .arcCount = 0,
                .projectileType = Linear,
                .projectileSprite = Sprites::WaterProjectile,
                .sprite = Sprites::WaterFamiliar,
            },
            [Earth] = FamiliarStats {
                .damage = 100.0f,
                .effectMagnitude = 0.0f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.0,
                .speed = 100.0f,
                .projectileRadius = 314.0f,
                .projectileSpeed = 100.0f,
                .attackTime = 3.0f,
                .attackRange = 200.0f,
                .arcCount = 0,
                .projectileType = AoE,
                .projectileSprite = Sprites::EarthProjectile,
                .sprite = Sprites::EarthFamiliar,
            },
            [Lightning] = FamiliarStats {
                .damage = 25.0f,
                .effectMagnitude = 0.0f,
                .effectDuration = 0.5f,
                .effectTickRate = 0.0f,
                .speed = 125.0f,
                .projectileRadius = 256.0f,
                .projectileSpeed = 2000.0f,
                .attackTime = 2.0f,
                .attackRange = 100.0f,
                .arcCount = 3,
                .projectileType = Chain,
                .projectileSprite = Sprites::LightningProjectile,
                .sprite = Sprites::LightningFamiliar,
            },
        },
    };
};

Game::Game(Vector2 screenSize) :
    config(CreateConfig()),
    screenSize(screenSize),
    player(this),
    familiarSpawner(screenSize * 0.5f, 10.0f, Game::SpawnRandomFamiliar),
    enemySpawner(screenSize * 0.5f, 5.0f, Game::SpawnRandomEnemy),
    renderData("./atlas.png")
    {
}

void Game::Init() {
    player.Init(this);
    pixelOffset = screenSize * 0.5f;
    zoom = 1080.0f / screenSize.y;
    pixelOffset.x *= -1;
    TraceLog(LOG_INFO, "Atlas texture size: %d, %d", renderData.GetAtlas().width, renderData.GetAtlas().height);
}

void Game::Update(float dt) {
    BeginMode2D(Camera2D { .zoom = zoom * 4.0f });
    if (!player.GetHealth().IsDead()) {
        player.Update(dt);
        player.Render(&renderData);
    } else {
        
    }
    if (familiars.empty()) {
        TraceLog(LOG_INFO, "No familiar!");
    }
    for (Familiar& familiar : familiars) {
        familiar.Update(dt, player);
        Enemy* target = familiar.GetTarget(this);
        if (target) {
            familiar.Attack(this, target);
        }
    }
    for (Enemy* enemy : enemies) {
        enemy->Update(dt, player);
        if (enemy->CanFire(player)) {
            enemy->Fire(this);
        }
    }
    ProcessProjectiles(dt);
    familiarSpawner.Update(dt, this);
    enemySpawner.Update(dt, this);
    for (size_t i = 0; i < enemies.size();) {
        if (enemies[i]->GetHealth().IsDead()) {
            enemies[i] = enemies.back();
            enemies.pop_back();
        } else {
            enemies[i]->Render(&renderData);
            i++;
        }
    } 
    for (Familiar& familiar : familiars) {
        familiar.Render(&renderData);
    }
    EndMode2D();
}

void Game::Shutdown() {

}

void Game::ProcessProjectiles(float dt) {
    for (size_t i = 0; i < enemyProjectiles.size();) {
        Projectile* projectile = enemyProjectiles[i];
        if (CheckCollisionCircles(projectile->position, projectile->collisionRadius, player.position, player.collisionRadius)) {
            if (!familiars.empty()) {
                if (familiars.back().tier > Tier::Common) {
                    familiars.pop_back();
                } else {
                    familiars.back().DropTier();
                }
            }
            player.DoCollision(projectile);
            enemyProjectiles[i] = enemyProjectiles.back();
            enemyProjectiles.pop_back();
        } else if (projectile->OffScreen(this)) {
            enemyProjectiles[i] = enemyProjectiles.back();
            enemyProjectiles.pop_back();
        } else {
            projectile->Render(&renderData);
            i++;
        }
    }

    for (size_t i = 0; i < familiarProjectiles.size();) {
        Projectile* projectile = familiarProjectiles[i];
        bool deleted = false;
        int hitCount = 0;
        int hitMax = 0;
        switch (projectile->type) {
        case Linear:
            hitMax = 1;
            break;
        case AoE:
            hitMax = enemies.size();
            break;
        case Chain:
            hitMax = projectile->chainCount;
            break;
        }
        for (Enemy* enemy : enemies) {
            if (enemy->DoCollision(projectile)) {
                deleted = true;
                hitCount++;
            }
            if (hitCount >= hitMax) {
                break;
            }
        }
        if (deleted) {
            familiarProjectiles[i] = familiarProjectiles.back();
            familiarProjectiles.pop_back();
        } else {
            projectile->Render(&renderData);
            i++;
        }
    }
}

void Game::SpawnRandomEnemy(Game* game, Vector2 position) {
    if (GetRandomValue(0, 1)) {
        game->enemies.push_back(dynamic_cast<Enemy*>(new NormalEnemy(game, position)));
    } else {
        game->enemies.push_back(dynamic_cast<Enemy*>(new HeavyEnemy(game, position)));
    }
}


void Game::SpawnRandomFamiliar(Game* game, Vector2 position) {
    game->familiars.push_back(Familiar(position, (FamiliarType) GetRandomValue(0, 3), (Tier) GetRandomValue(0, 4), game->config));
}
