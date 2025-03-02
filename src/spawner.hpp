#pragma once
#include "raylib.h"
class Game;

typedef void (*Spawn)(Game* game, Vector2 position);
enum SpawnerType { Area, Range };

class Spawner {
    SpawnerType type;
    Vector2 centre;
    float spawnCooldown;
    Rectangle spawnArea;
    float minSpawnRange;
    float maxSpawnRange;
    float spawnTimer = 0.0f;
    float difficulty = 1.0f;
    int spawnMax = 100;
    Spawn spawnCommand;
    bool canSpawn = false;

public:
    Spawner();
    Spawner(Vector2 centre, Rectangle spawnArea, float spawnCooldown, Spawn spawnFunction);
    Spawner(Vector2 centre, float minSpawnRange, float maxSpawnRange, float spawnCooldown, Spawn spawnFunction);
    void Update(float dt, Game* game);
    void DecreaseSpawnCooldown(float amount, float min);
};
