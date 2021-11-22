#ifndef ENGINE_HPP
#define ENGINE_HPP


#include "MathUtil.hpp"
#include "GraphicsUtil.hpp"
struct Canvas
{

    Canvas(const char* _name, Int_32 _width, Int_32 _height);

    Int_32 Start();
    void SetInitFunc(void (*_init) ());
    void SetUpdateFunc(void (*_update) ());
    void SetCloseFunc(void (*_close) ());
    void DrawPixel(int _x, int _y, Color color);

    Uint_32 Width;
    Uint_32 Height;

    private:
        void (*init) ();
        void (*update) ();
        void (*close) ();

};



#endif