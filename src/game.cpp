#include "game.hpp"
#include "familiar.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "raylib.h"
#include "raymath.h"
#include "normal_enemy.hpp"
#include "heavy_enemy.hpp"

Game::Game(Vector2 screenSize) :
    screenSize(screenSize),
    player(),
    familiarSpawner(screenSize * 0.5f, 10.0f, Game::SpawnRandomFamiliar),
    enemySpawner(screenSize * 0.5f, 5.0f, Game::SpawnRandomEnemy),
    renderData("./atlas.png")
    {
}

void Game::Init() {
    player.Init(this);
    pixelOffset = screenSize * 0.5f;
    pixelOffset.x *= -1;
    TraceLog(LOG_INFO, "Atlas texture size: %d, %d", renderData.GetAtlas().width, renderData.GetAtlas().height);
}

void Game::Update(float dt) {
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
    }
    for (Enemy* enemy : enemies) {
        enemy->Update(dt, player);
        if (enemy->CanFire(player)) {
            enemy->Fire(this);
        }
    }
    ProcessProjectiles(dt);
    for (Familiar& familiar : familiars) {
        familiar.Render(&renderData);
    }
}

void Game::Shutdown() {

}

void Game::ProcessProjectiles(float dt) {
    familiarSpawner.Update(dt, this);
    enemySpawner.Update(dt, this);

    for (size_t i = 0; i < enemyProjectiles.size();) {
        Projectile* projectile = enemyProjectiles[i];
        if (CheckCollisionCircles(projectile->position, projectile->collisionRadius, player.position, player.collisionRadius)) {
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
        for (size_t enemyIdx = 0; enemyIdx < enemies.size();) {
            Enemy* enemy = enemies[enemyIdx];
            if (enemy->DoCollision(projectile)) {
                enemies[enemyIdx] = enemies.back();
                enemies.pop_back();
                deleted = true;
                break;
            } else {
                enemy->Render(&renderData);
                enemyIdx++;
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
    game->familiars.push_back(Familiar(position, (FamiliarType) GetRandomValue(0, 3), (Tier) GetRandomValue(0, 4)));
}
