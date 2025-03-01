#include "entity_animator.hpp"
#include "render_data.hpp"
#include <vector>

EntityAnimator::EntityAnimator() : 
    animations(), 
    currentFrame(), 
    currentAnimation(Idle), 
    animationTime(0.2f), 
    currentAnimationTime(0.0f) {
}

void EntityAnimator::Play(Animation animation) {
    if (currentAnimation == animation) {
        return;
    }
    if (animations.find(animation) != animations.end()) {
        currentAnimation = animation;
        currentFrame = 0;
        currentAnimationTime = 0.2f;
    } else {
        TraceLog(LOG_WARNING, "Could not find animation: %s in animations", AnimationToString(animation));
    }
}

void EntityAnimator::Update(float dt) {
    currentAnimationTime += dt;
    while (currentAnimationTime >= animationTime) {
        currentAnimationTime -= animationTime;
        currentFrame = ++currentFrame % animations[currentAnimation].size();
    }
}

Sprites::SpriteID EntityAnimator::GetSprite() {
    return animations[currentAnimation][currentFrame];
}

void EntityAnimator::SetAnimations(Animation animation, std::vector<Sprites::SpriteID> frames) {
    animations[animation] = std::vector<Sprites::SpriteID>();
    for (Sprites::SpriteID frame : frames) {
        animations[animation].push_back(frame);
    }
}
