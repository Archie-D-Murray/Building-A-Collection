#pragma once

#include "raylib.h"
class Game;

class Fader {
    constexpr static Color CLEAR = { 0 };
public:
    Color targetColour = { 0, 0, 0, 0 };
    Color currentColour = { 0, 0, 0, 255 };
    Color initialColour = { 0, 0, 0, 255 };
    float timer = 0.0f;
    float time = 1.0f;
    void Update(Game* game, float dt);
    void StartFade(bool transparent);
    bool FinishedStateFade();
};
