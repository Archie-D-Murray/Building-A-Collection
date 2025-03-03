#include "sound_manager.hpp"
#include "raylib.h"
SoundManager::SoundManager(std::vector<SFXFile> sfxFiles, std::vector<BGMFile> bgmFiles) {
    for (SFXFile& sfxFile : sfxFiles) {
        sfx[sfxFile.type] = LoadSound(sfxFile.file);
    }
    for (BGMFile& bgmFile : bgmFiles) {
        bgm[bgmFile.type] = LoadMusicStream(bgmFile.file);
    }
}
void SoundManager::PlaySFX(SFXType type) {
    PlaySound(sfx[type]);
}
void SoundManager::PlayBGM(BGMType type) {
    if (target != current) { return; }
    target = type;
    mixTimer = 0.0f;
}

void SoundManager::Update(float dt) {
    if (target != current) {
        mixTimer += dt;
        if (current != BGMNone) {
            SetMusicVolume(bgm[current], Clamp(1.0f - mixTimer / mixTime, 0.0f, 1.0f));
        }
        if (target != BGMNone) {
            if (!IsMusicStreamPlaying(bgm[target])) {
                PlayMusicStream(bgm[target]);
            }
            SetMusicVolume(bgm[target], Clamp(mixTimer / mixTime, 0.0f, 1.0f));
        }
        if (mixTimer >= mixTime) {
            StopMusicStream(bgm[current]);
            current = target;
            if (current != BGMNone) {
            }
        }
    }   
}
