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
            .collisionRadius = 6.0f,
        },
        .enemyStats = {
            [Normal] = EnemyStats {
                .health = 25.0f,
                .damage = 5.0f,
                .projectileSpeed = 125.0f,
                .collisionRadius = 8.0f,
                .projectileRadius = 4.0f,
                .speed = 100.0f,
                .sprite = Sprites::NormalTypeEnemy,
            },
            [Heavy] = EnemyStats {
                .health = 50.0f,
                .damage = 15.0f,
                .projectileSpeed = 0.0f,
                .collisionRadius = 16.0f,
                .projectileRadius = 0.0f,
                .speed = 75.0f,
                .sprite = Sprites::HeavyTypeEnemy,
            }
        },
        .familiarStats = {
            [Fire] = FamiliarStats {
                .damage = 30.0f,
                .effectMagnitude = 3.0f,
                .effectDuration = 2.0f,
                .effectTickRate = 0.25f,
                .speed = 175.0f,
                .projectileRadius = 8.0f,
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
                .projectileRadius = 8.0f,
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
                .projectileRadius = 32.0f,
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
                .projectileRadius = 24.0f,
                .projectileSpeed = 500.0f,
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
    zoom(4.0f * 1080.0f / screenSize.y),
    player(this),
    renderData("./atlas.png")
    {
    Rectangle spawnArea = { 0, 0, screenSize.x / zoom, screenSize.y / zoom };
    familiarSpawner = Spawner(screenSize * 0.5f / zoom, spawnArea.height * 0.1f, spawnArea.height * 0.45f, 10.0f, Game::SpawnRandomFamiliar);
    enemySpawner = Spawner(screenSize * 0.5f / zoom, spawnArea, 2.5f, Game::SpawnRandomEnemy);
}

void Game::Init() {
    player.Init(this);
    pixelOffset = screenSize * 0.5f;
    pixelOffset.x *= -1;
}

void Game::Update(float dt) {
    BeginMode2D(Camera2D { .zoom = zoom });
    if (!player.GetHealth().IsDead()) {
        player.Update(dt);
        player.Render(&renderData);
    } else {
        
    }
    for (size_t i = 0; i < familiarEggs.size();) {
        if (CheckCollisionCircles(player.position, player.collisionRadius, familiarEggs[i].position, familiarEggs[i].collisionRadius)) {
            player.AddFamiliar(this, familiarEggs[i].type, familiarEggs[i].tier);
            familiarEggs[i] = familiarEggs.back();
            familiarEggs.pop_back();
        } else {
            familiarEggs[i].Render(&renderData);
            i++;
        }
    }
    for (size_t i = 0; i < familiars.size(); i++) {
        Familiar& familiar = familiars[i];
        familiar.Update(dt, player, (float) i / (float) familiars.size());
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
        projectile->Update(dt);
        if (player.Collides(projectile)) {
            player.Damage(this, projectile);
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
        projectile->Update(dt);
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
    if (game->enemies.empty()) {
        game->enemies.push_back(dynamic_cast<Enemy*>(new NormalEnemy(game, position)));
        return;
    }
    if (!game->enemies.empty() && game->enemies.size() % 10 == 0 && game->enemies.back()->type != Heavy) {
        game->enemies.push_back(dynamic_cast<Enemy*>(new HeavyEnemy(game, position)));
    } else {
        game->enemies.push_back(dynamic_cast<Enemy*>(new NormalEnemy(game, position)));
    }
}


void Game::SpawnRandomFamiliar(Game* game, Vector2 position) {
    TraceLog(LOG_INFO, "Spawned egg at (%.0f, %.0f)", position.x, position.y);
    game->familiarEggs.push_back(FamiliarEgg(position, (FamiliarType) GetRandomValue(0, 3), (Tier) GetRandomValue(0, 4)));
}
