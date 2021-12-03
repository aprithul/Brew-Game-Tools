#include <iostream>
#include <chrono>
// user headers
#include "Backend.hpp"
#include "Canvas.hpp"

#define MATH_UTIL_IMPLEMENTATION
#include "MathUtil.hpp"


#define UTILS_IMPLEMENTATION
#include "Utils.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Bool_8 is_game_running;

#ifdef main
#undef main
#endif

Uint_32 canvasBufferSizeInBytes = 0;
const char* canvasTitle;

Uint_32 Canvas::_nextId = 1;
Image Canvas::_imageDataStore[MAX_IMAGES_LOADABLE];

Canvas::Canvas(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen) : 
    Width(_width), Height(_height), PixelSize(_pixelSize), DeltaTime(0)
{
    canvasTitle = _name;
    CreateWindow(_name, Width*PixelSize, Height*PixelSize, _setFullscreen);
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

void Canvas::DrawPixel(Int_32 _x, Int_32 _y, Color color)
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
            canvasBuffer[ (_rWidth * (_ry+_yOffset)) + (_rx + _xOffset) ] = color.Value;
        }   
    }

}

void Canvas::DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2,Color _color)
{
    if(_x1 == _x2)
    {
        if(_y2 < _y1)
        {
            Utils_Swap_i(&_x1, &_x2);
            Utils_Swap_i(&_y1, &_y2);
        }

        for(int _yi = _y1, _xi = _x1; _yi <= _y2; _yi++)
        {
            DrawPixel(_xi, _yi, _color);
        }
    }
    else if(_y1 == _y2)
    {
        if(_x2 < _x1)
        {
            Utils_Swap_i(&_x1, &_x2);
            Utils_Swap_i(&_y1, &_y2);
        }

        for(int _xi = _x1,_yi = _y1; _xi <= _x2; _xi++)
        {
            DrawPixel(_xi, _yi, _color);
        }
    }
    else
    {
        Float_32 _error = 0;
        Float_32 _m = (Float_32)(_y2 - _y1) / (_x2 - _x1);
        if(_m <= 1.f && _m >= -1.f)
        {
            if(_x2 < _x1)
            {
                Utils_Swap_i(&_x1, &_x2);
                Utils_Swap_i(&_y1, &_y2);
            }

            Int_32 _inc = 1;
            if(_m < 0 )
            {
                _inc = -1;
                _m = -_m;
            }

            for(Int_32 _xi = _x1, _yi = _y1; _xi <= _x2; _xi++)
            {
                DrawPixel(_xi, _yi, _color);

                _error += _m;
                if(_error >= 0.5f)
                {
                    _error -= 1.0f;
                    _yi+=_inc;
                }
            }
        }
        else
        {
            if(_y2 < _y1)
            {
                Utils_Swap_i(&_x1, &_x2);
                Utils_Swap_i(&_y1, &_y2);
            }
            _m = (Float_32)(_x1 - _x2) / (_y1 - _y2);

            Int_32 _inc = 1;
            if(_m < 0 )
            {
                _inc = -1;
                _m = -_m;
            }

            for(Int_32 _yi = _y1, _xi = _x1; _yi <= _y2; _yi++)
            {
                DrawPixel(_xi, _yi, _color);

                _error += _m;
                if(_error >= 0.5f)
                {
                    _error -= 1.0f;
                    _xi+=_inc;
                }
                
            }
        }
    }


}

Int_32 sqrd_dist(Int_32 x0, Int_32 y0, Int_32 x1, Int_32 y1)
{
    return ((x0 - x1)*(x0 - x1) + (y0-y1)*(y0-y1));
}

void Canvas::DrawCircle(Int_32 _x, Int_32 _y, Int_32 radius,Color color)
{
    Float_32 _radius_sq = radius * radius;
    for(Int_32 _xi = 0, _yi = radius; _xi <= _yi; _xi++)
    {
        DrawPixel(_x +_xi, _y + _yi, color);
        DrawPixel( _x + _yi, _y +_xi, color);

        DrawPixel(_x -_xi, _y + _yi, color);
        DrawPixel(_x + _yi,_y -_xi,  color);

        DrawPixel(_x +_xi, _y - _yi, color);
        DrawPixel(_x - _yi, _y +_xi, color);

        DrawPixel(_x -_xi, _y - _yi, color);
        DrawPixel(_x - _yi, _y -_xi, color);

        Int_32 _nxi = _xi + 1;
        Int_32 _nyi = _yi - 1;

        Int_32 dif0 = abs(sqrd_dist(0, 0, _nxi, _yi) - _radius_sq);
        Int_32 dif1 = abs(sqrd_dist(0, 0, _nxi, _nyi) - _radius_sq);
        
        if( dif1 < dif0 )
        {
            _yi = _nyi;
        }
    }
}



