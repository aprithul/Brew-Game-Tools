#include "Backend.hpp"
#include "SDL2/SDL.h"


#if defined(AUDIO_MANAGER_SDL2_MIXER) || defined(_DEBUG)
    #include "AudioManager_SDL2_Mixer.cpp"
#endif  

#if defined(INPUT_Manager_SDL2) || defined(_DEBUG)
    #include "InputManager_SDL2.cpp"
#endif  

#if defined(RENDERER_SW) || defined(_DEBUG)
    #include "RendererSW.cpp"
#endif


void CleanupBackend()
{
    SDL_Quit();
}

