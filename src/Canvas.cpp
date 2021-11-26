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
    steady_clock::time_point _lastTime, _newTime;
    _lastTime = steady_clock::now();
    _newTime = _lastTime;

    Double_64 accumulator = 0;
    Double_64 targetFrameTime = (Double_64)1000/60;
    Uint_32 frameCount = 0;
    Double_64 secondCounter = 0;

    while(is_game_running)
    {
        ProcessInput();

        if(WasWindowCrossed())
            is_game_running = false;

        // clear canvasBuffer
        memset(canvasBuffer, 0, canvasBufferSizeInBytes);

        update();
        DrawScreen();
        
        _lastTime = _newTime;
        _newTime = steady_clock::now();
        duration<Double_64> time_span = duration_cast<duration<Double_64>>(_newTime - _lastTime);
        DeltaTime = time_span.count() * 1000;
        accumulator = (targetFrameTime - DeltaTime);

        secondCounter += (Double_64)DeltaTime/1000;
        frameCount++;
        if(secondCounter >= 1.0)
        {
            printf("FPS: %d\n", frameCount);
            secondCounter = 0;
            frameCount = 0;
        }

        
        //Delay(accumulator);
        // //printf("%f %f\n", DeltaTime, accumulator);
        // if(accumulator > 0)
        // {
        //     steady_clock::time_point _beforeWait = steady_clock::now();
        //     Delay(accumulator);
        //     duration<Double_64> _waitDuration = duration_cast<duration<Double_64>>(steady_clock::now() - _beforeWait);
        //     //printf("%f\n", _waitDuration);
        //     DeltaTime += _waitDuration.count()*1000;
        // }
        //if(accumulator > 0)
        //printf("Accum: %f\n", accumulator);

        // steady_clock::time_point _dts = steady_clock::now();
        // while(accumulator > 0)
        // {
        //     duration<Double_64> _dt = duration_cast<duration<Double_64>>(steady_clock::now() - _dts);
        //     //printf("%f\n", dt);
        //     _dts = steady_clock::now();
        //     accumulator -= _dt.count()*1000;
        //     DeltaTime += _dt.count()*1000;
        //     Delay(1);
        // }

    }

    close();
    Cleanup();

    return 0;
}