void Canvas::DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 radius, Color fillColor)
{
    Float_32 _radius_sq = radius * radius;
    for(Int_32 _xi = 0, _yi = radius; _xi <= _yi; _xi++)
    {
        DrawLine(_x +_xi, _y + _yi, _x -_xi, _y + _yi, fillColor);

        DrawLine(_x - _yi, _y + _xi, _x + _yi, _y + _xi, fillColor);

        DrawLine(_x +_xi, _y - _yi, _x -_xi, _y - _yi, fillColor);

        DrawLine(_x + _yi, _y -_xi,  _x - _yi, _y -_xi,  fillColor);


        Int_32 _nxi = _xi + 1;
        Int_32 _nyi = _yi - 1;

        Int_32 dif0 = abs(sqrd_dist(0, 0, _nxi, _yi) - _radius_sq);
        Int_32 dif1 = abs(sqrd_dist(0, 0, _nxi, _nyi) - _radius_sq);
        
        if( dif1 < dif0 )
        {
            _yi = _nyi;
        }
    }
}

void Canvas::DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height, Color _color)
{
    DrawLine(_x, _y, _x + _width, _y, _color);
    DrawLine(_x, _y, _x, _y+_height, _color);
    DrawLine(_x + _width, _y, _x + _width, _y + _height, _color);
    DrawLine(_x, _y + _height, _x + _width, _y + _height, _color);
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

Bool_8 Canvas::LoadImage(const char* _filename)
{
    Image& _image = _imageDataStore[_nextImagePosition];
    Uint_32* _imageData = (Uint_32*)stbi_load(_filename, &_image.Width,&_image.Height, &_image.Channels, 4);
    if(_imageData)
    {
        _image.Id = Canvas::_nextId++;
        _image.Data = _imageData;
        _nextImagePosition++;
        return true;
    }
    return false;
}


const Image* Canvas::GetImageById(Uint_32 _id)
{
    if(_id> 0)
    {
        for(int _i=0; _i<_nextImagePosition; _i++)
        {
            if(_imageDataStore[_i].Id == _id)
                return &_imageDataStore[_i];
        }
    }

}

void Canvas::DeleteImageById(Uint_32 _id)
{
    if(_id> 0 )
    {
        // find image with Id == _id
        int _i=0;
        for(; _i<_nextImagePosition; _i++)
        {
            if(_imageDataStore[_i].Id == _id)
                break;
        }

        if(_i == _nextImagePosition)    // _id not found
            return;

        // delete image at _i and swap in the last image into it's place
        stbi_image_free(_imageDataStore[_i].Data);
        _imageDataStore[_i].Id = 0;
        _imageDataStore[_i] = _imageDataStore[_nextImagePosition-1];
        _imageDataStore[_nextImagePosition-1].Id = 0;
        _nextImagePosition--;
    }

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
        memset(canvasBuffer, 0x00, canvasBufferSizeInBytes);

        update();
        DrawScreen();
        
        duration<Double_64> time_span = duration_cast<duration<Double_64> >(steady_clock::now() - _lastTime);
        _lastTime = steady_clock::now();

        DeltaTime = time_span / std::chrono::milliseconds(1);   // DT in ms

        milSecondCounter += DeltaTime;
        frameCount++;
        if(milSecondCounter >= 1000.0)
        {
            
            //printf("FPS: %d\n", frameCount);
            char buffer[64];
            sprintf(buffer, "%s | %d x %d x %d | FPS: %d", canvasTitle, Width, Height, PixelSize, frameCount);
            SetWindowTitle(buffer);
            milSecondCounter -= 1000.0;
            frameCount = 0;
        }

    }

    close();
    Cleanup();

    return 0;
}