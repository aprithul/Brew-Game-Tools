#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "GraphicsUtil.hpp"
#include "Types.hpp"
#include "MathUtil.hpp"
#include <unordered_map>


    char windowTitle[512];

    void Renderer_Create(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode);
    void Renderer_Close();
    
    void Renderer_BlitImage(const Image* const _image, Vec2f _pos);
    void Renderer_BlitImageAlphaBlended(const Image* const _image, Vec2f _pos);
    void Renderer_BlitImage(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode);
    void Renderer_BlitImage(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 brightness, Interpolation _interpolationMode);
    void Renderer_BlitImageAlphaBlended(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Interpolation _interpolationMode);
    void Renderer_BlitImageAlphaBlended(const Image* const _image, const Mat3x3& _rot, Vec2f _trans, Vec2f _scale, Float_32 brightness, Interpolation _interpolationMode);


    // drawing funcitons
    void Renderer_DrawPixel1x(Int_32 _x, Int_32 _y, const Color& color);
    void Renderer_DrawPixel(Int_32 _x, Int_32 _y, const Color& color);
    void Renderer_DrawPixelAlphaBlended1x(Int_32 _x, Int_32 _y, const Color& color);
    void Renderer_DrawPixelAlphaBlended(Int_32 _x, Int_32 _y, const Color& color);
    void Renderer_DrawCircle(Int_32 _x, Int_32 _y, Int_32 radius,const Color& color);
    void Renderer_DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 radius, const Color& fillColor);
    void Renderer_DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2, const Color& _color);
    void Renderer_DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height,const Color& color);


    // Text rendering
    Uint_32 Renderer_LoadFont(const char* _filename, Uint_32 _size);
    void Renderer_DeleteFont(Uint_32 _font);
    void Renderer_GetTextBmp(const char* _text, Uint_32 _font);
    void Renderer_DrawText(const char* _text, Uint_32 _font, Int_32 _size, const Color& _col, Vec2f _location);
    void Renderer_DrawText(const char* _text, Uint_32 _font, Int_32 _size, const Color& _col, Vec2f _location, Float_32 _rot, Vec2f _scale);
    void Renderer_SetFontSize(Uint_32 _size);

    void Renderer_ClearFast(unsigned char grayBrightness);
    void Renderer_ClearSlow(const Color& _col);
    void Renderer_Draw();
    void Renderer_SetVsyncMode(VsyncMode _mode);
    void Renderer_SetFrameRate(Uint_32 _fps);
    void Renderer_SetWindowTitle(const char* _title);

    void Renderer_SetImageOrigin(Image* _img , Float_32 _x, Float_32 _y);
       
    Double_64 targetFrameTime = 0;
    Uint_32 targetFrameRate = 60;
    VsyncMode vsyncMode = VsyncMode::VSYNC_ON;
    Uint_32 Width;
    Uint_32 Height;
    Uint_32 PixelSize;
    void (*drawPixelFptr)(Int_32 _x, Int_32 _y, const Color& _col);
    void (*drawPixelAlphaBlendedFptr)(Int_32 _x, Int_32 _y, const Color& _col);


#endif // Renderer
