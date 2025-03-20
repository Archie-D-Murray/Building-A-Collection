#pragma once

#include "raylib.h"
#include "render_data.hpp"
Vector2 GetTextOffset(const char* text, Sprites::RenderData* data, float size);
void DrawTextCentred(const char* text, const Vector2& position, Sprites::RenderData* data, float size, Color tint);
bool Button(Vector2 position, Vector2 size, const char* text, Sprites::RenderData* data);
bool DrawSlider(Vector2 position, Vector2 size, const char* text, float padding, Sprites::RenderData* data, float* outValue);
