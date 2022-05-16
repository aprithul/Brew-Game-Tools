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
};

#endif // Audio source