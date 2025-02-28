#include "projectile.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include "render_data.hpp"
Projectile::Projectile(Vector2 position, Vector2 direction, float speed, float collisionRadius, float damage, Sprites::SpriteID sprite) :
    position(position),
    direction(direction),
    speed(speed),
    collisionRadius(collisionRadius),
    sprite(sprite),
    damage(damage) {
    angle = Vector2Angle({ 0, -1.0f }, direction) * RAD2DEG;
}

void Projectile::Update(float dt) {
    position += direction * (speed * dt);
}

void Projectile::Render(Sprites::RenderData* data) {
    data->DrawSprite(sprite, position, angle);
}

bool Projectile::OffScreen(Game* game) {
    return !CheckCollisionPointRec(position, Rectangle { 0, 0, game->screenSize.x, game->screenSize.y });
}
