#include "familiar_egg.hpp"

FamiliarEgg::FamiliarEgg(const Vector2& position, FamiliarType type) :
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

void FamiliarEgg::Render(Sprites::RenderData* data) const {
    data->DrawSprite(sprite, position);
}
