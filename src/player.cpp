#include "player.hpp"
#include "familiar.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "projectile.hpp"


Player::Player(Game* game) : effectable(), health(100.0f) {}

void Player::Init(Game* game) {
    sprite = Sprites::Player;
    position = game->screenSize * 0.5f / game->zoom;
    velocity = { 0 };
    speed = game->config.playerStats.speed;
    collisionRadius = game->config.playerStats.collisionRadius;
    health = Health(game->config.playerStats.health);
    AddFamiliar(game, Fire, Common);
    effectable = Effectable();
    effectable.health = &health;
    TraceLog(LOG_INFO, "Player health: %.0f", health.CurrentHealth());
}

void Player::Update(float dt) {
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
    position += velocity;
}

void Player::Render(Sprites::RenderData* data) {
    DrawTexturePro(data->GetAtlas(), data->GetSource(sprite), data->GetDest(sprite, position), data->GetOffset(sprite), 0, WHITE);
}

void Player::AddFamiliar(Game* game, FamiliarType type, Tier tier) {
    for (Familiar& familiar : game->familiars) {
        if (familiar.type == type && familiar.tier < Epic) {
            familiar.AdvanceTier();
            return;
        }
    }
    game->familiars.push_back(Familiar(position, type, tier, game->config));
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
    if (game->familiars.empty()) {
        TraceLog(LOG_INFO, "Player was hit for %0.0f damage", projectile->damage);
        health.Damage(projectile->damage);
        game->damageNumberManager.PushDamageNumber(projectile->damage, projectile->position);
        for (Effect& effect : projectile->effects) {
            effectable.AcceptEffect(effect);
        }
    } else {
        if (game->familiars.back().tier == Tier::Common) {
            TraceLog(LOG_INFO, "Familiar tanked hit");
            game->familiars.pop_back();
        } else {
            TraceLog(LOG_INFO, "Familiar dropped tier");
            game->familiars.back().DropTier();
        }
    }
}
