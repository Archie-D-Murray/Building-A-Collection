#include "projectile.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include "render_data.hpp"
#include "vfx.hpp"

Projectile::Projectile(Vector2 position, Vector2 direction, float speed, float collisionRadius, float damage, std::vector<Sprites::SpriteID> frames) :
    position(position),
    direction(direction),
    speed(speed),
    collisionRadius(collisionRadius),
    damage(damage) {
    angle = Vector2Angle({ 0, -1.0f }, direction) * RAD2DEG;
    animator.SetAnimations(Idle, frames);
}

void Projectile::AddVFX(float duration, std::vector<Sprites::SpriteID> frames) {
    data.frames = frames;
    data.duration = duration;
    data.hasVFX = true;
}

void Projectile::Update(float dt) {
    position += direction * (speed * dt);
    animator.Update(dt);
}

void Projectile::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position, angle);
}

void Projectile::PushVFX(Game* game, const Vector2& position) {
    if (data.hasVFX) {
        game->visualEffects.push_back(VisualEffect(position, data.duration, data.frames));
        data.hasVFX = false;
    }
}

bool Projectile::OffScreen(Game* game) {
    return Vector2Distance(position, game->screenSize * 0.5f) >= game->worldRadius - collisionRadius;
}
