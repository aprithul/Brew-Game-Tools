#include "AudioManager.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <unordered_map>

std::unordered_map<Uint_32, Mix_Music*> loadedMusic;
Uint_32 nextMusicId = 0;

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
        printf("Music with id : %d not found\n");
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