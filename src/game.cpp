#include "game.hpp"
#include "player.hpp"
#include "raylib.h"
#include "raymath.h"
#include "normal_enemy.hpp"
#include "heavy_enemy.hpp"

Game::Game(Vector2 screenSize) :
    screenSize(screenSize),
    player(),
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
    player.Update(dt);
    player.Render(&renderData);
    if (familiars.empty()) {
        TraceLog(LOG_INFO, "No familiar!");
    }
    for (Familiar& familiar : familiars) {
        familiar.Update(dt, player);
        familiar.Render(&renderData);
    }
    for (Enemy* enemy : enemies) {
        enemy->Update(dt, player);
        if (enemy->CanFire(player)) {
            enemy->Fire(this);
        }
        enemy->Render(&renderData);
    }
}

void Game::Shutdown() {

}
