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
BrewGameTool::BrewGameTool():DeltaTime(0),nextId(0)
{
    printf("BGT instance created.\n");
}

void BrewGameTool::SetupRenderer(const char* _name, Uint_32 _width, Uint_32 _height, Uint_32 _pixelSize, Bool_8 _setFullscreen, VsyncMode _mode)
{
    Width = _width;
    Height = _height;
    PixelSize = _pixelSize;

    renderer = new Renderer(_name, _width, _height, _pixelSize, _setFullscreen, _mode);
    printf("Renderer created.\n");
    
}

void BrewGameTool::SetupAudio(Int_32 _frequency, Int_32 _channels, Int_32 _chunkSize)
{
    audioManager = new AudioManager(_frequency, _channels, _chunkSize);
    printf("Audio manager created.\n");

}

void BrewGameTool::SetupInput()
{
    inputManager = new InputManager();
    printf("Input manager created.\n");

}


void BrewGameTool::Quit()
{
    isGameRunning = false;
}

void BrewGameTool::SetNextFrameClearColor(Color _color)
{
    clearColor = _color;
}

Int_32 BrewGameTool::Run()
{
    using namespace std::chrono;

    printf("App started\n");
    //targetFrameTime = (Double_64)1000/targetFrameRate;
    Uint_32 frameCount = 0;
    Double_64 secondCounter = 0;

    init(); // app init
    isGameRunning = true;

    steady_clock::time_point _lastTime = steady_clock::now();
    while(isGameRunning)
    {
        inputManager->ProcessInput();
        if(inputManager->WasWindowCrossed())
            isGameRunning = false;

        //renderer->ClearSlow(clearColor);
        renderer->ClearFast(0x80);
        update(); // app update
        renderer->Draw();
           
        duration<Double_64> time_span = duration_cast<duration<Double_64> >(steady_clock::now() - _lastTime);
        _lastTime = steady_clock::now();
        DeltaTime = time_span / std::chrono::milliseconds(1) / 1000;   // DT in seconds

        secondCounter += DeltaTime;
        frameCount++;

        if(secondCounter >= 1)
        {
            
            //printf("FPS: %d\n", frameCount);
            char buffer[64];
            sprintf(buffer, "%s | %d x %d x %d | FPS: %d", renderer->windowTitle, Width, Height, PixelSize, frameCount);
            renderer->SetWindowTitle(buffer);
            secondCounter -= 1.0;
            frameCount = 0;
        }

    }
    close();

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
    _image.Origin_x = _image.Width/2.f;
    _image.Origin_y = _image.Height/2.f;

    if(_imageData)
    {
        nextId++;
        // bgra
        // change byte order so that [r][g][b][a] becomes [b][g][r][a]
        Uint_32 _img_data_size = _image.Width * _image.Height;
        for(int _i=0; _i<_img_data_size; _i++)
        {
            Swap( &_imageData[_i*4], &_imageData[_i*4 + 2] );
        }
        _image.Data = (Uint_32*)_imageData;
        _imageDataStore[nextId] = _image;
        return nextId;
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

void BrewGameTool::SetImageOrigin(Uint_32 _id, Float_32 _x, Float_32 _y)
{
    Image* img = GetImageById(_id);
    if(img)
    {
        img->Origin_x = _x;
        img->Origin_y = _y;
    }
}


void BrewGameTool::DrawImage(Uint_32 _imageId, Vec2f& _pos, Vec2f& _origin)
{
    Image* _image = GetImageById(_imageId);
    renderer->BlitImage(_image, _pos, _origin);
}

void BrewGameTool::DrawImage(Uint_32 _imageId, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode)
{
    Image* _image = GetImageById(_imageId);
    Mat3x3 rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(_rot);
    rotMat(0,1) = -sinf(_rot);
    rotMat(1,0) = sinf(_rot);
    rotMat(1,1) = cosf(_rot);

    renderer->BlitImage(_image, {_image->Origin_x, _image->Origin_y}, rotMat, _trans, _scale, _interpolationMode);

}

void BrewGameTool::DrawImage(Uint_32 _imageId, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode)
{
    Image* _image = GetImageById(_imageId);
    Mat3x3 rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(_rot);
    rotMat(0,1) = -sinf(_rot);
    rotMat(1,0) = sinf(_rot);
    rotMat(1,1) = cosf(_rot);
   renderer->BlitImage(_image, {_image->Origin_x, _image->Origin_y}, rotMat, _trans, _scale, brightness, _interpolationMode);
}

void BrewGameTool::DrawImageAlphaBlended(Uint_32 _imageId, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Interpolation _interpolationMode)
{

    Image* _image = GetImageById(_imageId);
    Mat3x3 rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(_rot);
    rotMat(0,1) = -sinf(_rot);
    rotMat(1,0) = sinf(_rot);
    rotMat(1,1) = cosf(_rot);
    renderer->BlitImageAlphaBlended(_image, {_image->Origin_x, _image->Origin_y}, rotMat, _trans, _scale, _interpolationMode);
}

void BrewGameTool::DrawImageAlphaBlended(Uint_32 _imageId, Float_32 _rot, Vec2f& _trans, Vec2f& _scale, Float_32 brightness, Interpolation _interpolationMode)
{
    Image* _image = GetImageById(_imageId);
    Mat3x3 rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(_rot);
    rotMat(0,1) = -sinf(_rot);
    rotMat(1,0) = sinf(_rot);
    rotMat(1,1) = cosf(_rot);
    renderer->BlitImageAlphaBlended(_image, {_image->Origin_x, _image->Origin_y}, rotMat, _trans, _scale, brightness, _interpolationMode);
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

Uint_32 BrewGameTool::LoadFont(const char* _filename)
{
    return renderer->LoadFont(_filename);
}

void BrewGameTool::DeleteFont(Uint_32 _font)
{
    renderer->DeleteFont(_font);
}

void BrewGameTool::DrawText(const char* _text, Uint_32 _font, Int_32 _size, Color _col, Vec2f _location)
{
    renderer->DrawText(_text, _font, _size, _col, _location);
}

void BrewGameTool::DrawText(const char* _text, Uint_32 _font, Int_32 _size, Color _col, Vec2f _location, Float_32 _rot, Vec2f _scale)
{
    renderer->DrawText(_text, _font, _size, _col, _location, _rot, _scale);
}

void BrewGameTool::SetFontSize(Uint_32 _size)
{
    renderer->SetFontSize(_size);
}




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


void BrewGameTool::SetMusicVolume(Float_32 _volume)
{
    audioManager->SetMusicVolume(_volume);
}

Uint_32 BrewGameTool::LoadSoundEffect(const char* _filename)
{
    return audioManager->LoadSoundEffect(_filename);
}

void BrewGameTool::PlaySoundEffect(Uint_32 _effect)
{
    audioManager->PlaySoundEffect(_effect);
}

void BrewGameTool::SetSoundEffectVolume(Float_32 _volume)
{
    audioManager->SetSoundEffectVolume(_volume);
}

void BrewGameTool::SetMasterVolume(Float_32 _volume)
{
    audioManager->SetMusicVolume(_volume);
    audioManager->SetSoundEffectVolume(_volume);
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