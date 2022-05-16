#include <iostream>
#include <chrono>
// user headers
//#include "Backend.hpp"
#include "BrewGameTool.hpp"

#define MATH_UTIL_IMPLEMENTATION
#include "MathUtil.hpp"

#define GRAPHICS_UTILS_IMPLEMENTATION
#include "GraphicsUtil.hpp"

#define UTILS_IMPLEMENTATION
#include "Utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Backend_SDL2.cpp"

#ifdef main
#undef main
#endif

//Uint_32 canvasBufferSizeInBytes = 0;

//Uint_32 BrewGameTool::_nextId = 1;
//Image BrewGameTool::_imageDataStore[MAX_IMAGES_LOADABLE];

// Lifetime
BrewGameTool::BrewGameTool()
{
}

void BrewGameTool::SetupRenderer(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode)
{
    renderer = new Renderer(_name, _width, _height, _pixelSize, _setFullscreen, _mode);
}

void BrewGameTool::SetupAudio(Int_32 _frequency, Int_32 _channels, Int_32 _chunkSize)
{
    audioManager = new AudioManager(_frequency, _channels, _chunkSize);
}

void BrewGameTool::SetupInput()
{
    inputManager = new InputManager();
}


void BrewGameTool::Quit()
{
    is_game_running = false;
}

Int_32 BrewGameTool::Run()
{
    using namespace std::chrono;
    is_game_running = true;
    steady_clock::time_point _lastTime = steady_clock::now();

    //targetFrameTime = (Double_64)1000/targetFrameRate;
    Uint_32 frameCount = 0;
    Double_64 milSecondCounter = 0;

    init(); // app init
    while(is_game_running)
    {
        inputManager->ProcessInput();
        if(inputManager->WasWindowCrossed())
            is_game_running = false;

        renderer->Clear();

         // draw axis
        DrawLine(Width/2, Height/2, Width, Height/2,Color(0xff00ff00));
        DrawLine(Width/2, Height/2 , Width/2, Height, Color(0xff0000ff));

        update(); // app update
        renderer->Draw();
           
        duration<Double_64> time_span = duration_cast<duration<Double_64> >(steady_clock::now() - _lastTime);
        _lastTime = steady_clock::now();
        DeltaTime = time_span / std::chrono::milliseconds(1);   // DT in ms

        milSecondCounter += DeltaTime;
        frameCount++;

        if(milSecondCounter >= 1000.0)
        {
            
            //printf("FPS: %d\n", frameCount);
            char buffer[64];
            sprintf(buffer, "%s | %d x %d x %d | FPS: %d", renderer->windowTitle, Width, Height, PixelSize, frameCount);
            renderer->SetWindowTitle(buffer);
            milSecondCounter -= 1000.0;
            frameCount = 0;
        }

    }
    close();
    
    

    printf("All services cleaned up, quitting program.\n");
    return 0;
}

void BrewGameTool::SetInitFunc(void (*_init) ())
{
    this->init = _init;
}

void BrewGameTool::SetUpdateFunc(void (*_update) ())
{
    this->update = _update;
}

void BrewGameTool::SetCloseFunc(void (*_close) ())
{
    this->close = _close;
}

// Drawing
void BrewGameTool::DrawPixel(Float_32 _x, Float_32 _y, Color _color)
{
    renderer->DrawPixel(_x, _y, _color);
}

void BrewGameTool::DrawPixelAlphaBlended(Float_32 _x, Float_32 _y, Color _color)
{
    renderer->DrawPixel(_x, _y, _color);
}

void BrewGameTool::DrawLine(Int_32 _x1,  Int_32 _y1, Int_32 _x2, Int_32 _y2,Color _color)
{
   renderer->DrawLine(_x1,  _y1, _x2, _y2, _color);
}

void BrewGameTool::DrawCircle(Int_32 _x, Int_32 _y, Int_32 _radius,Color _color)
{
    renderer->DrawCircle(_x, _y, _radius, _color);
}

void BrewGameTool::DrawFilledCircle(Int_32 _x, Int_32 _y, Int_32 _radius, Color _fillColor)
{
    renderer->DrawFilledCircle(_x, _y, _radius, _fillColor);
}

