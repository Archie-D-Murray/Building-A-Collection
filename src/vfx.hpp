#pragma once

#include "entity_animator.hpp"
#include "raylib.h"
#include "render_data.hpp"

class VisualEffect {
    EntityAnimator animator;
    Vector2 position;
    float duration;
    float timer = 0.0f;

public:
    VisualEffect(Vector2 position, float duration, std::vector<Sprites::SpriteID> frames);
    void Update(float dt);
    void Render(Sprites::RenderData* data);
    bool Finished();
};
