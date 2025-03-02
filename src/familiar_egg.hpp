#pragma once

#include "game_config.hpp"
#include "raylib.h"
#include "render_data.hpp"

class FamiliarEgg {
public:
    FamiliarType type;
    Vector2 position;
    float collisionRadius = 16.0f;
    Sprites::SpriteID sprite;

    FamiliarEgg(const Vector2& position, FamiliarType type);

    void Render(Sprites::RenderData* data) const;
};
