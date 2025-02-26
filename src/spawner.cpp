#include "spawner.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"

Spawner::Spawner(Vector2 centre, float spawnCooldown, Spawn spawnCommand) : centre(centre), spawnCooldown(spawnCooldown), spawnCommand(spawnCommand) {}

void Spawner::Update(float dt, Game* game) {
    if (spawnTimer >= 0.0f) {
        spawnTimer += dt;
    }
    if (game->enemies.size() > spawnMax) {
        return;
    }

    if (spawnTimer >= spawnCooldown) {
        spawnTimer -= spawnCooldown;
        spawnCommand(game, centre + Vector2Rotate(Vector2 { 0, 1 }, (float) GetRandomValue(0, 360)) * GetRandomValue(minSpawnRadius, maxSpawnRadius));
    }    
}
