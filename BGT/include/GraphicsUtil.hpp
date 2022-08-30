#ifndef GRAPHICS_UTIL_HPP
#define GRAPHICS_UTIL_HPP

#include "Types.hpp"
#include <vector>

enum VsyncMode
{
    VSYNC_OFF = 0,
    VSYNC_ON = 1,
    VSYNC_ADAPTIVE = -1
};


enum ColorFormat
{
    RGBA,
    BGRA,
    ARGB
};

enum Interpolation
{
    INTERPOLATION_NEAREST,
    INTERPOLATION_LINEAR
};

struct Color
{
    Color();
    Color(Byte_8 r, Byte_8 g, Byte_8 b, Byte_8 a, ColorFormat _formatInMemory);
    Color(Uint_32 argb);
    void RetrieveComponenets(Byte_8* r, Byte_8* g, Byte_8* b, Byte_8* a);
    Uint_32 Value;
    ColorFormat formatInMemory;

    static Color interpolate(const Color& a, const Color& b, Float_32 t);
    static Color alphaBlend(const Color& bg, const Color& ov);
};


struct Image // 200 bytes
{
    Uint_32* Data;
    Int_32 Width;
    Int_32 Height;
    Int_32 Diagonal;
    Int_32 Channels;
    Float_32 Origin_x;
    Float_32 Origin_y;
};


struct Animation
{
    Animation();
    void AddFrame(Uint_32 _frame);
    Int_32 GetCurrentFrame();
    void Play();
    void Pause();
    void Update(Double_64 _deltaTime);
    void SetSpeed(Double_64 _frameDelta);
    Bool_8 IsPlaying();

    private:
        std::vector<Uint_32> frames;
        Double_64 nextFrameDelta;
        Bool_8 isPlaying;
        Double_64 timeAccum;
        Int_32 currentFrameInd;

};

#endif

#ifdef GRAPHICS_UTILS_IMPLEMENTATION
#undef GRAPHICS_UTILS_IMPLEMENTATION

Animation::Animation():nextFrameDelta(INT_MAX), isPlaying(false), timeAccum(0), currentFrameInd(0)
{

}

void Animation::SetSpeed(Double_64 _frameDelta)
{
    nextFrameDelta = _frameDelta;
}

void Animation::AddFrame(Uint_32 _frame)
{
    frames.push_back(_frame);
}

void Animation::Play()
{
    isPlaying = true;
}

void Animation::Pause()
{
    isPlaying = false;
}

void Animation::Update(Double_64 _deltaTime)
{
    if(isPlaying)
    {
        timeAccum += _deltaTime;
        if(timeAccum > nextFrameDelta)
        {
            timeAccum -= nextFrameDelta;
            currentFrameInd = (currentFrameInd+1)%frames.size();
        }
    }
}

Int_32 Animation::GetCurrentFrame()
{
    return frames[currentFrameInd];
}

Bool_8 Animation::IsPlaying()
{
    return isPlaying;
}

Color::Color()
{
    formatInMemory = ColorFormat::ARGB;
    Value = 0x00000000;
}

Color::Color(Byte_8 r, Byte_8 g, Byte_8 b, Byte_8 a, ColorFormat _formatInMemory)
{
    formatInMemory = _formatInMemory;
    switch (_formatInMemory)
    {
        case RGBA:
            Value = (((Uint_32)a)<<24  & 0xff000000)|
                    (((Uint_32)b)<<16  & 0x00ff0000)|
                    (((Uint_32)g)<< 8  & 0x0000ff00)|
                    (((Uint_32)r)      & 0x000000ff);
            break;
        case BGRA:
            Value = (((Uint_32)a)<<24  & 0xff000000)|
                    (((Uint_32)r)<<16  & 0x00ff0000)|
                    (((Uint_32)g)<< 8  & 0x0000ff00)|
                    (((Uint_32)b)      & 0x000000ff);
            break;
        case ARGB:
            Value = (((Uint_32)b)<<24  & 0xff000000)|
                    (((Uint_32)g)<<16  & 0x00ff0000)|
                    (((Uint_32)r)<< 8  & 0x0000ff00)|
                    (((Uint_32)a)      & 0x000000ff);
            break;
    }
}

Color::Color( Uint_32 argb)
{
    formatInMemory = ColorFormat::ARGB;
    Value = argb;
}


static Uint_32 lerp(Uint_32 valA, Uint_32 valB, Float_32 t)
{
    return (Uint_32)(valA*(1.f-t) + valB*t);
}



Color Color::interpolate(const Color& a, const Color& b, Float_32 t)
{
    static const Uint_32 _maskA = 0xff000000;
    static const Uint_32 _maskR = 0x00ff0000;
    static const Uint_32 _maskG = 0x0000ff00;
    static const Uint_32 _maskB = 0x000000ff;

    Uint_32 interpolated = ((lerp( (_maskA & a.Value)>>24,  (_maskA & b.Value)>>24, t))<<24);
    interpolated |=        ((lerp( (_maskR & a.Value)>>16,  (_maskR & b.Value)>>16, t))<<16);
    interpolated |=        ((lerp( (_maskG & a.Value)>>8,   (_maskG & b.Value)>>8, t))<<8);
    interpolated |=        ((lerp( (_maskB & a.Value),      (_maskB & b.Value), t)));

    return Color(interpolated);
}

Color Color::alphaBlend(const Color& bg, const Color& ov)
{
    static const Uint_32 _maskA = 0xff000000;
    static const Uint_32 _maskR = 0x00ff0000;
    static const Uint_32 _maskG = 0x0000ff00;
    static const Uint_32 _maskB = 0x000000ff;
    Float_32 a = ((ov.Value & _maskA)>>24)/(Float_32)0xff;

    Uint_32 interpolated = 0xff000000;
    interpolated |=        ((lerp( (_maskR & bg.Value)>>16,  (_maskR & ov.Value)>>16, a))<<16);
    interpolated |=        ((lerp( (_maskG & bg.Value)>>8,   (_maskG & ov.Value)>>8, a))<<8);
    interpolated |=        ((lerp( (_maskB & bg.Value),      (_maskB & ov.Value), a)));

    return Color(interpolated);
}

void Color::RetrieveComponenets(Byte_8* r, Byte_8* g, Byte_8* b, Byte_8* a)
{
    switch (formatInMemory)
    {
        case RGBA:
        {
            *a = (Value>>24) & 0x000000ff;
            *b = (Value>>16)  & 0x000000ff;
            *g = (Value>>8)  & 0x000000ff;
            *r =  Value  & 0x000000ff;
            printf("%hx, %hx, %hx, %hx\n", *a, *b, *g, *r);
        }   
            break;
        case BGRA:
        {
            *a = (Value>>24) & 0x000000ff;
            *r = (Value>>16)  & 0x000000ff;
            *g = (Value>>8)  & 0x000000ff;
            *b =  Value  & 0x000000ff;
        }
            break;
        case ARGB:
        {
            *b = (Value>>24) & 0x000000ff;
            *g = (Value>>16)  & 0x000000ff;
            *r = (Value>>8)  & 0x000000ff;
            *a =  Value  & 0x000000ff;
        }
            break;
    }
}


#endif
