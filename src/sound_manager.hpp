#include "raylib.h"
#include "game_config.hpp"

class SoundManager {
    Sound sfx[SFXCount];
    Music bgm[BGMCount];

    BGMType current;
    BGMType target;
    float mixTime = 2.0f;
    float mixTimer;

public:
    SoundManager(std::vector<SFXFile> sfx, std::vector<BGMFile> bgm);
    void PlaySFX(SFXType type);
    void PlayBGM(BGMType type);
    void Update(float dt);
};
