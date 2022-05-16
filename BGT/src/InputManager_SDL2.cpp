#include "InputManager.hpp"
#include "SDL2/SDL.h"
#include <unordered_map>
#include <unordered_set>
#include <stdio.h>

std::unordered_map<SDL_Keycode, BGT_Key> sdlToBgtKeymap;
std::unordered_map<BGT_Key, Float_32, std::hash<Int_32>> keyVal;
std::unordered_set<BGT_Key, std::hash<Int_32>> keysPressedThisFrame;
std::unordered_set<BGT_Key, std::hash<Int_32>> keysReleasedThisFrame;
Bool_8 windowCrossed = false;

void mapSdlkToBgtk()
{
    sdlToBgtKeymap[SDLK_UP] = BGTK_UP;
    sdlToBgtKeymap[SDLK_DOWN] = BGTK_DOWN;
    sdlToBgtKeymap[SDLK_LEFT] = BGTK_LEFT;
    sdlToBgtKeymap[SDLK_RIGHT] = BGTK_RIGHT;

    sdlToBgtKeymap[SDLK_a] = BGTK_A;
    sdlToBgtKeymap[SDLK_b] = BGTK_B;
    sdlToBgtKeymap[SDLK_c] = BGTK_C;
    sdlToBgtKeymap[SDLK_d] = BGTK_D;
    sdlToBgtKeymap[SDLK_e] = BGTK_E;
    sdlToBgtKeymap[SDLK_f] = BGTK_F;
    sdlToBgtKeymap[SDLK_g] = BGTK_G;
    sdlToBgtKeymap[SDLK_h] = BGTK_H;
    sdlToBgtKeymap[SDLK_i] = BGTK_I;
    sdlToBgtKeymap[SDLK_j] = BGTK_J;
    sdlToBgtKeymap[SDLK_k] = BGTK_K;
    sdlToBgtKeymap[SDLK_l] = BGTK_L;
    sdlToBgtKeymap[SDLK_m] = BGTK_M;
    sdlToBgtKeymap[SDLK_n] = BGTK_N;
    sdlToBgtKeymap[SDLK_o] = BGTK_O;
    sdlToBgtKeymap[SDLK_p] = BGTK_P;
    sdlToBgtKeymap[SDLK_q] = BGTK_Q;
    sdlToBgtKeymap[SDLK_r] = BGTK_R;
    sdlToBgtKeymap[SDLK_s] = BGTK_S;
    sdlToBgtKeymap[SDLK_t] = BGTK_T;
    sdlToBgtKeymap[SDLK_u] = BGTK_U;
    sdlToBgtKeymap[SDLK_v] = BGTK_V;
    sdlToBgtKeymap[SDLK_w] = BGTK_W;
    sdlToBgtKeymap[SDLK_x] = BGTK_X;
    sdlToBgtKeymap[SDLK_y] = BGTK_Y;
    sdlToBgtKeymap[SDLK_z] = BGTK_Z;

    sdlToBgtKeymap[SDLK_0] = BGTK_0;
    sdlToBgtKeymap[SDLK_1] = BGTK_1;
    sdlToBgtKeymap[SDLK_2] = BGTK_2;
    sdlToBgtKeymap[SDLK_3] = BGTK_3;
    sdlToBgtKeymap[SDLK_4] = BGTK_4;
    sdlToBgtKeymap[SDLK_5] = BGTK_5;
    sdlToBgtKeymap[SDLK_6] = BGTK_6;
    sdlToBgtKeymap[SDLK_7] = BGTK_7;
    sdlToBgtKeymap[SDLK_8] = BGTK_8;
    sdlToBgtKeymap[SDLK_9] = BGTK_9;

    sdlToBgtKeymap[SDLK_LCTRL] = BGTK_LCTRL;
    sdlToBgtKeymap[SDLK_RCTRL] = BGTK_RCTRL;
    sdlToBgtKeymap[SDLK_LALT] = BGTK_LALT;
    sdlToBgtKeymap[SDLK_RALT] = BGTK_RALT;
    sdlToBgtKeymap[SDLK_CAPSLOCK] = BGTK_CAPSLOCK;
    sdlToBgtKeymap[SDLK_TAB] = BGTK_TAB;
    sdlToBgtKeymap[SDLK_RETURN] = BGTK_RETURN;
    sdlToBgtKeymap[SDLK_PAGEUP] = BGTK_PAGEUP;
    sdlToBgtKeymap[SDLK_PAGEDOWN] = BGTK_PAGEDOWN;
    sdlToBgtKeymap[SDLK_HOME] = BGTK_HOME;
    sdlToBgtKeymap[SDLK_END] = BGTK_END;
    sdlToBgtKeymap[SDLK_DELETE] = BGTK_DELETE;
    
    sdlToBgtKeymap[SDLK_ESCAPE] = BGTK_ESCAPE;
    sdlToBgtKeymap[SDLK_SPACE] = BGTK_SPACE;
    sdlToBgtKeymap[SDLK_BACKSPACE] = BGTK_BACKSPACE;
    sdlToBgtKeymap[SDLK_LEFTBRACKET] = BGTK_LBRACKET;
    sdlToBgtKeymap[SDLK_RIGHTBRACKET] = BGTK_RBRACKET;
    sdlToBgtKeymap[SDLK_KP_LEFTBRACE] = BGTK_LBRACE;
    sdlToBgtKeymap[SDLK_RIGHTBRACKET] = BGTK_RBRACE;
    sdlToBgtKeymap[SDLK_LEFTPAREN] = BGTK_LPARENTHESES;
    sdlToBgtKeymap[SDLK_RIGHTPAREN] = BGTK_RPARENTHESES;
    sdlToBgtKeymap[SDLK_SEMICOLON] = BGTK_SEMICOLON;
    sdlToBgtKeymap[SDLK_COMMA] = BGTK_COMMA;
    sdlToBgtKeymap[SDLK_PERIOD] = BGTK_PERIOD;
    sdlToBgtKeymap[SDLK_SLASH] = BGTK_FORWARDSLASH;
    sdlToBgtKeymap[SDLK_BACKSLASH] = BGTK_BACKWARDSLASH;
    sdlToBgtKeymap[SDLK_MINUS] = BGTK_MINUS;
    sdlToBgtKeymap[SDLK_PLUS] = BGTK_PLUS;
    sdlToBgtKeymap[SDLK_BACKQUOTE] = BGTK_BACKTICK,
    sdlToBgtKeymap[SDLK_QUOTE] = BGTK_QUOTATION,
    
    sdlToBgtKeymap[SDLK_F1] = BGTK_F1;
    sdlToBgtKeymap[SDLK_F2] = BGTK_F2;
    sdlToBgtKeymap[SDLK_F3] = BGTK_F3;
    sdlToBgtKeymap[SDLK_F4] = BGTK_F4;
    sdlToBgtKeymap[SDLK_F5] = BGTK_F5;
    sdlToBgtKeymap[SDLK_F6] = BGTK_F6;
    sdlToBgtKeymap[SDLK_F7] = BGTK_F7;
    sdlToBgtKeymap[SDLK_F8] = BGTK_F8;
    sdlToBgtKeymap[SDLK_F9] = BGTK_F9;
    sdlToBgtKeymap[SDLK_F10] = BGTK_F10;
    sdlToBgtKeymap[SDLK_F11] = BGTK_F11;
    sdlToBgtKeymap[SDLK_F12] = BGTK_F12;
}

InputManager::InputManager()
{
    if(SDL_Init(SDL_INIT_EVENTS)==0)
    {
        mapSdlkToBgtk();
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
            case SDL_QUIT:
            {
                windowCrossed = true;
            }
            break;
            case SDL_KEYDOWN:
            {
                BGT_Key bgtk = sdlToBgtKeymap[event.key.keysym.sym];
                if(keyVal[bgtk] < 0.5f)
                    keysPressedThisFrame.insert(bgtk);
                keyVal[bgtk] = 1.f;
            }
            break;
            case SDL_KEYUP:
            {
                BGT_Key bgtk = sdlToBgtKeymap[event.key.keysym.sym];
                if(keyVal[bgtk] > 0.5f)
                    keysReleasedThisFrame.insert(bgtk);
                keyVal[bgtk] = 0.f;
            }
            break;
            default:
            break;
        }
    }   
}

Bool_8 InputManager::WasWindowCrossed()
{
    return windowCrossed;
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
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    printf("Input events backend cleaned");
    
}

