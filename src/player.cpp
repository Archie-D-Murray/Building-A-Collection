#include "player.hpp"
#include "entity_animator.hpp"
#include "familiar.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "projectile.hpp"
#include "render_data.hpp"


Player::Player(Game* game) : effectable(), health(100.0f) {}

void Player::Init(Game* game) {
    animator.SetAnimations(Idle, game->config->playerStats.idle);
    animator.SetAnimations(Move, game->config->playerStats.move);
    effectable.Init(game);
    position = game->screenSize * 0.5f;
    velocity = { 0 };
    speed = game->config->playerStats.speed;
    dashSpeed = speed * 3.0f;
    dashDuration = 0.25f;
    dashCooldown = 1.0f;
    collisionRadius = game->config->playerStats.collisionRadius;
    health = Health(game->config->playerStats.health);
    AddFamiliar(game, Fire);
    familiarRotation = 0.0f;
    effectable = Effectable();
    effectable.health = &health;
    TraceLog(LOG_INFO, "Player health: %.0f", health.CurrentHealth());
}

void Player::Update(Game* game, float dt) {
    health.Update(dt);
    familiarRotation += dt * PI;
    if (familiarRotation >= 2 * PI) {
        familiarRotation -= 2 * PI;
    }
    if (effectable.stunned) {
        return;
    }
    effectable.Update(game, position, dt);
    Vector2 input = {
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)),
    };

    if (IsKeyPressed(KEY_SPACE) && dashCooldownTimer <= 0.0f && Vector2LengthSqr(input) > 0) {
        dashTimer += dashDuration;
        dashCooldownTimer += dashCooldown;
        dashDirection = Vector2Normalize(input);
        vulnerable = false;
        game->visualEffects.push_back(VisualEffect(position - dashDirection, 0.48f, game->config->playerStats.dashParticles, Vector2Angle(Vector2 { -1, 0 }, dashDirection * -1.0f) * RAD2DEG));
    }
    if (dashCooldownTimer >= 0.0f) {
        dashCooldownTimer -= dt;
    }
    if (dashTimer <= 0.0f) {
        if (!vulnerable) {
            TraceLog(LOG_INFO, "Set vulnerable to true in velocity handle dashTimer was %f", dashTimer);
        }
        vulnerable = true;
        velocity = Vector2Normalize(input) * (speed * dt * effectable.speedModifier);
        // If frame rate is uncapped then dt = 0.0002 will mean velocity magnitude test fails
        animator.Play(Vector2LengthSqr(input) > 0.01f ? Move : Idle);
    } else {
        vulnerable = false;
        velocity = dashDirection * dashSpeed * dt;
        dashTimer -= dt;
        animator.Play(Move);
    }
    animator.Update(dt);
    float distanceFromCenter = Vector2Length(position - game->screenSize * 0.5f);
    Vector2 dirFromWorld = Vector2Normalize(game->screenSize * 0.5f - position);
    if (distanceFromCenter >= game->worldRadius - collisionRadius && Vector2DotProduct(dirFromWorld, Vector2Normalize(velocity)) < 0.0f) {
        float angle = Vector2Angle(Vector2 { 0, 1 }, position - game->screenSize * 0.5f);
        position = game->screenSize * 0.5f + Vector2Rotate({ 0, game->worldRadius - collisionRadius }, angle);
        // Cancel dash on wall hit
        dashTimer = 0.0f;
        if (!vulnerable) {
            TraceLog(LOG_INFO, "Set vulnerable to true in velocity handle");
        }
        vulnerable = true;
        TraceLog(LOG_INFO, "Dash cancelled");
    } else {
        position += velocity;
    }
}

void Player::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position, 0.0f, vulnerable ? health.DamageTint() : BLUE);
    effectable.Render(data, position);
}

bool Player::IsVulnerable() {
    return vulnerable;
}

void Player::AddFamiliar(Game* game, FamiliarType type) {
    for (Familiar& familiar : game->familiars) {
        if (familiar.type == type && familiar.tier < Epic) {
            familiar.AdvanceTier();
            return;
        }
    }
    game->familiars.push_back(Familiar(position, type, Tier::Common, game));
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
    if (!vulnerable) { return; }
    PlayHitSFX(game);
    health.Damage(game, projectile->damage, position);
    game->soundManager->ContinueCombatMusic();
    projectile->PushVFX(game, position);
    for (Effect& effect : projectile->effects) {
        effectable.AcceptEffect(effect);
    }
    FamiliarDamage(game);
}

void Player::Damage(Game* game, float damage) {
    if (!vulnerable) { return; }
    PlayHitSFX(game);
    health.Damage(game, damage, position);
    game->soundManager->ContinueCombatMusic();
    FamiliarDamage(game);
}

void Player::FamiliarDamage(Game* game) {
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

void Player::PlayHitSFX(Game* game) {
    if (health.IsDead()) {
        game->soundManager->PlaySFX(PlayerDeath);
    } else {
        game->soundManager->PlaySFX(PlayerHit);
    }
}
