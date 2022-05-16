#include "AudioBackend.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <unordered_map>

Uint_32 nextMusicId = 0;
std::unordered_map<Uint_32, Mix_Music*> loadedMusic;


Uint_32 AB_LoadMusic(const char* _fileName)
{
    Mix_Music* _music = Mix_LoadMUS(_fileName);
    if(_music)
    {
        loadedMusic[nextMusicId] = _music;
        nextMusicId++;
    }
    else
        printf( "Failed to load music %s ! SDL_mixer Error: %s\n", _fileName, Mix_GetError());
    
    return nextMusicId-1;
    
}

Uint_32 AB_LoadSoundEffect()
{

    return 0;
}

void AB_PlayMusic(Uint_32 musicId, Bool_8 doLoop)
{
    if(loadedMusic.find(musicId) != loadedMusic.end())
        Mix_PlayMusic( loadedMusic[musicId], doLoop? -1 : 1); // -1 == play infinite times
    else
        printf("Music with id : %d not found\n");
}

void AB_PauseMusic()
{
    Mix_PauseMusic();
}

void AB_ResumeMusic()
{
    Mix_ResumeMusic();
}

void AB_StopMusic()
{
    Mix_HaltMusic();
}

void AB_PlaySoundEffect(Uint_32 soundEffectId)
{

}

Bool_8 AB_IsPlayingMusic()
{
    return !Mix_PausedMusic() && Mix_PlayingMusic();
}

void AB_Init()
{
    if(SDL_Init(SDL_INIT_AUDIO) == 0)
    {
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );    
        }
        else
            printf("SDL_mixer initialized\n");
    }
    else
        printf("Couldn't initialize sdl Audio\n");
}


void AB_Cleanup()
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



