#include "spawner.hpp"
#include "game.hpp"
#include "raylib.h"

Spawner::Spawner() : canSpawn(false) { }

Spawner::Spawner(Vector2 centre, Rectangle spawnArea, float spawnCooldown, Spawn spawnCommand) : 
    canSpawn(true), 
    centre(centre), 
    spawnCooldown(spawnCooldown), 
    spawnArea(spawnArea), 
    spawnCommand(spawnCommand),
    type(Area)
{}

Spawner::Spawner(Vector2 centre, float minSpawnRange, float maxSpawnRange, float spawnCooldown, Spawn spawnCommand) : 
    canSpawn(true), 
    centre(centre), 
    spawnCooldown(spawnCooldown),
    minSpawnRange(minSpawnRange),
    maxSpawnRange(maxSpawnRange),
    spawnCommand(spawnCommand),
    type(Range)
{}

void Spawner::Update(float dt, Game* game) {
    if (!canSpawn) { return; }
    if (spawnTimer >= 0.0f) {
        spawnTimer += dt;
    }
    if (game->enemies.size() > spawnMax) {
        return;
    }

    if (spawnTimer <= spawnCooldown) {
        return;
    }

    Vector2 spawnPoint;
    if (type == Area) {
        float value = GetRandomValue(0, 2.0f * (spawnArea.width + spawnArea.height));
        if (value > (spawnArea.width + spawnArea.height)) {
            if (value < spawnArea.width) {
                spawnPoint = { value, 0 };
            } else {
                spawnPoint = { spawnArea.width, value - spawnArea.width };
            }
        } else {
            value -= (spawnArea.width + spawnArea.height);
            if (value < spawnArea.width) {
                spawnPoint = { spawnArea.width - value, spawnArea.height };
            } else {
                spawnPoint = { 0, spawnArea.height - (value - spawnArea.width) };
            }
        }
    } else {
        spawnPoint = centre + Vector2Rotate({ 0, (float) GetRandomValue(minSpawnRange, maxSpawnRange) }, (float) GetRandomValue(0, 360));
    }
    spawnCommand(game, spawnPoint);
    spawnTimer -= spawnCooldown;
}
