#include "Backend.hpp"
#include "SDL2/SDL.h"

// include backend to use
#if defined(RENDERING_BACKEND_SDL2_OPENGL) || defined(DEBUG)
    #include "RenderingBackend_SDL2_OpenGL.cpp"
#endif

#if defined(AUDIO_BACKEND_SDL2_MIXER) || defined(DEBUG)
    #include "AudioBackend_SDL2_Mixer.cpp"
#endif  

#if defined(INPUT_BACKEND_SDL2) || defined(DEBUG)
    #include "InputBackend_SDL2.cpp"
#endif  

void CleanupBackend()
{
    SDL_Quit();
}

 


