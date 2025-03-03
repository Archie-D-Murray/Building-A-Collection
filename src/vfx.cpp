#include "vfx.hpp"

VisualEffect::VisualEffect(Vector2 position, float duration, std::vector<Sprites::SpriteID> frames, float rotation) {
    this->position = position;
    this->duration = duration;
    this->rotation = rotation;
    timer = 0.0f;
    animator.SetAnimations(Idle, frames);
}
void VisualEffect::Update(float dt) {
    timer += dt;
    animator.Update(dt);
}
void VisualEffect::Render(Sprites::RenderData* data) {
    data->DrawSprite(animator.GetSprite(), position, rotation);
}
bool VisualEffect::Finished() {
    return timer >= duration;
}
