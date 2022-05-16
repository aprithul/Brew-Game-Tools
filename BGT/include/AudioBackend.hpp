#ifndef AUDIO_BACKEND_HPP
#define AUDIO_BACKEND_HPP

#include "Types.hpp"

Uint_32 AB_LoadMusic(const char* _fileName);
Uint_32 AB_LoadSoundEffect();

void AB_PlayMusic(Uint_32 musicId, Bool_8 doLoop);
void AB_PauseMusic();
void AB_ResumeMusic();
Bool_8 AB_IsPlayingMusic();
void AB_StopMusic();

void AB_PlaySoundEffect(Uint_32 musicId, Bool_8 doLoop);

void AB_Init();
void AB_Cleanup();


#endif