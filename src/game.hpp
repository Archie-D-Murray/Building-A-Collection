#pragma once
#include "damage_number_manager.hpp"
#include "fader.hpp"
#include "familiar.hpp"
#include "familiar_egg.hpp"
#include "game_config.hpp"
#include "player.hpp"
#include "render_data.hpp"
#include "enemy.hpp"
#include "spawner.hpp"
#include <vector>

GameConfig CreateConfig();

enum State { None, InGame, Menu, Quit };

class Game {
    Sprites::RenderData* renderData;
    State nextState = None;
public:
    State state;
    Fader fader;
    Vector2 screenSize;
    Player player;
    std::vector<Familiar> familiars;
    std::vector<FamiliarEgg> familiarEggs;
    std::vector<Enemy*> enemies;
    std::vector<Projectile*> enemyProjectiles;
    std::vector<Projectile*> familiarProjectiles;
    Spawner enemySpawner;
    Spawner familiarSpawner;
    DamageNumberManager damageNumberManager;
    const GameConfig config;
    float zoom = 4.0f;
    float scale;

    Game(State state, Vector2 screenSize, Sprites::RenderData* data);
    void Init();
    State Update(float dt);
    void Shutdown();

private:
    void ProcessProjectiles(float dt);
    void GameUI(float dt);
    void GameBackground();
    static void SpawnRandomEnemy(Game* game, Vector2 position);
    static void SpawnRandomFamiliar(Game* game, Vector2 position);
};