void BrewGameTool::DrawRectangle(Int_32 _x, Int_32 _y, Int_32 _width, Int_32 _height, Color _color)
{
    renderer->DrawRectangle(_x, _y, _width, _height, _color);
}

Uint_32 BrewGameTool::LoadImage(const char* _filename)
{
    stbi_set_flip_vertically_on_load(true);
    Image _image;
    stbi_uc* _imageData = stbi_load(_filename, &_image.Width,&_image.Height, &_image.Channels, 4);
    _image.Diagonal = sqrtf(_image.Width*_image.Width + _image.Height*_image.Height);
    if(_imageData)
    {
        _nextId++;
        // bgra
        // change byte order so that [r][g][b][a] becomes [b][g][r][a]
        Uint_32 _img_data_size = _image.Width * _image.Height;
        for(int _i=0; _i<_img_data_size; _i++)
        {
            Utils_Swap_uc( &_imageData[_i*4], &_imageData[_i*4 + 2] );
        }
        _image.Data = (Uint_32*)_imageData;
        _imageDataStore[_nextId] = _image;
        return _nextId;
    }
    return 0;
}

Image* const BrewGameTool::GetImageById(Uint_32 _id)
{
    if(_imageDataStore.find(_id) != _imageDataStore.end())
    {
        return &_imageDataStore[_id];
    }

    return nullptr;
}

void BrewGameTool::DeleteImageById(Uint_32 _id)
{
    auto it = _imageDataStore.find(_id);
    if(it != _imageDataStore.end())
    {
        stbi_image_free(_imageDataStore[_id].Data);
        _imageDataStore.erase(it);
    }
}

void BrewGameTool::BlitImage(const Image* const _image, Vec2f& _pos, Vec2f& _origin)
{
    renderer->BlitImage(_image, _pos, _origin);
}

void BrewGameTool::BlitImage(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode)
{
   renderer->BlitImage(_image, _origin, _rot, _trans, _scale, _interpolationMode);

}

void BrewGameTool::BlitImage(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode)
{
   renderer->BlitImage(_image, _origin, _rot, _trans, _scale, brightness, _interpolationMode);
}

void BrewGameTool::BlitImageAlphaBlended(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode)
{
   renderer->BlitImageAlphaBlended(_image, _origin, _rot, _trans, _scale, _interpolationMode);
}

void BrewGameTool::BlitImageAlphaBlended(const Image* const _image, Vec2f& _origin, Mat3x3& _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode)
{
   renderer->BlitImageAlphaBlended(_image, _origin, _rot, _trans, _scale, brightness, _interpolationMode);
}

void BrewGameTool::SetVsyncMode(VsyncMode _mode)
{
    renderer->SetVsyncMode(_mode);
}

void BrewGameTool::SetFrameRate(Uint_32 _fps)
{
    renderer->SetFrameRate(_fps);
}
//  Drawing



// Input
Bool_8 BrewGameTool::OnKeyDown(BGT_Key _key)
{
    return inputManager->OnKeyDown(_key);
}

Bool_8 BrewGameTool::OnKeyUp(BGT_Key _key)
{
    return inputManager->OnKeyUp(_key);
}

Float_32 BrewGameTool::GetKey(BGT_Key _key)
{
    return inputManager->GetKey(_key);
}



// Music
Uint_32 BrewGameTool::LoadMusic(const char* _filename)
{
    return audioManager->LoadMusic(_filename);
}

void BrewGameTool::PlayMusic(Uint_32 _music, Bool_8 doLoop)
{
    audioManager->PlayMusic(_music, doLoop);
}

void BrewGameTool::PauseMusic()
{
    audioManager->PauseMusic();
}

void BrewGameTool::ResumeMusic()
{
    audioManager->ResumeMusic();
}

Bool_8 BrewGameTool::IsPlayingMusic()
{
    return audioManager->IsPlayingMusic();
}

void BrewGameTool::StopMusic()
{
    audioManager->StopMusic();
}


BrewGameTool::~BrewGameTool()
{
    // cleanup input, audio and renderer
    
    
    delete inputManager;
    delete audioManager;
    delete renderer;
    CleanupBackend();

    for(auto it : _imageDataStore)
    {
        stbi_image_free(it.second.Data);
    }

    printf("Deleted BGT instance.\n");

}