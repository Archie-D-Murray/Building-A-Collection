#include "player.hpp"
#include "raylib.h"
#include "raymath.h"

void Player::Init() {
    position = { 720, 405 };
    TraceLog(LOG_INFO, "Player is at [ %0.0f, %0.0f]", position.x, position.y);
    velocity = { 0 };
    speed = 5.0f;
}

void Player::Update(float dt) {
    Vector2 input = {
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)),
    };

    velocity = Vector2Normalize(input) * (speed * dt);
}

void Player::Render(Sprites::RenderData* data) {
    DrawTexturePro(data->GetAtlas(), data->GetSource(sprite), data->GetDest(sprite, position), data->GetOffset(sprite), 0, WHITE);
}

void Player::Destroy() {

}
