#ifndef ENGINE_HPP
#define ENGINE_HPP

#define DRAW_GL_API

#include "MathUtil.hpp"

struct Canvas
{

    DRAW_GL_API int Start();
    DRAW_GL_API void SetInitFunc(void (*_init) ());
    DRAW_GL_API void SetUpdateFunc(void (*_update) ());
    DRAW_GL_API void SetCloseFunc(void (*_close) ());


    // drawing API
    DRAW_GL_API void DrawPixel(Vec2f position);

    private:
        void (*init) ();
        void (*update) ();
        void (*close) ();

};



#endif