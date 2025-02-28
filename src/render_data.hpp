#pragma once
#include "raylib.h"

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
        Count 
    };
    const static float SPRITE_SIZE = 16.0f;

    class RenderData {
    private:
        Texture2D atlas;

        const Sprite sprites[SpriteID::Count] = {
            [Player] =               CreateSprite({ 0 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliar] =         CreateSprite({ 1 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliar] =        CreateSprite({ 2 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliar] =        CreateSprite({ 3 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliar] =    CreateSprite({ 4 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireFamiliarEgg] =      CreateSprite({ 5 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterFamiliarEgg] =     CreateSprite({ 6 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthFamiliarEgg] =     CreateSprite({ 7 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningFamiliarEgg] = CreateSprite({ 8 * SPRITE_SIZE, 2 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [FireProjectile] =       CreateSprite({ 5 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [WaterProjectile] =      CreateSprite({ 6 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EarthProjectile] =      CreateSprite({ 7 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [LightningProjectile] =  CreateSprite({ 8 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [NormalTypeEnemy]     =  CreateSprite({ 9 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [HeavyTypeEnemy]      =  CreateSprite({10 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
            [EnemyProjectile]     =  CreateSprite({11 * SPRITE_SIZE, 0 * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE}, { 8, 8 } ),
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
    };
}
