#pragma once
#include "game_config.hpp"
#include "player.hpp"
#include "render_data.hpp"
#include "familiar.hpp"
#include "enemy.hpp"
#include "spawner.hpp"
#include <vector>

class Game {
    Sprites::RenderData renderData;
public:
    Vector2 screenSize;
    Vector2 pixelOffset;
    Player player;
    std::vector<Familiar> familiars;
    std::vector<Enemy*> enemies;
    std::vector<Projectile*> enemyProjectiles;
    std::vector<Projectile*> familiarProjectiles;
    Spawner enemySpawner;
    Spawner familiarSpawner;
    const GameConfig config = CreateConfig();

    Game(Vector2 screenSize);
    void Init();
    void Update(float dt);
    void Shutdown();

private:
    void ProcessProjectiles(float dt);

    static void SpawnRandomEnemy(Game* game, Vector2 position);
    static void SpawnRandomFamiliar(Game* game, Vector2 position);
};
