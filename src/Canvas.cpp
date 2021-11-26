#include <iostream>
#include <chrono>
// user headers
#include "Backend.hpp"
#include "Canvas.hpp"
#define MATH_UTIL_IMPLEMENTATION



Bool_8 is_game_running;

#ifdef main
#undef main
#endif

Uint_32 canvasBufferSizeInBytes = 0;
Canvas::Canvas(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize) : 
    Width(_width), Height(_height), PixelSize(_pixelSize), DeltaTime(0)
{
    CreateWindow(_name, Width*PixelSize, Height*PixelSize);
    canvasBufferSizeInBytes = Width*PixelSize*Height*PixelSize*sizeof(Uint_32);
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

void Canvas::DrawPixel(int _x, int _y, Color color)
{
    Int_32 _rx = _x * PixelSize;
    Int_32 _ry = _y * PixelSize;
    Int_32 _rWidth = Width * PixelSize;
    Int_32 _rHeight = Height * PixelSize;

    if(_rx < 0 || _ry < 0 || _rx >= _rWidth || _ry >= _rHeight)
        return;

    for(Uint_32 _xOffset = 0; _xOffset < PixelSize; _xOffset++)
    {
        for(Uint_32 _yOffset = 0; _yOffset < PixelSize; _yOffset++)
        {
            canvasBuffer[ (_rWidth * (_ry+_yOffset)) + (_rx + _xOffset) ] = color.value;
        }
    }

}

void Canvas::PrintBuffer()
{    
    Int_32 _rWidth = Width * PixelSize;
    Int_32 _rHeight = Height * PixelSize;

    printf("--Buffer--\n");

    for(Int_32 _y = 0; _y < _rHeight; _y++)
    {
        for(Int_32 _x = 0; _x < _rWidth; _x++)
        {
            printf("%X - ", canvasBuffer[_rWidth*_y + _x]);
        }
        printf("\n");
    }

    printf("--Printed buffer--\n");

}


Int_32 Canvas::Start()
{
    using namespace std::chrono;
    init();
    is_game_running = true;
    steady_clock::time_point _lastTime = steady_clock::now();

    Double_64 targetFrameTime = (Double_64)1000/60;
    Uint_32 frameCount = 0;
    Double_64 milSecondCounter = 0;

    while(is_game_running)
    {
        ProcessInput();

        if(WasWindowCrossed())
            is_game_running = false;

        // clear canvasBuffer
        memset(canvasBuffer, 0, canvasBufferSizeInBytes);

        update();
        DrawScreen();
        
        duration<Double_64> time_span = duration_cast<duration<Double_64>>(steady_clock::now() - _lastTime);
        _lastTime = steady_clock::now();

        DeltaTime = time_span / std::chrono::milliseconds(1);   // DT in ms

        milSecondCounter += DeltaTime;
        frameCount++;
        if(milSecondCounter >= 1000.0)
        {
            printf("FPS: %d\n", frameCount);
            milSecondCounter -= 1000.0;
            frameCount = 0;
        }

    }

    close();
    Cleanup();

    return 0;
}