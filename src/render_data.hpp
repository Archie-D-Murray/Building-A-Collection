#pragma once
#include "raylib.h"
#include "raymath.h"

namespace Sprites {
    struct Sprite {
        Rectangle spriteRect;
        Vector2 offset;
    };
    enum SpriteID { Player, Count };
    const static float PixelPerUnit = 16.0f;

    class RenderData {
    private:
        Texture2D atlas;

        const Sprite sprites[SpriteID::Count] = {
            [Player] = CreateSprite({0, 0, PixelPerUnit, PixelPerUnit}, { 8, 8 } ),
        };
    public:
        RenderData(const char* file) {
            atlas = LoadTexture(file);
        }
        static inline Sprite CreateSprite(const Rectangle rect, const Vector2 offset) {
            return Sprite { .spriteRect = rect, .offset = offset };
        }

        inline Texture2D& GetAtlas() {
            return atlas;
        }

        inline Sprite GetSprite(SpriteID sprite) {
            return sprites[sprite];
        };

        inline Rectangle GetSource(SpriteID sprite) {
            return sprites[sprite].spriteRect;
        }

        inline Vector2 GetOffset(SpriteID sprite) {
            return sprites[sprite].offset * PixelPerUnit;
        }

        inline Rectangle GetDest(SpriteID sprite, Vector2 position) {
            return Rectangle {
                .x = position.x,
                .y = position.y,
                .width = sprites[sprite].spriteRect.width * PixelPerUnit,
                .height = sprites[sprite].spriteRect.height * PixelPerUnit,
            };
        };
    };
}
