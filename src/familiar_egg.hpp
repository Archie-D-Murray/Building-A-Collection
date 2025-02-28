#pragma once

#include "game_config.hpp"
#include "raylib.h"
#include "render_data.hpp"

class FamiliarEgg {
public:
    Tier tier;
    FamiliarType type;
    Vector2 position;
    float collisionRadius = 16.0f;
    Sprites::SpriteID sprite;

    FamiliarEgg(const Vector2& position, FamiliarType type, Tier tier) :
        tier(tier),
        type(type),
        position(position)
        {
        switch (type) {
        case Fire:
            sprite = Sprites::FireFamiliarEgg;
            break;
        case Water:
            sprite = Sprites::WaterFamiliarEgg;
            break;
        case Earth:
            sprite = Sprites::EarthFamiliarEgg;
            break;
        case Lightning:
            sprite = Sprites::LightningFamiliarEgg;
            break;
        default:
            break;
        }
    };

    void Render(Sprites::RenderData* data) const {
        data->DrawSprite(sprite, position);
    }
};
