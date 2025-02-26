#pragma once
#include "raylib.h"
class Game;

typedef void (*Spawn)(Game* game, Vector2 position);

class Spawner {
    Vector2 centre;
    float spawnCooldown;
    int minSpawnRadius;
    int maxSpawnRadius;
    float spawnTimer = 0.0f;
    float difficulty = 1.0f;
    int spawnMax = 100;
    Spawn spawnCommand;

public:
    Spawner(Vector2 centre, float spawnCooldown, Spawn spawnFunction);
    void Update(float dt, Game* game);
};
