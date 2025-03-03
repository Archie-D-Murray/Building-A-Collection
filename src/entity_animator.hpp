#pragma once
#include "render_data.hpp"
#include <map>
#include <vector>

enum Animation { Idle, Move, Attack, OnFire, Iced, Zapped, Grounded };

class EntityAnimator {
    std::map<Animation, std::vector<Sprites::SpriteID>> animations;
    size_t currentFrame = 0;
    Animation currentAnimation = Idle;
    float animationTime = 0.2f;
    float currentAnimationTime = 0.0f;

    static inline const char* AnimationToString(Animation animation) {
        switch (animation) {
        case Idle:
            return "Idle";
        case Move:
            return "Move";
        case Attack:
            return "Attack";
        case OnFire:
            return "OnFire";
        case Iced:
            return "Iced";
        case Zapped:
            return "Zapped";
        case Grounded:
            return "Grounded";
        }
        return "None";
    }

public:
    EntityAnimator();
    void SetAnimations(Animation animation, std::vector<Sprites::SpriteID> frames);
    void Update(float dt);
    void Play(Animation animation);
    Sprites::SpriteID GetSprite();
};
