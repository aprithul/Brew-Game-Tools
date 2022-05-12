#include "GraphicsUtil.hpp"


Color::Color(Byte_8 r, Byte_8 g, Byte_8 b, Byte_8 a, ColorFormat _formatInMemory)
{
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