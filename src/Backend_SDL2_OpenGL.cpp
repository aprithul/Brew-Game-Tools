#include "Config.hpp"

#ifdef ENABLE_BACKEND_SDL2_OPENGL

#include "Backend.hpp"
#include "SDL2/SDL.h"
#include "GL/glew.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

SDL_Window* window;
SDL_GLContext gl_context;

void set_sdl_gl_attributes()
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
}


void CreateWindow(const char* _name, Int_32 _width, Int_32 _height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        set_sdl_gl_attributes();
        window = SDL_CreateWindow(_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);
        gl_context = SDL_GL_CreateContext(window);

        if(glewInit() == GLEW_OK)
        {
            printf("Canvas created with OpenGL context : %s", (const char*)glGetString(GL_VERSION));
        }
        else
        {
            printf("GLEW initialization failed");
        }
    }
    else
    {
        printf("SDL initialization failed\n");
    }
}

Bool_8 windowCrossed = false;
Bool_8 WasWindowCrossed()
{
    return windowCrossed;
}


void ProcessInput()
{
    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            windowCrossed = true;
        }
    }   
}


void Draw()
{
    glClearColor( 0,0,0,1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
}


void Cleanup()
{
    SDL_Quit();
}

#endif