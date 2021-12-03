#ifndef ENGINE_HPP
#define ENGINE_HPP

#define MAX_IMAGES_LOADABLE 1024

#include "GraphicsUtil.hpp"
#include "MathUtil.hpp"

struct Canvas
{

    Canvas(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen);

    Int_32 Start();
    void SetInitFunc(void (*_init) ());
    void SetUpdateFunc(void (*_update) ());
    void SetCloseFunc(void (*_close) ());
    
    // image loading functions
    Uint_32 LoadImage(const char* _filename);
    Image* const GetImageById(Uint_32 _id);
    void DeleteImageById(Uint_32 _id);

    // image blitting function

    void BlitImage(const Image* const _image, Int_32 _x, Int_32 _y);
    void BlitImage(const Image* const _image, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale);

    
    // drawing funcitons
    void DrawPixel(Int_32 _x, Int_32 _y, Color color);
    void DrawCircle(Int_32 _x, Int_32 _y, Int_32 radius,Color color);
    void DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 radius, Color fillColor);
    void DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2,Color _color);
    void DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height,Color color);
    

    void PrintBuffer();

    Uint_32 Width;
    Uint_32 Height;
    Uint_32 PixelSize;
    Double_64 DeltaTime;
    private:
        static Image _imageDataStore[MAX_IMAGES_LOADABLE];
        static Uint_32 _nextId;
        Uint_32 _nextImagePosition = 0;
        void (*init) ();
        void (*update) ();
        void (*close) ();

};



#endif