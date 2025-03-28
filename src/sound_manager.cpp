#include "sound_manager.hpp"
#include "raylib.h"
SoundManager::SoundManager(std::vector<SFXFile> sfxFiles, std::vector<BGMFile> bgmFiles, GameConfig* config) {
    for (SFXFile& sfxFile : sfxFiles) {
        sfx[sfxFile.type] = LoadSound(sfxFile.file);
        SetSoundVolume(sfx[sfxFile.type], globalVolume * sfxVolume);
    }
    for (BGMFile& bgmFile : bgmFiles) {
        bgm[bgmFile.type] = LoadMusicStream(bgmFile.file);
    }
    globalVolume = config->soundSettings.globalVolume;
    bgmVolume = config->soundSettings.bgmVolume;
    sfxVolume = config->soundSettings.sfxVolume;
}
void SoundManager::PlaySFX(SFXType type) {
    SetSoundVolume(sfx[type], globalVolume * sfxVolume);
    SetSoundPitch(sfx[type], (float) GetRandomValue(45, 55) * 0.01f);
    PlaySound(sfx[type]);
}
void SoundManager::PlayBGM(BGMType type) {
    if (target != current) { return; }
    target = type;
    mixTimer = 0.0f;
}

void SoundManager::ContinueCombatMusic() {
    combatTimer = fminf(combatTimer + combatTime, combatTime);
    PlayBGM(Combat);
}

void SoundManager::Update(float dt) {
    if (IsKeyPressed(KEY_M)) {
        PlayBGM(Passive);
    }
    if (combatTimer >= 0.0f) { // Playing combat music
        combatTimer -= dt;
        if (combatTimer <= 0.0f) { // First case of done with combat music
            PlayBGM(Passive);
        }
    }
    if (target != current) {
        mixTimer += dt;
        if (current != BGMNone) {
            SetMusicVolume(bgm[current], Clamp(1.0f - mixTimer / mixTime, 0.0f, 1.0f) * globalVolume * bgmVolume);
            UpdateMusicStream(bgm[current]);
        }
        if (target != BGMNone) {
            if (!IsMusicStreamPlaying(bgm[target])) {
                PlayMusicStream(bgm[target]);
            }
            SetMusicVolume(bgm[target], Clamp(mixTimer / mixTime, 0.0f, 1.0f) * globalVolume * bgmVolume);
            UpdateMusicStream(bgm[target]);
        }
        if (mixTimer >= mixTime) {
            if (current != BGMNone) {
                StopMusicStream(bgm[current]);
            }
            current = target;
        }
    } else if (current != BGMNone) {
        UpdateMusicStream(bgm[current]);
    }

}

SoundManager::~SoundManager() {
    for (size_t i = SFXNone + 1; i < (size_t) SFXCount; i++) {
        UnloadSound(sfx[i]);
    }
    for (size_t i = BGMNone + 1; i < (size_t) BGMCount; i++) {
        UnloadMusicStream(bgm[i]);
    }
}

void SoundManager::DrawUI() {
    /*DrawText(TextFormat("BGM target: %d, BGM current %d, combat timer: %.2f, mixTimer = %.2f", target, current, combatTimer, mixTimer), 10, 70, 14, WHITE);*/
}

void SoundManager::UpdateVolume() {
    for (size_t i = SFXNone + 1; i < SFXCount; i++) {
        SetSoundVolume(sfx[i], globalVolume * sfxVolume);
    }
    for (size_t i = BGMNone + 1; i < BGMCount; i++) {
        SetMusicVolume(bgm[i], globalVolume * bgmVolume);
    }
}
