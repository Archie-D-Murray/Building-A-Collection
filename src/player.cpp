#include "player.hpp"
#include "raylib.h"
#include "raymath.h"

void Player::Init() {
    position = { 720, 405 };
    velocity = { 0 };
    speed = 100.0f;
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
