#include "game.hpp"
#include "player.hpp"
#include "raylib.h"
#include "raymath.h"

Game::Game(Vector2 screenSize) :
    screenSize(screenSize),
    player(),
    renderData("./atlas.png"),
    worldCamera() {
}

void Game::Init() {
    player.Init();
    worldCamera = Camera2D {
        .offset = { 0, 0 },
        .rotation = 0.0f,
        .zoom = 1,
    };
    pixelOffset = screenSize * 0.5f;
    pixelOffset.x *= -1;
    TraceLog(LOG_INFO, "Atlas texture size: %d, %d", renderData.GetAtlas().width, renderData.GetAtlas().height);
}

void Game::Update(float dt) {
    player.Update(dt);
    player.Render(&renderData);
}

void Game::Shutdown() {

}
