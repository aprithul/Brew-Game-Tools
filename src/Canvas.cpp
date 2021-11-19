#include <iostream>

// user headers
#include "Backend.hpp"
#include "Canvas.hpp"

#define MATH_UTIL_IMPLEMENTATION



Bool_8 is_game_running;

#ifdef main
#undef main
#endif

Canvas::Canvas(const char* _name, Int_32 _width, Int_32 _height)
{
    CreateWindow(_name, _width, _height);
}


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

Int_32 Canvas::Start()
{
    init();
    is_game_running = true;

    while(is_game_running)
    {
        ProcessInput();

        if(WasWindowCrossed())
            is_game_running = false;

        update();
        Draw();
    }

    close();
    Cleanup();

    return 0;
}