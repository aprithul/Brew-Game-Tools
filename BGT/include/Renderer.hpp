#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "GraphicsUtil.hpp"
#include "Types.hpp"
#include "MathUtil.hpp"
#include <unordered_map>

class Renderer
{

public:
    char windowTitle[512];

    Renderer(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode);
    ~Renderer();
    
    void BlitImage(const Image* const _image, Vec2f& _pos, Vec2f& _origin);
    void BlitImageAlphaBlended(const Image* const _image, Vec2f& _pos, Vec2f& _origin);
    void BlitImage(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode);
    void BlitImage(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode);
    void BlitImageAlphaBlended(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode);
    void BlitImageAlphaBlended(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode);


    // drawing funcitons
    void DrawPixel(Float_32 _x, Float_32 _y, Color color);
    void DrawPixelAlphaBlended(Float_32 _x, Float_32 _y, Color color);
    void DrawCircle(Int_32 _x, Int_32 _y, Int_32 radius,Color color);
    void DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 radius, Color fillColor);
    void DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2,Color _color);
    void DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height,Color color);


    // Text rendering
    Uint_32 LoadFont(const char* _filename);
    void DeleteFont(Uint_32 _font);
    void GetTextBmp(const char* _text, Uint_32 _font);
    void DrawText(const char* _text, Uint_32 _font, Vec2f _location);
    void SetFontSize(Uint_32 _size);

    void Clear();
    void Draw();
    void SetVsyncMode(VsyncMode _mode);
    void SetFrameRate(Uint_32 _fps);
    void SetWindowTitle(const char* _title);


private:
       
        Double_64 targetFrameTime = 0;
        Uint_32 targetFrameRate = 60;
        VsyncMode vsyncMode = VsyncMode::VSYNC_ON;
        Uint_32 Width;
        Uint_32 Height;
        Uint_32 PixelSize;
};


#endif // Renderer