#include "fader.hpp"
#include "game.hpp"
#include "raylib.h"

void Fader::Update(Game* game, float dt) {
    if (timer == time) { return; }
    timer = fmaxf(timer + dt, time);
    currentColour = ColorLerp(initialColour, targetColour, timer / time);
    DrawRectangleV({ 0 }, game->screenSize, currentColour);
}

void Fader::StartFade(bool transparent) {
    timer = 0.0f;
    if (transparent) {
        initialColour = BLACK;
        targetColour = CLEAR;
    } else {
        initialColour = CLEAR;
        targetColour = BLACK;
    }
}

bool Fader::FinishedStateFade() {
    return targetColour.a == 255 && timer >= time;
}
