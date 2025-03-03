#include "raylib.h"
#include "game_config.hpp"

class SoundManager {
    Sound sfx[SFXCount];
    Music bgm[BGMCount];

    BGMType current;
    BGMType target;
    float mixTime = 2.0f;
    float mixTimer;
    float combatTimer = 0.0f;
    float combatTime = 5.0f;

public:
    SoundManager(std::vector<SFXFile> sfx, std::vector<BGMFile> bgm);
    void PlaySFX(SFXType type);
    void PlayBGM(BGMType type);
    void ContinueCombatMusic();
    void Update(float dt);
};
