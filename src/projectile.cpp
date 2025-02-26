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
    damage(damage) {
    angle = Vector2Angle({ 0, 1 }, direction);
}

void Projectile::Update(float dt) {
    position += direction * (speed * dt);
}

void Projectile::Render(Sprites::RenderData* data) {
    DrawCircleV(position, 10, ORANGE);
}

bool Projectile::OffScreen(Game* game) {
    return !CheckCollisionPointRec(position, Rectangle { 0, 0, game->screenSize.x, game->screenSize.y });
}
