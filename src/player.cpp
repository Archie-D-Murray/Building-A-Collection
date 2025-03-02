#include "player.hpp"
#include "familiar.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "projectile.hpp"
#include "render_data.hpp"


Player::Player(Game* game) : effectable(), health(100.0f) {}

void Player::Init(Game* game) {
    animator.SetAnimations(Idle, game->config.playerStats.idle);
    animator.SetAnimations(Move, game->config.playerStats.move);
    TraceLog(LOG_INFO, "Initilised player animations");
    position = game->screenSize * 0.5f;
    velocity = { 0 };
    speed = game->config.playerStats.speed;
    collisionRadius = game->config.playerStats.collisionRadius;
    health = Health(game->config.playerStats.health);
    AddFamiliar(game, Fire);
    effectable = Effectable();
    effectable.health = &health;
    TraceLog(LOG_INFO, "Player health: %.0f", health.CurrentHealth());
}

void Player::Update(Game* game, float dt) {
    familiarRotation += dt * PI;
    if (effectable.stunned) {
        return;
    }
    effectable.Update(dt);
    Vector2 input = {
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)),
    };

    velocity = Vector2Normalize(input) * (speed * dt * effectable.speedModifier);
    animator.Play(Vector2LengthSqr(velocity) > 0.01f ? Move : Idle);
    animator.Update(dt);
    float distanceFromCenter = Vector2Length(position - game->screenSize * 0.5f);
    Vector2 dirFromWorld = Vector2Normalize(game->screenSize * 0.5f - position);
    if (distanceFromCenter >= game->worldRadius - collisionRadius && Vector2DotProduct(dirFromWorld, Vector2Normalize(velocity)) < 0.0f) {
        float angle = Vector2Angle(Vector2 { 0, 1 }, position - game->screenSize * 0.5f);
        position = game->screenSize * 0.5f + Vector2Rotate({ 0, game->worldRadius - collisionRadius }, angle);
    } else {
        position += velocity;
    }
}

void Player::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position);
}

void Player::AddFamiliar(Game* game, FamiliarType type) {
    for (Familiar& familiar : game->familiars) {
        if (familiar.type == type && familiar.tier < Epic) {
            familiar.AdvanceTier();
            return;
        }
    }
    game->familiars.push_back(Familiar(position, type, Tier::Common, game->config));
}

void Player::Destroy() {

}

Vector2 Player::Velocity() const { 
    return velocity; 
}

bool Player::Collides(Projectile* projectile) {
    return CheckCollisionCircles(projectile->position, projectile->collisionRadius, position, collisionRadius);
}

void Player::Damage(Game* game, Projectile* projectile) {
    TraceLog(LOG_INFO, "Player was hit for %0.0f damage", projectile->damage);
    health.Damage(projectile->damage);
    projectile->PushVFX(game);
    game->damageNumberManager.PushDamageNumber(projectile->damage, projectile->position);
    for (Effect& effect : projectile->effects) {
        effectable.AcceptEffect(effect);
    }
    if (game->familiars.empty()) {
        return;
    }
    if (game->familiars.back().tier == Tier::Common) {
        TraceLog(LOG_INFO, "Familiar tanked hit");
        game->familiars.pop_back();
    } else {
        TraceLog(LOG_INFO, "Familiar dropped tier");
        game->familiars.back().DropTier();
    }
}
