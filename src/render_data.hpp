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
        Player0,
        Player1,
        Player2,
        FamiliarOutline,
        FireFamiliar0,
        FireFamiliar1,
        FireFamiliar2,
        WaterFamiliar0,
        WaterFamiliar1,
        WaterFamiliar2,
        EarthFamiliar0,
        EarthFamiliar1,
        EarthFamiliar2,
        LightningFamiliar0,
        LightningFamiliar1,
        LightningFamiliar2,
        FireFamiliarEgg,
        WaterFamiliarEgg,
        EarthFamiliarEgg,
        LightningFamiliarEgg,
        FamiliarEggIndicator,
        FireProjectile0,
        FireProjectile1,
        WaterProjectile0,
        WaterProjectile1,
        EarthProjectile0,
        EarthProjectile1,
        LightningProjectile0,
        LightningProjectile1,
        EarthVisual,
        LightningVisual0,
        LightningVisual1,
        LightningVisual2,
        NormalTypeEnemy0,
        NormalTypeEnemy1,
        NormalTypeEnemy2,
        HeavyTypeEnemy0,
        HeavyTypeEnemy1,
        HeavyTypeEnemy2,
        EnemyProjectile0,
        EnemyProjectile1,
        HealthBarOverlay,
        HealthBarBackground,
        Lamp,
        Count 
    };
    const static float SPRITE_SIZE = 16.0f;

    class RenderData {
    private:
        Texture2D atlas;
        Texture2D worldMask;
        Font font;
        NPatchInfo normal;
        NPatchInfo pressed;
        Color hoverTint = { 0xA8, 0xA8, 0xA8, 0xA8 };

        const Sprite sprites[SpriteID::Count] = {
            [Player0]               = CreateSprite({ 0 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [Player1]               = CreateSprite({ 0 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [Player2]               = CreateSprite({ 0 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliar0]         = CreateSprite({ 1 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliar1]         = CreateSprite({ 1 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliar2]         = CreateSprite({ 1 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliar0]        = CreateSprite({ 2 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliar1]        = CreateSprite({ 2 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliar2]        = CreateSprite({ 2 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliar0]        = CreateSprite({ 3 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliar1]        = CreateSprite({ 3 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliar2]        = CreateSprite({ 3 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliar0]    = CreateSprite({ 4 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliar1]    = CreateSprite({ 4 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliar2]    = CreateSprite({ 4 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliarEgg]       = CreateSprite({ 5 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliarEgg]      = CreateSprite({ 6 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliarEgg]      = CreateSprite({ 7 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliarEgg]  = CreateSprite({ 8 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireProjectile0]       = CreateSprite({ 5 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireProjectile1]       = CreateSprite({ 5 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterProjectile0]      = CreateSprite({ 6 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterProjectile1]      = CreateSprite({ 6 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthProjectile0]      = CreateSprite({ 7 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthProjectile1]      = CreateSprite({ 7 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningProjectile0]  = CreateSprite({ 8 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningProjectile1]  = CreateSprite({ 8 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [NormalTypeEnemy0]      = CreateSprite({ 9 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [NormalTypeEnemy1]      = CreateSprite({ 9 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [NormalTypeEnemy2]      = CreateSprite({ 9 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [HeavyTypeEnemy0]       = CreateSprite({10 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [HeavyTypeEnemy1]       = CreateSprite({10 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [HeavyTypeEnemy2]       = CreateSprite({10 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EnemyProjectile0]      = CreateSprite({11 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EnemyProjectile1]      = CreateSprite({11 * SPRITE_SIZE, 1 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),

            //---------------------------------------------------------------------------------------------------------------------------\\
            // Visual Effect                                                                                                             \\
            //---------------------------------------------------------------------------------------------------------------------------\\

            [EarthVisual]           = CreateSprite({  1 * SPRITE_SIZE, 10 * SPRITE_SIZE, 2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, { SPRITE_SIZE, SPRITE_SIZE }),
            [LightningVisual0]      = CreateSprite({  4 * SPRITE_SIZE, 10 * SPRITE_SIZE, 2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, { SPRITE_SIZE, SPRITE_SIZE }),
            [LightningVisual1]      = CreateSprite({  6 * SPRITE_SIZE, 10 * SPRITE_SIZE, 2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, { SPRITE_SIZE, SPRITE_SIZE }),
            [LightningVisual2]      = CreateSprite({  8 * SPRITE_SIZE, 10 * SPRITE_SIZE, 2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, { SPRITE_SIZE, SPRITE_SIZE }),
            [FamiliarOutline]       = CreateSprite({ 10 * SPRITE_SIZE, 10 * SPRITE_SIZE, 2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, { SPRITE_SIZE, SPRITE_SIZE }),
            [FamiliarEggIndicator]  = CreateSprite({  0 * SPRITE_SIZE, 11 * SPRITE_SIZE, 1 * SPRITE_SIZE, 1 * SPRITE_SIZE}, { 8, 8 }),

            //---------------------------------------------------------------------------------------------------------------------------\\
            // UI                                                                                                                        \\
            //---------------------------------------------------------------------------------------------------------------------------\\

            [HealthBarOverlay]      = CreateSprite({ 0 * SPRITE_SIZE, 3 * SPRITE_SIZE + 5, 6.0f * SPRITE_SIZE, 6}, { 3 * SPRITE_SIZE, 3 } ),
            [HealthBarBackground]   = CreateSprite({ 6 * SPRITE_SIZE, 3 * SPRITE_SIZE + 5, 6.0f * SPRITE_SIZE, 6}, { 3 * SPRITE_SIZE, 3 } ),

            //---------------------------------------------------------------------------------------------------------------------------\\
            // Background                                                                                                                \\
            //---------------------------------------------------------------------------------------------------------------------------\\

            [Lamp]                  = CreateSprite({ 3 * SPRITE_SIZE, 10 * SPRITE_SIZE, SPRITE_SIZE, 2 * SPRITE_SIZE}, { 8, SPRITE_SIZE }),
    };
    public:
        RenderData(const char* atlasFileName, const char* fontFileName, const char* maskFileName) {
            atlas = LoadTexture(atlasFileName);
            worldMask = LoadTexture(maskFileName);
            SetTextureFilter(atlas, TEXTURE_FILTER_POINT);
            SetTextureFilter(worldMask, TEXTURE_FILTER_POINT);
            font = LoadFont(fontFileName);
            normal = {
                .source = { 0, 4 * SPRITE_SIZE, 6 * SPRITE_SIZE, 6 * SPRITE_SIZE },
                .left = 10,
                .top = 10,
                .right = 10,
                .bottom = 10,
                .layout = NPATCH_NINE_PATCH,
            };
            pressed = NPatchInfo {
                .source = { 6 * SPRITE_SIZE, 4 * SPRITE_SIZE, 6 * SPRITE_SIZE, 6 * SPRITE_SIZE },
                .left = 10,
                .top = 10,
                .right = 10,
                .bottom = 10,
                .layout = NPATCH_NINE_PATCH,
            };
        }
        static inline Sprite CreateSprite(const Rectangle rect, const Vector2 offset) {
            return Sprite { .spriteRect = rect, .offset = offset };
        }

        inline Texture2D& GetAtlas() {
            return atlas;
        }

        inline Texture2D& WorldMask() {
            return worldMask;
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

        void Unload() {
            UnloadTexture(atlas);
            UnloadFont(font);
        }

        std::string static RectToString(const Rectangle& rectangle) {
            return TextFormat("x: %.0f, y: %.0f, width: %.0f, height: %.0f", rectangle.x, rectangle.y, rectangle.width, rectangle.height);
        }

        std::string static VectorToString(const Vector2& vector) {
            return TextFormat("x: %.0f, y: %.0f", vector.x, vector.y);
        }

        void SpriteInfo(SpriteID sprite) {
            TraceLog(LOG_INFO, "Sprite: %s\n\t\tSource: %s\n\t\tDest: %s\n\t\tOffset: %s", std::to_string(sprite).c_str(), RectToString(GetSource(sprite)).c_str(), RectToString(GetDest(sprite, {0})).c_str(), VectorToString(GetOffset(sprite)).c_str());
        }

        inline NPatchInfo& Normal() {
            return normal;
        }

        inline NPatchInfo& Pressed() {
            return pressed;
        }

        inline Color HoverTint() {
            return hoverTint;
        }
    };
}
