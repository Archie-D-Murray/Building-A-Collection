#include "gui.hpp"
#include "raylib.h"
#include "render_data.hpp"

Vector2 GetTextOffset(const char* text, Sprites::RenderData* data, float size = 20.0f) {
    return MeasureTextEx(data->GetFont(), text, size, 1.0f) * 0.5f;
}

void DrawTextCentred(const char* text, const Vector2& position, Sprites::RenderData* data, float size = 20.0f) {
    DrawTextEx(data->GetFont(), text, position - GetTextOffset(text, data, size), size, 1.0f, LIGHTGRAY);
}

bool Button(Vector2 position, Vector2 size, const char* text, Sprites::RenderData* data) {
    Rectangle rect = { position.x - 0.5f * size.x, position.y - 0.5f * size.y, size.x, size.y };
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawTextureNPatch(data->GetAtlas(), data->Pressed(), rect, { 0 }, 0.0f, WHITE);
            DrawTextCentred(text, position, data);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                return true;
            }
        } else {
            DrawTextureNPatch(data->GetAtlas(), data->Normal(), rect, { 0 }, 0.0f, data->HoverTint());
            DrawTextCentred(text, position, data);
        }
    } else {
        DrawTextureNPatch(data->GetAtlas(), data->Normal(), rect, { 0 }, 0.0f, WHITE);
        DrawTextCentred(text, position, data);
    }
    return false;
}
