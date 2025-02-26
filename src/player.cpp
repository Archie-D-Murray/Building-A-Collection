#include "player.hpp"
#include "familiar.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "projectile.hpp"


Player::Player(Game* game) : effectable(), health(game->config.playerStats.health) { 
}

void Player::Init(Game* game) {
    sprite = Sprites::Player;
    position = { 720, 405 };
    velocity = { 0 };
    speed = game->config.playerStats.speed;
    collisionRadius = game->config.playerStats.collisionRadius;
    game->familiars.push_back(Familiar(position + Vector2 { 50.0f, 0.0f }, Fire, Common, game->config));
    effectable = Effectable();
    effectable.health = &health;
    TraceLog(LOG_INFO, "Added base familiar");
}

void Player::Update(float dt) {
    if (effectable.stunned) {
        return;
    }
    effectable.Update(dt);
    Vector2 input = {
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)),
    };

    velocity = Vector2Normalize(input) * (speed * dt * effectable.speedModifier);
    DrawText(TextFormat("Player speed: %0.0f", dt * speed * effectable.speedModifier), 10, 60, 18, WHITE);
    position += velocity;
}

void Player::Render(Sprites::RenderData* data) {
    DrawTexturePro(data->GetAtlas(), data->GetSource(sprite), data->GetDest(sprite, position), data->GetOffset(sprite), 0, WHITE);
}

void Player::Destroy() {

}

Vector2 Player::Velocity() const { 
    return velocity; 
}

bool Player::DoCollision(Projectile* projectile) {
    if (CheckCollisionCircles(position, collisionRadius, projectile->position, projectile->collisionRadius)) {
        health.Damage(projectile->damage);
        for (Effect& effect : projectile->effects) {
            
        }
        return true;
    }
    return false;
}
