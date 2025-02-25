#pragma once

#include "raylib.h"

class Game;
class Player;

class Enemy {
    Vector2 velocity;
public:
    Vector2 position;
    
    void Init(Game* game);
    void Update(float dt, const Player& player);
    void Destroy(Game* game);
};
