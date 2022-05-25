#include "InputManager.hpp"
#include "SDL/SDL.h"
#include <unordered_map>
#include <unordered_set>
#include <stdio.h>


std::unordered_map<BGT_Key, Float_32, std::hash<Int_32>> keyVal;
std::unordered_set<BGT_Key, std::hash<Int_32>> keysPressedThisFrame;
std::unordered_set<BGT_Key, std::hash<Int_32>> keysReleasedThisFrame;

InputManager::InputManager()
{
    if(SDL_Init(SDL_INIT_EVENTTHREAD)==0 && SDL_Init(SDL_INIT_JOYSTICK)==0)
    {
    }
    else
        printf("Failed to setup input\n");

}

void InputManager::ProcessInput()
{
    keysPressedThisFrame.clear();
    keysReleasedThisFrame.clear();

    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            
        }
    }   
}

Bool_8 InputManager::WasWindowCrossed()
{
    return false;
}

Bool_8 InputManager::OnKeyDown(BGT_Key _key)
{
    return keysPressedThisFrame.find(_key) != keysPressedThisFrame.end();
}

Bool_8 InputManager::OnKeyUp(BGT_Key _key)
{
    return keysReleasedThisFrame.find(_key) != keysReleasedThisFrame.end();
}

Float_32 InputManager::GetKey(BGT_Key _key)
{
    return keyVal[_key];
}

InputManager::~InputManager()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTTHREAD);
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    
    printf("Input events backend cleaned\n");
    
}

