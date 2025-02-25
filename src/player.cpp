#include "player.hpp"
#include "familiar.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game.hpp"

void Player::Init(Game* game) {
    position = { 720, 405 };
    velocity = { 0 };
    speed = 100.0f;
    game->familiars.push_back(Familiar());
    game->familiars.back().Init(Fire, Common);
    game->familiars.back().position = position + Vector2 { 50.0f, 0.0f };
    TraceLog(LOG_INFO, "Added base familiar");
}

void Player::Update(float dt) {
    Vector2 input = {
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)),
    };

    velocity = Vector2Normalize(input) * (speed * dt);
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
