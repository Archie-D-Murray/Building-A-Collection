#include "normal_enemy.hpp"
#include "game.hpp"
#include "player.hpp"

void NormalEnemy::Init(Game* game) {
    
}

void NormalEnemy::Update(float dt, const Player& player) {
    velocity = Vector2ClampValue(player.position - position, 0.0f, speed * dt);
    position += velocity;
    if (attackTimer > 0.0f) {
        attackTimer -= dt;
    }
}

void NormalEnemy::Render(Sprites::RenderData* data) {
    DrawCircleV(position, 40.0f, DARKBLUE);
}

void NormalEnemy::Fire(Game* game) {

}

void NormalEnemy::Destroy(Game* game) {
    
}
