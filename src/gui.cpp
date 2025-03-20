#include "gui.hpp"
#include "raylib.h"
#include "render_data.hpp"

Vector2 GetTextOffset(const char* text, Sprites::RenderData* data, float size = 20.0f) {
    return MeasureTextEx(data->GetFont(), text, size, 1.0f) * 0.5f;
}

void DrawTextCentred(const char* text, const Vector2& position, Sprites::RenderData* data, float size = 20.0f, Color tint = LIGHTGRAY) {
    DrawTextEx(data->GetFont(), text, position - GetTextOffset(text, data, size), size, 1.0f, tint);
}

bool Button(Vector2 position, Vector2 size, const char* text, Sprites::RenderData* data) {
    Rectangle rect = { position.x - 0.5f * size.x, position.y - 0.5f * size.y, size.x, size.y };
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawTextureNPatch(data->GetAtlas(), data->Pressed(), rect, { 0 }, 0.0f, LIGHTGRAY);
            DrawTextCentred(text, position, data);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                return true;
            }
        } else {
            DrawTextureNPatch(data->GetAtlas(), data->Normal(), rect, { 0 }, 0.0f, data->HoverTint());
            DrawTextCentred(text, position, data);
        }
    } else {
        DrawTextureNPatch(data->GetAtlas(), data->Normal(), rect, { 0 }, 0.0f, LIGHTGRAY);
        DrawTextCentred(text, position, data);
    }
    return false;
}

bool DrawSlider(Vector2 position, Vector2 size, const char* text, float padding, Sprites::RenderData* data, float* outValue) {
    Rectangle rect = { position.x - 0.5f * size.x, position.y - 0.5f * size.y, size.x, size.y };
    Rectangle slider = { rect.x + padding, rect.y + padding, rect.width - 2 * padding, rect.height - 2 * padding };
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, slider)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            *outValue = (Clamp(mousePos.x, slider.x, slider.x + slider.width) - slider.x) / slider.width;
            slider.width *= *outValue;
            DrawTextureNPatch(data->GetAtlas(), data->Pressed(), rect, { 0 }, 0.0f, LIGHTGRAY);
            DrawRectangleRec(slider, RED);
            DrawTextCentred(TextFormat("%s: %d", text, (int) round(*outValue * 100.0f)), position, data);
            return true;
        } else {
            slider.width *= *outValue;
            DrawTextureNPatch(data->GetAtlas(), data->Normal(), rect, { 0 }, 0.0f, data->HoverTint());
            DrawRectangleRec(slider, RED);
            DrawTextCentred(TextFormat("%s: %d", text, (int) round(*outValue * 100.0f)), position, data);
        }
    } else {
        slider.width *= *outValue;
        DrawTextureNPatch(data->GetAtlas(), data->Normal(), rect, { 0 }, 0.0f, LIGHTGRAY);
        DrawRectangleRec(slider, RED);
        DrawTextCentred(TextFormat("%s: %d", text, (int) round(*outValue * 100.0f)), position, data);
    }
    return false;
}
