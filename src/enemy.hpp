#pragma once

#include "raylib.h"
#include "render_data.hpp"

class Game;
class Player;

enum EnemyType { Normal, Heavy };

class Enemy {
protected:
    Vector2 velocity;
    Sprites::SpriteID sprite;
    float attackTimer = 0.0f;
public:
    Vector2 position;
    EnemyType type;
    float speed = 100.0f;
    float damage = 1;
    float range = 100.0f;
    float projectileSpeed = 100.0f;
    float attackTime = 1.0f;
    
    virtual void Init(Game* game);
    virtual void Update(float dt, const Player& player);
    virtual void Render(Sprites::RenderData* data);
    virtual void Destroy(Game* game);
    virtual void Fire(Game* game);
    virtual bool CanFire(const Player& player);
};
