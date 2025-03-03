#include "raylib.h"
#include "game_config.hpp"

class SoundManager {
    Sound sfx[SFXCount];
    Music bgm[BGMCount];

    BGMType current = BGMNone;
    BGMType target = BGMNone;
    float mixTime = 2.0f;
    float mixTimer = 0.0f;
    float combatTimer = 0.0f;
    float combatTime = 5.0f;

public:
    float globalVolume = 0.5f;
    SoundManager(std::vector<SFXFile> sfx, std::vector<BGMFile> bgm);
    ~SoundManager();
    void PlaySFX(SFXType type);
    void PlayBGM(BGMType type);
    void ContinueCombatMusic();
    void Update(float dt);
    void DrawUI();
};
