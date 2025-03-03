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

Game::Game(State state, Vector2 screenSize, Sprites::RenderData* data, GameConfig* config, SoundManager* soundManager) : 
    renderData(data), 
    state(state), 
    config(config), 
    screenSize(screenSize), 
    scale(1080.0f / screenSize.y), 
    zoom(4.0f * screenSize.y / 1080.0f),
    worldRadius(data->World().width * 0.5f),
    soundManager(soundManager),
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
    soundManager->PlayBGM(Passive);
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
    soundManager->DrawUI();
    /*DrawText(TextFormat("Player pos: [ %.0f, %.0f ], dash timer: %d", player.position.x, player.position.y, player.IsVulnerable() * RAD2DEG), 10, 35, 14, WHITE);*/
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
        if (player.GetHealth().IsDead() || IsKeyPressed(KEY_Q)) {
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
    // Sound
    soundManager->Update(dt);
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

            // TODO: Sometimes projectiles have weird speed values not sure why
            if (projectile->OffScreen(this) || projectile->speed <= EPSILON) {
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
    #if 0
    game->enemies.push_back(dynamic_cast<Enemy*>(new HeavyEnemy(game, position)));
    #else
    if (!game->enemies.empty() && game->spawnCount % 10 == 0) {
        game->enemies.push_back(dynamic_cast<Enemy*>(new HeavyEnemy(game, position)));
    } else {
        game->enemies.push_back(dynamic_cast<Enemy*>(new NormalEnemy(game, position)));
    }
    #endif
    game->spawnCount++;
    if (game->spawnCount % 25 == 0) {
        game->enemySpawner.DecreaseSpawnCooldown(0.1f, 0.25f);
        game->difficulty += 0.1f;
    }
}

void Game::SpawnRandomFamiliar(Game* game, Vector2 position) {
    game->familiarEggs.push_back(FamiliarEgg(position, (FamiliarType)GetRandomValue(0, 3)));
}
