#include <iostream>
#include "SDL2/SDL.h"
#include "GL/glew.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

// user headers
#include "Graphics.hpp"
#include "Engine.hpp"

#define MATH_UTIL_IMPLEMENTATION

SDL_Window* window;
SDL_GLContext gl_context;

Bool_8 is_game_running;

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

#ifdef main
#undef main
#endif


void Canvas::SetInitFunc(void (*_init) ())
{
    this->init = _init;
}


void Canvas::SetUpdateFunc(void (*_update) ())
{
    this->update = _update;
}


void Canvas::SetCloseFunc(void (*_close) ())
{
    this->close = _close;
}

void Canvas::DrawPixel(Vec2f position)
{

}

Int_32 Canvas::Start()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        std::cout<<"SDL2 initialized successfully."<<std::endl;
        set_sdl_gl_attributes();
        
        window = SDL_CreateWindow("Game", 0,0, 640, 480, SDL_WINDOW_OPENGL);
        gl_context = SDL_GL_CreateContext(window);


        if(glewInit() == GLEW_OK)
        {
            std::cout<<"GLEW initialization successful"<<'\n';
            std::cout<<glGetString(GL_VERSION)<<'\n';
        }
        else
        {
            std::cout<<"GLEW initialization failed"<<std::endl;
            return -1;
        }

        assert(init);
        init();
        SDL_Event event;
        is_game_running = true;

        while(is_game_running)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    is_game_running = false;
                }
            }   

            assert(init);
            update();

            glClearColor( 0,0,0,1);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            SDL_GL_SwapWindow(window);
        }
    
        close();
        SDL_Quit();
    }
    else
    {
        std::cout<<"SDL2 initialization failed."<<std::endl;
        return -1;
    }

    return 0;
}