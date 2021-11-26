#ifndef ENGINE_HPP
#define ENGINE_HPP


#include "MathUtil.hpp"
#include "GraphicsUtil.hpp"
struct Canvas
{

    Canvas(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize);

    Int_32 Start();
    void SetInitFunc(void (*_init) ());
    void SetUpdateFunc(void (*_update) ());
    void SetCloseFunc(void (*_close) ());
    void DrawPixel(int _x, int _y, Color color);
    void PrintBuffer();

    Uint_32 Width;
    Uint_32 Height;
    Uint_32 PixelSize;
    Double_64 DeltaTime;
    private:
        void (*init) ();
        void (*update) ();
        void (*close) ();

};



#endif