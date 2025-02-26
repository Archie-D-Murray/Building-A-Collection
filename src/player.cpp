#include "player.hpp"
#include "familiar.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "projectile.hpp"


Player::Player() : effectable(), health(10.0f) { }

void Player::Init(Game* game) {
    position = { 720, 405 };
    velocity = { 0 };
    speed = 100.0f;
    game->familiars.push_back(Familiar(position + Vector2 { 50.0f, 0.0f }, Fire, Common));
    health = Health(100.0f);
    health.BindDamageReceiver(dynamic_cast<DamageReciever*>(this));
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

void Player::OnDamage(Health* health) {
    TraceLog(LOG_INFO, "Player was damaged, health: %0.0f!", health->PercentHealth() * 100.0f);
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
