#ifndef ENGINE_HPP
#define ENGINE_HPP

#define MAX_IMAGES_LOADABLE 1024

#ifdef EXPORT_AS_DLL
    #define DllExport __declspec( dllexport )
#else
    #define DllExport
#endif

#include "GraphicsUtil.hpp"
#include "MathUtil.hpp"


struct DllExport BrewGameTool
{

    BrewGameTool(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode);

    Int_32 Run();
    void Quit();
    void SetFrameRate(Uint_32 fps);
    void SetVsyncMode(VsyncMode _mode);
    void SetInitFunc(void (*_init) ());
    void SetUpdateFunc(void (*_update) ());
    void SetCloseFunc(void (*_close) ());
    
    // key input functions
    Bool_8 OnKeyDown(BGT_Key _key);
    Bool_8 OnKeyUp(BGT_Key _key);
    Float_32 GetKey(BGT_Key _key);
    
    // image loading functions
    Uint_32 LoadImage(const char* _filename);
    Image* const GetImageById(Uint_32 _id);
    void DeleteImageById(Uint_32 _id);


    Uint_32 LoadMusic(const char* _filename);
    void PlayMusic(Uint_32 _music, Bool_8 doLoop);
    void PauseMusic();
    void ResumeMusic();
    Bool_8 IsPlayingMusic();
    void StopMusic();
    
    // image blitting function
    void BlitImage(const Image* const _image, Vec2f& _pos, Vec2f& _origin);
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
    

    void PrintBuffer();

    Uint_32 Width;
    Uint_32 Height;
    Uint_32 PixelSize;
    Double_64 DeltaTime;
    VsyncMode vsyncMode;

    private:
        static Image _imageDataStore[MAX_IMAGES_LOADABLE];
        static Uint_32 _nextId;
        Uint_32 _nextImagePosition = 0;
        Double_64 targetFrameTime = 0;
        Uint_32 targetFrameRate = 60;
        Bool_8 is_game_running;
        void (*init) ();
        void (*update) ();
        void (*close) ();

};



#endif // ENGINE_HPP