#include "game.hpp"
#include "familiar.hpp"
#include "game_config.hpp"
#include "gui.hpp"
#include "heavy_enemy.hpp"
#include "normal_enemy.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "raylib.h"
#include "raymath.h"
#include "render_data.hpp"

GameConfig CreateConfig() {
    return GameConfig {
        .playerStats = PlayerStats {
            .speed = 150.0f,
            .health = 100.f,
            .collisionRadius = 6.0f,
            .idle = { Sprites::Player0 },
            .move = { Sprites::Player0, Sprites::Player1, Sprites::Player2 },
        },
        .enemyStats = {
            ENUM_INDEX(Normal) EnemyStats {
               .health = 25.0f,
               .damage = 5.0f,
               .projectileSpeed = 125.0f,
               .collisionRadius = 8.0f,
               .projectileRadius = 4.0f,
               .speed = 100.0f,
               .sprites = { Sprites::NormalTypeEnemy0, Sprites::NormalTypeEnemy1, Sprites::NormalTypeEnemy2 },
               .projectileSprites = { Sprites::EnemyProjectile0, Sprites::EnemyProjectile1 },
            },
            ENUM_INDEX(Heavy) EnemyStats {
               .health = 50.0f,
               .damage = 15.0f,
               .projectileSpeed = 0.0f,
               .collisionRadius = 16.0f,
               .projectileRadius = 0.0f,
               .speed = 75.0f,
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
                .projectileSpeed = 125.0f,
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
                .projectileSpeed = 125.0f,
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
                .projectileSpeed = 100.0f,
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
    };
};

Game::Game(State state, Vector2 screenSize, Sprites::RenderData* data) : 
    renderData(data), 
    state(state), 
    config(CreateConfig()), 
    screenSize(screenSize), 
    scale(1080.0f / screenSize.y), 
    zoom(4.0f * 1080.0f / screenSize.y),
    worldRadius(data->World().width * 0.5f),
    player(this) 
{
    if (state == InGame) {
        player = Player(this);
        Rectangle spawnArea = {0, 0, screenSize.x / zoom, screenSize.y / zoom};
        familiarSpawner = Spawner(screenSize * 0.5f, worldRadius * 0.2f, worldRadius * 0.9f, 10.0f, Game::SpawnRandomFamiliar);
        enemySpawner = Spawner(screenSize * 0.5f, spawnArea, 2.5f, Game::SpawnRandomEnemy);
    }
    fader.StartFade(true);
}

void Game::Init() {
    if (state == InGame) {
        player.Init(this);
    }
}

void Game::GameUI(float dt) {
    DrawFPS(10, 10);
    const Vector2 healthBarScale = {0.5f, 0.95f};
    const Color red = {128, 0, 0, 255};
    const float scale = screenSize.x * 0.25f / renderData->GetSource(Sprites::HealthBarBackground).width;
    Rectangle bar = {0, 0, 92 * scale * player.GetHealth().PercentHealth(), 2 * scale};
    bar.x = screenSize.x * healthBarScale.x - bar.width * 0.5f;
    bar.y = screenSize.y * healthBarScale.y - bar.height * 0.5f;
    renderData->DrawSpriteSize(Sprites::HealthBarBackground, screenSize * healthBarScale, scale);
    const float healthBarXOffset = 12 + 3 * Sprites::SPRITE_SIZE;
    DrawRectangleRec(bar, red);
    renderData->DrawSpriteSize(Sprites::HealthBarOverlay, screenSize * healthBarScale, scale);
    DrawText(TextFormat("Player pos: [ %.0f, %.0f ], distance from centre: %.0f, dot: %.2f", player.position.x, player.position.y, Vector2Distance(player.position, screenSize * 0.5f), Vector2DotProduct(Vector2Normalize(screenSize * 0.5f - player.position), Vector2Normalize(player.Velocity()))), 10, 35, 14, WHITE);
}

void Game::GameBackground() {
    DrawTextureV(renderData->World(), screenSize * 0.5f - Vector2 { worldRadius, worldRadius }, WHITE);
};

Vector2 ClosestPointOnRectangle(Vector2 point, Rectangle rect, float padding) {
    Vector2 min = {rect.x + padding, rect.y + padding};
    Vector2 max = {rect.x + rect.width - padding, rect.y + rect.height - padding};

    return Vector2Clamp(point, min, max);
}

State Game::Update(float dt) {
    if (state == InGame) {
        Camera2D camera = { .offset = screenSize * 0.5f, .target = player.position, .zoom = zoom };
        Rectangle screenRect = { camera.target.x - screenSize.x / (2.0f * zoom), camera.target.y - screenSize.y / (2.0f * zoom), screenSize.x / zoom, screenSize.y / zoom };
        BeginMode2D(camera);
        GameBackground();
        player.Update(this, dt);
        for (size_t i = 0; i < familiarEggs.size();) {
            if (CheckCollisionCircles(player.position, player.collisionRadius, familiarEggs[i].position, familiarEggs[i].collisionRadius)) {
                player.AddFamiliar(this, familiarEggs[i].type);
                familiarEggs[i] = familiarEggs.back();
                familiarEggs.pop_back();
            } else {
                if (CheckCollisionCircleRec(familiarEggs[i].position, familiarEggs[i].collisionRadius, screenRect)) {
                    familiarEggs[i].Render(renderData);
                } else {
                    renderData->DrawSprite(Sprites::FamiliarEggIndicator, ClosestPointOnRectangle(familiarEggs[i].position, screenRect, familiarEggs[i].collisionRadius * 0.5f));
                }
                i++;
            }
        }
        for (size_t i = 0; i < familiars.size(); i++) {
            Familiar& familiar = familiars[i];
            familiar.Update(dt, player, (float)i / (float)familiars.size());
            Enemy* target = familiar.GetTarget(this);
            if (target) {
                familiar.Attack(this, target);
            }
        }
        for (Enemy* enemy : enemies) {
            enemy->Update(dt, this);
            if (enemy->CanFire(player)) {
                enemy->Fire(this);
            }
        }
        UpdateVisualEffects(dt);
        ProcessProjectiles(dt);
        familiarSpawner.Update(dt, this);
        enemySpawner.Update(dt, this);
        for (size_t i = 0; i < enemies.size();) {
            if (enemies[i]->GetHealth().IsDead()) {
                enemies[i] = enemies.back();
                enemies.pop_back();
            } else {
                enemies[i]->Render(renderData);
                i++;
            }
        }
        for (Familiar& familiar : familiars) {
            familiar.Render(renderData);
        }
        player.Render(renderData);
        damageNumberManager.Update(dt);
        damageNumberManager.Render(renderData);
        EndMode2D();
        DrawText(TextFormat("Screen rect: %s", Sprites::RenderData::RectToString(screenRect).c_str()), 10, 100, 10, WHITE);
        GameUI(dt);
        if (player.GetHealth().IsDead()) {
            fader.StartFade(false);
            nextState = Menu;
        }
    } else {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        if (Button(Vector2{screenSize.x * 0.5f, screenSize.y * 0.5f}, Vector2{screenSize.x * 0.25f, screenSize.x * 0.025f}, "Play", renderData)) {
            TraceLog(LOG_INFO, "Started transition to game");
            nextState = InGame;
            fader.StartFade(false);
        }
        if (Button(Vector2{screenSize.x * 0.5f, screenSize.y * 0.5f + 1.5f * screenSize.x * 0.025f}, Vector2{screenSize.x * 0.25f, screenSize.x * 0.025f}, "Quit", renderData)) {
            TraceLog(LOG_INFO, "Started transition to game");
            nextState = Quit;
            fader.StartFade(false);
        }
    }
    fader.Update(this, dt);
    if (fader.FinishedStateFade()) {
        return nextState;
    }
    return None;
}

void Game::UpdateVisualEffects(float dt) {
    for (size_t i = 0; i < visualEffects.size();) {
        visualEffects[i].Update(dt);
        if (visualEffects[i].Finished()) {
            visualEffects[i] = visualEffects.back();
            visualEffects.pop_back();
        } else {
            visualEffects[i].Render(renderData);
            i++;
        }
    }
}

void Game::Shutdown() {}

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
            projectile->Render(renderData);
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
            if (enemy->DoCollision(this, projectile)) {
                deleted = true;
                hitCount++;
            }
            if (hitCount >= hitMax) {
                break;
            }
            if (projectile->OffScreen(this)) {
                deleted = true;
            }
        }
        if (deleted) {
            familiarProjectiles[i] = familiarProjectiles.back();
            familiarProjectiles.pop_back();
        } else {
            projectile->Render(renderData);
            i++;
        }
    }
}

void Game::SpawnRandomEnemy(Game* game, Vector2 position) {
    if (!game->enemies.empty() && game->spawnCount % 10 == 0) {
        game->enemies.push_back(dynamic_cast<Enemy*>(new HeavyEnemy(game, position)));
    } else {
        game->enemies.push_back(dynamic_cast<Enemy*>(new NormalEnemy(game, position)));
    }
    game->spawnCount++;
    if (game->spawnCount % 25 == 0) {
        game->enemySpawner.DecreaseSpawnCooldown(0.1f, 0.25f);
        game->difficulty += 0.1f;
    }
}

void Game::SpawnRandomFamiliar(Game* game, Vector2 position) {
    TraceLog(LOG_INFO, "Spawned egg at (%.0f, %.0f)", position.x, position.y);
    game->familiarEggs.push_back(FamiliarEgg(position, (FamiliarType)GetRandomValue(0, 3)));
}
