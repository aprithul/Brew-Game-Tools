#ifndef ENGINE_HPP
#define ENGINE_HPP

#define MAX_IMAGES_LOADABLE 1024

#ifdef EXPORT_AS_DLL
    #define DllExport __declspec( dllexport )
#else
    #define DllExport
#endif

#include "Defines.hpp"
#include "Renderer.hpp"
#include "AudioManager.hpp"
#include "InputManager.hpp"
#include "GraphicsUtil.hpp"
#include "MathUtil.hpp"
#include <unordered_map>

struct DllExport BrewGameTool
{

    BrewGameTool();
    ~BrewGameTool();

    void SetupRenderer(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode);
    void SetupAudio(Int_32 _frequency, Int_32 _channels, Int_32 _chunkSize);
    void SetupInput();
    void SetNextFrameClearColor(Color _col);

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
    Uint_32 LoadSoundEffect(const char* _filename);
    
    void PlayMusic(Uint_32 _music, Bool_8 doLoop);
    void PauseMusic();
    void ResumeMusic();
    Bool_8 IsPlayingMusic();
    void StopMusic();
    void SetMusicVolume(Float_32 _volume);

    void PlaySoundEffect(Uint_32 _effect);
    void SetSoundEffectVolume(Float_32 _volume);
    
    void SetMasterVolume(Float_32 _volume);

    // image blitting function
    void DrawImage(Uint_32 _imageId, Vec2f& _pos, Vec2f& _origin);
    void DrawImage(Uint_32 _imageId, Vec2f& _origin, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode);
    void DrawImage(Uint_32 _imageId, Vec2f& _origin, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode);
    void DrawImageAlphaBlended(Uint_32 _imageId, Vec2f& _origin, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode);
    void DrawImageAlphaBlended(Uint_32 _imageId, Vec2f& _origin, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode);
    
    
    // drawing funcitons
    void DrawPixel(Float_32 _x, Float_32 _y, Color color);
    void DrawPixelAlphaBlended(Float_32 _x, Float_32 _y, Color color);
    void DrawCircle(Int_32 _x, Int_32 _y, Int_32 radius,Color color);
    void DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 radius, Color fillColor);
    void DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2,Color _color);
    void DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height,Color color);

    // text drawing
    Uint_32 LoadFont(const char* _filename);
    void DeleteFont(Uint_32 _font);
    void DrawText(const char* _text, Uint_32 _font, Int_32 _size, Color _col, Vec2f _location);
    void DrawText(const char* _text, Uint_32 _font, Int_32 _size, Color _col, Vec2f _location, Float_32 _rot, Vec2f _scale);
    void SetFontSize(Uint_32 _size);

    Uint_32 Width;
    Uint_32 Height;
    Uint_32 PixelSize;
    Double_64 DeltaTime;
    VsyncMode vsyncMode;

    private:
        //static Image _imageDataStore[MAX_IMAGES_LOADABLE];
        Uint_32 nextId;
        Color clearColor;
        std::unordered_map<Uint_32, Image> _imageDataStore;
        Bool_8 isGameRunning;
        void (*init) ();
        void (*update) ();
        void (*close) ();
        Renderer* renderer;
        AudioManager* audioManager;
        InputManager* inputManager;


};



#endif // ENGINE_HPP