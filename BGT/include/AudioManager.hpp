#ifndef AUDIO_SOURCE
#define AUDIO_SOURCE

#include "Types.hpp"
#include <unordered_map>

void Audio_Create(Int_32 _frequency, Int_32 _channels, Int_32 _chunkSize);
void Audio_Close();
Uint_32 Audio_LoadMusic(const char* _filename);
void Audio_PlayMusic(Uint_32 _music, Bool_8 doLoop);
void Audio_PauseMusic();
void Audio_ResumeMusic();
Bool_8 Audio_IsPlayingMusic();
void Audio_StopMusic();
void Audio_SetMusicVolume(Float_32 _volume);


Uint_32 Audio_LoadSoundEffect(const char* _filename);
void Audio_PlaySoundEffect(Uint_32 _soundEffect);
void Audio_SetSoundEffectVolume(Float_32 _volume);

#endif // Audio source