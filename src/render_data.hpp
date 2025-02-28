#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>

namespace Sprites {
    struct Sprite {
        Rectangle spriteRect;
        Vector2 offset;
    };
    enum SpriteID { 
        Player,
        FireFamiliar,
        WaterFamiliar,
        EarthFamiliar,
        LightningFamiliar,
        FireFamiliarEgg,
        WaterFamiliarEgg,
        EarthFamiliarEgg,
        LightningFamiliarEgg,
        FireProjectile,
        WaterProjectile,
        EarthProjectile,
        LightningProjectile,
        NormalTypeEnemy,
        HeavyTypeEnemy,
        EnemyProjectile,
        HealthBarOverlay,
        HealthBarBackground,
        Count 
    };
    const static float SPRITE_SIZE = 16.0f;

    class RenderData {
    private:
        Texture2D atlas;
        Font font;

        const Sprite sprites[SpriteID::Count] = {
            [Player]               = CreateSprite({ 0 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliar]         = CreateSprite({ 1 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliar]        = CreateSprite({ 2 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliar]        = CreateSprite({ 3 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliar]    = CreateSprite({ 4 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliarEgg]      = CreateSprite({ 5 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliarEgg]     = CreateSprite({ 6 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliarEgg]     = CreateSprite({ 7 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliarEgg] = CreateSprite({ 8 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireProjectile]       = CreateSprite({ 5 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterProjectile]      = CreateSprite({ 6 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthProjectile]      = CreateSprite({ 7 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningProjectile]  = CreateSprite({ 8 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [NormalTypeEnemy]      = CreateSprite({ 9 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [HeavyTypeEnemy]       = CreateSprite({10 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EnemyProjectile]      = CreateSprite({11 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),

            //---------------------------------------------------------------------------------------------------------------------------\\
            // UI                                                                                                                        \\
            //---------------------------------------------------------------------------------------------------------------------------\\

            [HealthBarOverlay]     = CreateSprite({ 0 * SPRITE_SIZE, 3 * SPRITE_SIZE + 5, 6.0f * SPRITE_SIZE, 6}, { 3 * SPRITE_SIZE, 3 } ),
            [HealthBarBackground]  = CreateSprite({ 6 * SPRITE_SIZE, 3 * SPRITE_SIZE + 5, 6.0f * SPRITE_SIZE, 6}, { 3 * SPRITE_SIZE, 3 } ),
        };
    public:
        RenderData(const char* atlasFileName, const char* fontFileName) {
            atlas = LoadTexture(atlasFileName);
            font = LoadFont(fontFileName);
        }
        static inline Sprite CreateSprite(const Rectangle rect, const Vector2 offset) {
            return Sprite { .spriteRect = rect, .offset = offset };
        }

        inline Texture2D& GetAtlas() {
            return atlas;
        }

        inline Font& GetFont() {
            return font;
        }

        inline Sprite GetSprite(SpriteID sprite) {
            return sprites[sprite];
        };

        inline Rectangle GetSource(SpriteID sprite) {
            return sprites[sprite].spriteRect;
        }

        inline Vector2 GetOffset(SpriteID sprite) {
            return sprites[sprite].offset;
        }

        inline Rectangle GetDest(SpriteID sprite, Vector2 position) {
            return Rectangle {
                .x = position.x,
                .y = position.y,
                .width = sprites[sprite].spriteRect.width,
                .height = sprites[sprite].spriteRect.height,
            };
        };

        void DrawSprite(SpriteID sprite, const Vector2& position, float rotation = 0.0f, Color tint = WHITE) {
            DrawTexturePro(GetAtlas(), GetSource(sprite), GetDest(sprite, position), GetOffset(sprite), rotation, tint);
        }

        void DrawSpriteSize(SpriteID sprite, const Vector2& position, float scale, Color tint = WHITE) {
            Vector2 size = Vector2Scale({ sprites[sprite].spriteRect.width, sprites[sprite].spriteRect.height }, scale);
            DrawTexturePro(GetAtlas(), GetSource(sprite), Rectangle { position.x, position.y, size.x, size.y }, GetOffset(sprite) * scale, 0.0f, tint);
        }

        std::string RectToString(const Rectangle& rectangle) {
            return TextFormat("x: %.0f, y: %.0f, width: %.0f, height: %.0f", rectangle.x, rectangle.y, rectangle.width, rectangle.height);
        }

        std::string VectorToString(const Vector2& vector) {
            return TextFormat("x: %.0f, y: %.0f", vector.x, vector.y);
        }

        void SpriteInfo(SpriteID sprite) {
            TraceLog(LOG_INFO, "Sprite: %s\n\t\tSource: %s\n\t\tDest: %s\n\t\tOffset: %s", std::to_string(sprite).c_str(), RectToString(GetSource(sprite)).c_str(), RectToString(GetDest(sprite, {0})).c_str(), VectorToString(GetOffset(sprite)).c_str());
        }
    };
}
