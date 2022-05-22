#ifndef AUDIO_SOURCE
#define AUDIO_SOURCE

#include "Types.hpp"
#include <unordered_map>

class AudioManager
{
private:
public:
    AudioManager(Int_32 _frequency, Int_32 _channels, Int_32 _chunkSize);
    ~AudioManager();
    Uint_32 LoadMusic(const char* _filename);
    void PlayMusic(Uint_32 _music, Bool_8 doLoop);
    void PauseMusic();
    void ResumeMusic();
    Bool_8 IsPlayingMusic();
    void StopMusic();
    void SetMusicVolume(Float_32 _volume);


    Uint_32 LoadSoundEffect(const char* _filename);
    void PlaySoundEffect(Uint_32 _soundEffect);
    void SetSoundEffectVolume(Float_32 _volume);

};

#endif // Audio source