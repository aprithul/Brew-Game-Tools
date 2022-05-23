#include "AudioManager.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <unordered_map>
#include "Utils.hpp"

std::unordered_map<Uint_32, Mix_Music*> loadedMusic;
std::unordered_map<Uint_32, Mix_Chunk*> loadedChunk;
Uint_32 nextMusicId = 0;
Uint_32 nextChunkId = 0;

AudioManager::AudioManager(Int_32 _frequency, Int_32 _channels, Int_32 _chunkSize)
{
    
    if(SDL_Init(SDL_INIT_AUDIO) == 0)
    {
        if( Mix_OpenAudio( _frequency, MIX_DEFAULT_FORMAT, _channels, _chunkSize ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );    
        }
        else
            printf("SDL_mixer initialized\n");
    }
    else
        printf("Couldn't initialize sdl Audio\n");
}

AudioManager::~AudioManager()
{
    for(auto& _music : loadedMusic)
    {
        Mix_FreeMusic(_music.second);
        _music.second = nullptr;
    }

    Mix_Quit();
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    printf("SDL2_mixer audio backend cleaned\n");
    
}


Uint_32 AudioManager::LoadMusic(const char* _filename)
{
    Mix_Music* _music = Mix_LoadMUS(_filename);
    if(_music)
    {
        nextMusicId++;
        loadedMusic[nextMusicId] = _music;
    }
    else
        printf( "Failed to load music %s ! SDL_mixer Error: %s\n", _filename, Mix_GetError());
    
    return nextMusicId;
}

void AudioManager::PlayMusic(Uint_32 musicId, Bool_8 doLoop)
{
    if(loadedMusic.find(musicId) != loadedMusic.end())
        Mix_PlayMusic( loadedMusic[musicId], doLoop? -1 : 1); // -1 == play infinite times
    else
        printf("Music with id : %u not found\n", musicId);
}

void AudioManager::SetMusicVolume(Float_32 _volume)
{
    _volume = Clamp(_volume, 0, 1);
    Int_32 _volumeI = (Int_32)(_volume*255);
    Mix_VolumeMusic(_volumeI);
}

void AudioManager::PauseMusic()
{
    Mix_PauseMusic();
}

void AudioManager::ResumeMusic()
{
    Mix_ResumeMusic();
}

Bool_8 AudioManager::IsPlayingMusic()
{
    return !Mix_PausedMusic() && Mix_PlayingMusic();
}

void AudioManager::StopMusic()
{
    Mix_HaltMusic();
}

Uint_32 AudioManager::LoadSoundEffect(const char* _filename)
{
    

    Mix_Chunk* _chunk = Mix_LoadWAV(_filename);
    if(_chunk)
    {
        nextChunkId++;
        loadedChunk[nextChunkId] = _chunk;
    }
    else
        printf( "Failed to load sound effect %s ! SDL_mixer Error: %s\n", _filename, Mix_GetError());
    
    return nextChunkId;
}

void AudioManager::PlaySoundEffect(Uint_32 _soundEffect)
{
     if(loadedChunk.find(_soundEffect) != loadedChunk.end())
        Mix_PlayChannel(-1, loadedChunk[_soundEffect], 0); // -1 == play infinite times
    else
        printf("Sound effect with id : %u not found\n", _soundEffect);
}


void AudioManager::SetSoundEffectVolume(Float_32 _volume)
{
    _volume = Clamp(_volume, 0, 1);
    Int_32 _volumeI = (Int_32)(_volume*255);
    Mix_Volume(-1, _volumeI);
}